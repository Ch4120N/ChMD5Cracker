#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>


#ifdef _WIN32
    #include <windows.h>

    static void enableColors()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
#endif

#include "include/hash.h"
#include "include/decorators.h"

using namespace std;
using namespace chrono;


class Ch4120N_MD5_HASH_CRACKER{
private:
    const int dbgtmr = 1;
    bool verbose_mode = false;
    string charset = "";
    int min_len, max_len;
    MD5 md5_hex;

    // Thread pool and synchronization
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queue_mutex;
    condition_variable condition;
    atomic<bool> stop_pool{false};
    atomic<long long> total_counter{0};
    atomic<bool> password_found{false};
    mutex found_mutex;
    string found_password;

    // Timing
    steady_clock::time_point global_start_time;

    // Thread management
    unsigned int num_threads;

public:
    Ch4120N_MD5_HASH_CRACKER(int argc, char *argv[]);
    ~Ch4120N_MD5_HASH_CRACKER();

    void print_banner();
    void usage();
    void print_verbose(const string &password, const string &hash_result, const string &target_hash, int hashes_per_sec);
    bool check_hash(const string &password, const string &target_hash, int thread_id);

    // Thread pool methods
    void init_thread_pool();
    unsigned int get_cpu_cores();
    template <class F>
    auto enqueue(F &&f) -> future<decltype(f())>;

    // Cracking methods
    void crack_range(const string &charset, int length, const string &target_hash,
                     size_t start_idx, size_t end_idx, int thread_id);
    void generate_combinations_range(const string &charset, string &current, int length,
                                     int pos, const string &target_hash, int thread_id,
                                     size_t start_char, size_t end_char);
    void distribute_work(const string &charset, int length, const string &target_hash);
    void crack(const string &charset, int length, const string &target_hash);

    // Monitoring thread
    void monitor_progress(const string &target_hash);

};

// Thread-safe wrapper for MD5
class ThreadSafeMD5
{
private:
    MD5 md5;
    mutex mtx;

public:
    string hash(const string &str)
    {
        lock_guard<mutex> lock(mtx);
        return md5.hash(str);
    }
};

// Global thread-safe MD5 instance
ThreadSafeMD5 thread_safe_md5;

int main(int argc, char **argv)
{
#ifdef _WIN32
    enableColors();
#endif
    Ch4120N_MD5_HASH_CRACKER app(argc, argv);
    return 0;
}

unsigned int Ch4120N_MD5_HASH_CRACKER::get_cpu_cores()
{
    unsigned int cores = thread::hardware_concurrency();

    if (cores == 0)
    {
// Fallback methods if hardware_concurrency() returns 0
#ifdef _WIN32
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        cores = sysinfo.dwNumberOfProcessors;
#elif defined(__linux__)
        cores = sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(__APPLE__)
        cores = sysconf(_SC_NPROCESSORS_ONLN);
#else
        cores = 4; // Default fallback
#endif
    }

    // Use logical processors (hyper-threading) for maximum performance
    cout << InfoMessage("CPU Cores detected: ") << color_code(Color::FG_BRIGHT_GREEN) << cores << endl;

    // Adjust based on system capability
    if (cores >= 8)
    {
        return cores; // Use all cores on powerful systems
    }
    else if (cores >= 4)
    {
        return cores; // Use all cores on mid-range systems
    }
    else
    {
        return max(2u, cores); // At least 2 threads on low-end systems
    }
}

void Ch4120N_MD5_HASH_CRACKER::init_thread_pool()
{
    num_threads = get_cpu_cores();
    cout << InfoMessage("Initializing thread pool with " + to_string(num_threads) + " workers") << endl;
    // cout << "[ * ] Initializing thread pool with " << num_threads << " workers" << endl;

    // Start worker threads
    for (unsigned int i = 0; i < num_threads; ++i)
    {
        workers.emplace_back([this, i]
                             {
            while (true) {
                function<void()> task;
                {
                    unique_lock<mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] {
                        return this->stop_pool || !this->tasks.empty();
                    });
                    
                    if (this->stop_pool && this->tasks.empty()) {
                        return;
                    }
                    
                    task = move(this->tasks.front());
                    this->tasks.pop();
                }
                
                task();
                
                // Check if password was found
                if (password_found.load()) {
                    return;
                }
            } });
    }
}

template <class F>
auto Ch4120N_MD5_HASH_CRACKER::enqueue(F &&f) -> future<decltype(f())>
{
    using return_type = decltype(f());

    auto task = make_shared<packaged_task<return_type()>>(
        bind(forward<F>(f)));

    future<return_type> res = task->get_future();
    {
        lock_guard<mutex> lock(queue_mutex);
        if (stop_pool)
        {
            throw runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace([task]()
                      { (*task)(); });
    }

    condition.notify_one();
    return res;
}

Ch4120N_MD5_HASH_CRACKER::~Ch4120N_MD5_HASH_CRACKER()
{
    stop_pool = true;
    condition.notify_all();
    for (thread &worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

void Ch4120N_MD5_HASH_CRACKER::monitor_progress(const string &target_hash)
{
    auto last_print_time = steady_clock::now();

    while (!password_found.load() && !stop_pool)
    {
        this_thread::sleep_for(chrono::seconds(1));

        auto current_time = steady_clock::now();
        auto elapsed = duration_cast<seconds>(current_time - global_start_time).count();

        if (!verbose_mode && elapsed >= dbgtmr && current_time - last_print_time >= chrono::seconds(dbgtmr))
        {
            auto local_counter = total_counter.exchange(0);
            double hashes_per_sec = local_counter / dbgtmr;

            cout << ProgressMessage("Speed: ") << color_code(Color::FG_BRIGHT_GREEN) << static_cast<int>(hashes_per_sec)
                 << " hashes/sec" << color_code(Color::FG_WHITE) << " | Total: " << color_code(Color::FG_BRIGHT_GREEN) << elapsed << " secs";
            cout.flush();

            last_print_time = current_time;
        }
    }
}

bool Ch4120N_MD5_HASH_CRACKER::check_hash(const string &password, const string &target_hash, int thread_id)
{
    // Calculate MD5 hash using thread-safe wrapper
    string hash_result = thread_safe_md5.hash(password);

    // Increment global counter
    total_counter++;

    // Check if hash matches
    if (target_hash == hash_result && !password_found.load())
    {
        lock_guard<mutex> lock(found_mutex);
        if (!password_found.load())
        {
            password_found.store(true);
            found_password = password;
            return true;
        }
    }

    // Print verbose output if enabled
    if (verbose_mode && total_counter % 1000 == 0) // Limit verbose output
    {
        auto current_time = steady_clock::now();
        auto elapsed = duration_cast<seconds>(current_time - global_start_time).count();
        if (elapsed > 0)
        {
            int hashes_per_sec = total_counter / elapsed;
            print_verbose(password, hash_result, target_hash, hashes_per_sec);
        }
    }

    return false;
}