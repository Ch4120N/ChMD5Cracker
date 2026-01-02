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


