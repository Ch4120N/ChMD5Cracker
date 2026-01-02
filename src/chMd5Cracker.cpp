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

Ch4120N_MD5_HASH_CRACKER::Ch4120N_MD5_HASH_CRACKER(int argc, char *argv[])
{
    print_banner();

    if (dbgtmr <= 0)
    {
        cout << FailMessage("Set dbgtmr to a value >=1 !") << endl;
        return;
    }

    if (argc < 5)
    {
        usage();
    }

    vector<string> args;
    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    string options = args[0];

    if (options.find('v') != string::npos)
    {
        verbose_mode = true;
    }

    if (options.find('a') != string::npos)
    {
        charset += "abcdefghijklmnopqrstuvwxyz";
    }
    if (options.find('A') != string::npos)
    {
        charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if (options.find('n') != string::npos)
    {
        charset += "0123456789";
    }
    if (options.find('s') != string::npos)
    {
        charset += "!\"$%&/()=?-.:\\*'-_:.;,";
    }

    if (options.find('m') != string::npos)
    {
        charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    }

    string clean_charset;
    unordered_set<char> seen;
    for (char c : charset)
    {
        if (seen.insert(c).second)
        { // insert returns true if element was new
            clean_charset += c;
        }
    }

    charset = std::move(clean_charset);

    if (charset.empty())
    {
        usage();
    }

    try
    {
        min_len = stoi(args[1]);
        max_len = stoi(args[2]);
    }
    catch (...)
    {
        usage();
    }

    if (min_len < 1 || max_len < min_len)
    {
        usage();
    }

    string target_hash = args[3];

    for (char &c : target_hash)
    {
        c = tolower(c);
    }

    if (target_hash.length() != 32)
    {
        cout << FailMessage("Sorry but it seems that the MD5 is not valid!", Color::FG_BRIGHT_RED) << endl;
        return;
    }

    cout << InfoMessage("Selected charset for attack: '") << color_code(Color::FG_BRIGHT_GREEN) << charset << color_code(Color::FG_WHITE) << "'" << std::endl;
    cout << InfoMessage("Target Hash: '") << color_code(Color::FG_BRIGHT_GREEN) << target_hash << color_code(Color::FG_WHITE) << "'" << std::endl;
    cout << InfoMessage("Length range: ") << color_code(Color::FG_BRIGHT_GREEN) << min_len << color_code(Color::FG_WHITE) << " to " << color_code(Color::FG_BRIGHT_GREEN) << max_len << color_code(Color::FG_WHITE) << " characters" << std::endl;

    // Initialize thread pool
    init_thread_pool();

    global_start_time = steady_clock::now();
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

void Ch4120N_MD5_HASH_CRACKER::generate_combinations_range(const string &charset, string &current, int length,
                                                           int pos, const string &target_hash, int thread_id,
                                                           size_t start_char, size_t end_char)
{
    if (password_found.load())
        return;

    if (pos == length)
    {
        if (check_hash(current, target_hash, thread_id))
        {
            return;
        }
        return;
    }

    // For the first position, only iterate through assigned character range
    if (pos == 0)
    {
        for (size_t i = start_char; i < end_char && !password_found.load(); ++i)
        {
            current[pos] = charset[i];
            generate_combinations_range(charset, current, length, pos + 1, target_hash,
                                        thread_id, 0, charset.size());
        }
    }
    else
    {
        // For subsequent positions, iterate through all characters
        for (char c : charset)
        {
            if (password_found.load())
                return;
            current[pos] = c;
            generate_combinations_range(charset, current, length, pos + 1, target_hash,
                                        thread_id, 0, charset.size());
        }
    }
}

void Ch4120N_MD5_HASH_CRACKER::crack_range(const string &charset, int length, const string &target_hash,
                                           size_t start_idx, size_t end_idx, int thread_id)
{
    if (start_idx >= end_idx || password_found.load())
        return;

    string current(length, ' ');
    generate_combinations_range(charset, current, length, 0, target_hash, thread_id, start_idx, end_idx);
}

void Ch4120N_MD5_HASH_CRACKER::distribute_work(const string &charset, int length, const string &target_hash)
{
    // Distribute first character among threads
    size_t charset_size = charset.size();
    size_t chars_per_thread = charset_size / num_threads;
    size_t remainder = charset_size % num_threads;

    vector<future<void>> futures;

    size_t start = 0;
    for (unsigned int i = 0; i < num_threads; ++i)
    {
        size_t end = start + chars_per_thread + (i < remainder ? 1 : 0);

        if (start < charset_size)
        {
            futures.emplace_back(
                enqueue([this, charset, length, target_hash, start, end, i]()
                        { this->crack_range(charset, length, target_hash, start, end, i); }));
        }

        start = end;
    }

    // Wait for all tasks to complete
    for (auto &future : futures)
    {
        future.wait();
    }
}

void Ch4120N_MD5_HASH_CRACKER::crack(const string &charset, int length, const string &target_hash)
{
    distribute_work(charset, length, target_hash);
}

void Ch4120N_MD5_HASH_CRACKER::print_banner()
{
    cout << color_code(Color::FG_BRIGHT_RED) << "  ___  _   _  __  __  ____   ___   ___  ____    __    ___  _  _  ____  ____ \n";
    cout << " / __)( )_( )(  \\/  )(  _ \\ | __) / __)(  _ \\  /__\\  / __)( )/ )( ___)(  _ \\\n";
    cout << "( (__  ) _ (  )    (  )(_) )|__ \\( (__  )   / /(__)\\( (__  )  (  )__)  )   /\n";
    cout << " \\___)(_) (_)(_/\\/\\_)(____/ (___/ \\___)(_)\\_)(__)(__)\\___)(_)\\_)(____)(_)\\_)\n\n";
    cout << color_code(Color::FG_BRIGHT_GREEN) << "                         POWERED BY Ch4120N" << endl;
    cout << color_code(Color::FG_BRIGHT_WHITE) << "                     MULTI-THREADED EDITION v2.0" << endl;
    cout << color_code(Color::FG_BRIGHT_BLUE) << "                     https://GitHub.com/Ch4120N" << endl;
    cout << endl
         << color_code(Color::RESET);
}

void Ch4120N_MD5_HASH_CRACKER::usage()
{
    // ----------------------- PREVIOUSE VERSION BANNER -------------------------------------------
    // cout << "\n";
    // cout << " #############################################################################\n";
    // cout << " # CH4120N MD5 Hash Cracker - Powerful MD5 Hash Cracking Tool                #\n";
    // cout << " # Owner: CH4120N                                                            #\n";
    // cout << " # Version: Multi-Threaded v2.0                                              #\n";
    // cout << " #                                                                           #\n";
    // cout << " # Usage: ./chMd5Cracker <setChar> <minChar> <maxChar> <hashMD5>             #\n";
    // cout << " # Character options: a - small letters   # a,b,c                            #\n";
    // cout << " #                    A - big letters     # A,B,C                            #\n";
    // cout << " #                    n - numbers         # 1,2,3                            #\n";
    // cout << " #                    s - symbols         # !,#,@                            #\n";
    // cout << " #                    v - verbose mode    # Show detailed progress           #\n";
    // cout << " #                                                                           #\n";
    // cout << " # Example: ./chMd5Cracker aAnsv 1 4 9bd4f3afae4f32050d2b0e467c9fb8ef        #\n";
    // cout << " #                                                                           #\n";
    // cout << " #############################################################################\n";

    cout << color_code(Color::FG_WHITE) << " USAGE: ./chMd5Cracker <setChar> <minChar> <maxChar> <hashMD5>\n\n";
    cout << " CHARACTER SET OPTIONS:\n";
    cout << "\ta\t- Lowercase letters\t| a, b, c, ..., z\n";
    cout << "\tA\t- Uppercase letters\t| A, B, C, ..., Z\n";
    cout << "\tn\t- Numbers\t| 0, 1, 2, ..., 9\n";
    cout << "\ts\t- Special symbols\t| !, @, #, $, %, ...\n";
    cout << "\tm\t- Mixed case\t| a-z, A-Z\n";
    cout << "\tv\t- Verbose mode\t| Detailed progress\n\n";
    cout << " EXAMPLES:\n";
    cout << "\tBasic (4-char lowercase + numbers):\n";
    cout << "\t    ./chMd5Cracker an 4 6 5f4dcc3b5aa765d61d8327deb882cf99\n\n";
    cout << "\tAdvanced (all chars, 1-8 length, verbose):\n";
    cout << "\t    ./chMd5Cracker aAnsv 1 8 e99a18c428cb38d5f260853678922e03\n\n";
    cout << " NOTES:\n";
    cout << "\t- High-performance tool. Use responsibly!\n";
    cout << "\t- Estimated speed: 500K-2M hashes/sec (depending on CPU)\n";
    cout << "\t- Quick Tip: Combine character sets for better results (e.g., 'aAn' for alphanumeric)\n";
    cout << "\t- Press Ctrl+C to Stop the opration!\n"
         << color_code(Color::RESET);

    exit(1);
}

void Ch4120N_MD5_HASH_CRACKER::print_verbose(const string &password, const string &hash_result,
                                             const string &target_hash, int hashes_per_sec)
{
    cout << ProgressMessage(color_code(Color::FG_WHITE) +
                            target_hash + color_code(Color::FG_BRIGHT_MAGENTA) + " => " +
                            color_code(Color::FG_BRIGHT_GREEN) + hash_result + color_code(Color::FG_BRIGHT_MAGENTA) +
                            " => " + color_code(Color::FG_BRIGHT_GREEN) + password + color_code(Color::FG_WHITE) + " | Speed: " + color_code(Color::FG_BRIGHT_GREEN) + to_string(hashes_per_sec) + " hashes/sec.");
    cout.flush();
}