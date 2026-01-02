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
    Ch4120N_Md5_Hash_Cracker(int argc, char *argv[]);
    ~Ch4120N_Md5_Hash_Cracker();

};