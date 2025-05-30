# 🖥️ Remote Shell with Virtual Spaces

A multithreaded C++ Remote Shell server that simulates isolated terminal sessions for multiple users. Built using raw POSIX sockets, Boost threads, and signal-safe architecture, with RESTful APIs powered by Crow for real-time session introspection and system monitoring.

## 🚀 Features

- 🔐 **Virtual Terminal Spaces**: Each user gets a separate shell-like environment, isolated from others.
- ⚙️ **Multithreaded Server**: Handles multiple client sessions in parallel using `std::thread`.
- 📡 **POSIX Sockets**: Low-level socket programming to simulate a secure shell behavior.
- 🌐 **Crow REST API**: `GET /ping`, with extensibility for session stats and management.
- 🧠 **Signal-Safe Shutdown**: Custom `SIGINT` and `SIGPIPE` handlers for graceful termination.
- 📈 **Color-coded CLI**: Styled output using ANSI escape codes for better UX.

## 🧰 Tech Stack

- C++17
- POSIX Sockets
- Boost.Thread
- Crow (header-only web framework)
- Linux/macOS
- CMake
- Catch2 & GoogleTest

## 📦 Build & Run

### Prerequisites

- CMake ≥ 3.14
- Git
- g++ or clang++
- Linux/macOS system

### 🛠 Build

```bash
git clone https://github.com/yourusername/remote-shell.git
cd remote-shell
chmod +x run.sh (For faster builds run ./Minimal run.sh)
./run.sh
```

> `run.sh` handles building with CMake and launching both the shell server and REST interface.

### 🌐 REST API (Port 8081)
- GET /ping → { "message": "Ping successful" }

### 🖧 Shell Client (Port 8080)
- Once connected, you can run commands like:
    - ⚠️ Default ports are 8080 (shell) and 8081 (REST). Ensure they're free before running.
    - You can customize them by setting `shellPort` and `restPort` in the main.cpp file.

```sh
 ./run.sh  (For faster builds run ./Minimal run.sh)

Once connected, you can run commands like:

```sh
ls
whoami
uname -a
```

## 📁 Project Structure

```
.
├── src/
│   ├── server/
│   ├── client/
│   └── ...
├── include/
├── tests/
├── build_and_run.sh
└── CMakeLists.txt
```

## ✅ To-Do

- [ ] Session authentication
- [ ] WebSocket live feed
- [ ] Container-based user isolation
- [ ] Command history tracking

## 📝 License and Platforms

![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macOS-blue)
![License](https://img.shields.io/badge/license-MIT-green)

---


> Made with ❤️ by Ronak