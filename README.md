<head>
  <meta name="google-site-verification" content="l4gzIHopgDDt57xRYeRvJZ5DYgg4lLb-qPciUxhNxkY" />
</head>

<div align="center">

# **Ch4120N MD5 Hash Cracker**
### _**Multi-Threaded MD5 Hash Cracker with Smart Distribution**_

![Language](https://img.shields.io/badge/Language-C++_v17-purple?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-All-red?style=for-the-badge)
![Version](https://img.shields.io/badge/Version-2.0.0-blue?style=for-the-badge)
![LISENCE](https://img.shields.io/badge/License-GPL%20v3.0-orange?style=for-the-badge)

---

> **Ch4120N MD5 Hash Cracker** is a powerful, high-performance, cross-platform MD5 hash cracking tool with multi-threading support and intelligent workload distribution.

</div>

---

## 👨‍💻 Project Programmer

> **Ch4120N** — [Ch4120N@Proton.me](mailto:Ch4120N@Proton.me)

---
## 🧠 Made For

> **Security professionals** and **penetration testers** who require a powerful **MD5 hash cracker** with **intelligent workload distribution** for **authorized security audits**, **forensic analysis**, and **password recovery**.

---

## 🖼️ Screenshots

<div align="center">

| Usage | Cracking Progress
| :---: | :---:
| <img src="./images/screenshot1.png" alt="ChMD5Cracker Main Usage" width="400" title="ChMD5Cracker Main Usage Info"/> | <img src="./images/screenshot2.png" alt="ChMD5Cracker On Cracking Progress" width="400" title="ChMD5Cracker On Cracking Progress"/>

</div>

---
## ✨ Features
### ⚡ Performance & Efficiency
- **Multi-threading**: Automatically detects CPU cores and utilizes all available threads
- **Optimized Work Distribution**: Intelligent task partitioning for maximum throughput
- **Real-time Progress Monitoring**: Live hash rate display and progress tracking
- **High-Speed Cracking**: Capable of processing millions of hashes per second
### 🎯 Advanced Capabilities
- **Custom Character Sets**: Flexible charset configuration with elimination of duplicates
- **Variable Password Length**: Support for minimum and maximum password lengths
- **Verbose Mode**: Detailed real-time output for debugging and analysis
- **Color-coded Output**: Beautiful terminal interface with cross-platform color support
- **Thread-Safe Operations**: Safe concurrent hash computation and state management
### 🔧 Technical Excellence
- **Cross-Platform**: Native support for Windows (MSVC/MinGW), Linux, and macOS
- **Modern C++17**: Leverages latest C++ features for optimal performance
- **Professional Build System**: CMake-based with proper dependency management
- **Clean Architecture**: Well-structured object-oriented design with proper RAII

---

## 💻 Supported Platforms

- [x] **Linux** (All distributions)
- [x] **Windows** (10+)
- [x] **macOS** (All versions)

---

## 🛠️ Build Instructions
### Prerequisites
- **C++17 compatible compiler** (g++ 8+, clang 7+, MSVC 2019+)
- **CMake 3.16+**
- **Git** (for cloning)

### Platform-Specific Builds
#### **Windows (MSVC)**
```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -DUSE_MSVC=ON
cmake --build . --config Release
```

#### **Windows (MinGW)**
```cmd
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DUSE_MINGW=ON
mingw32-make
```

#### **Linux**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

#### **MacOSX**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

#### **Advanced Build Options**
```bash
# Enable verbose mode compilation
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON

# Build with clang
cmake .. -DCMAKE_CXX_COMPILER=clang++

# Enable address sanitizer (debug builds)
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
```

---