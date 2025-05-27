# 🖥️ Remote Shell with Virtual Spaces

A multithreaded C++ Remote Shell server that simulates isolated terminal sessions for multiple users. Built using raw POSIX sockets, Boost threads, and custom signal handling, with RESTful APIs powered by Crow for real-time session introspection and system monitoring.

## 🚀 Features

- 🔐 **Virtual Terminal Spaces**: Each user gets a separate shell-like environment, isolated from others.
- ⚙️ **Multithreaded Server**: Handles multiple client sessions in parallel using `std::thread`.
- 📡 **POSIX Sockets**: Low-level socket programming to simulate a secure shell.
- 🌐 **Crow REST API**: Real-time status endpoints with `GET /ping` and room for session stats and management.
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
chmod +x build_and_run.sh
./build_and_run.sh
```

> `build_and_run.sh` handles building with CMake and launching both the shell server and REST interface.

### 🌐 REST API (Port 8081)

- `GET /ping` → `{ "message": "Ping successful" }`

### 🖧 Shell Client (Port 8080)

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

## 📝 License

MIT License. Feel free to fork and customize!

---

> Made with ❤️ by Ronak