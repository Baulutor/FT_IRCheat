# IRC (Internet Relay Chat) - 42 Project

## 📚 Project Overview

The **IRC** project from [42 School](https://www.42.fr/) involves building a simplified IRC server that communicates over TCP using the Internet Relay Chat protocol as described in [RFC 1459](https://tools.ietf.org/html/rfc1459). The server must support multiple clients, manage channels, handle user commands, and follow the expected behavior of a real IRC network.

The project focuses on:
- Handling network programming through sockets.
- Managing multiple client connections concurrently.
- Parsing a complex text-based protocol.
- Implementing command and state management.
- Ensuring compatibility with both **CLI clients** (`nc`) and **GUI clients** like **HexChat**.

---

## ✅ Features

### 🧠 Protocol Compatibility
- Full compatibility with **HexChat** (a graphical IRC client).
- Support for simple **CLI testing** using `nc` (netcat).

### 🧵 Multi-Client Support
- Non-blocking socket communication.
- Multiple clients handled using `poll()` or `select()`.
- Clean client disconnection and resource management.

### 🔌 Socket Programming
- IPv4 socket setup using `socket()`, `bind()`, `listen()`, and `accept()`.
- Communication over TCP with `recv()` and `send()`.
- Select-based I/O multiplexing for handling many clients in a single thread.

### 🛂 IRC Features Implemented
- **USER** and **NICK** registration.
- **JOIN**, **PART**, **PRIVMSG**, **NOTICE**.
- Channel management (`#channel`): creation, join, user listing.
- **PING/PONG** to keep connections alive.
- Proper **error responses** and numeric replies following RFC standards.
- Server password authentication at connection (via CLI or HexChat).

---

## 💻 How to Use

### 🛠️ Compile and Run

```bash
make
./ircserv <port> <password>
```

## Example:
./ircserv 6667 mypass123


### 🧪 Testing with Netcat
```
nc 127.0.0.1 6667
```
Then manually type IRC commands:
```
PASS mypass123
NICK testuser
USER testuser 0 * :Real Name
JOIN #channel
PRIVMSG #channel :Hello from netcat!
```
### 💬 Testing with HexChat
- Open HexChat → Network List → Add a new network (e.g. MyIRC).

- Add a server: 127.0.0.1/6667

- Set your nickname, real name, and connect.

- Enter password when prompted (/PASS mypass123), or configure it in the network settings.

- Join a channel (/join #channel) and chat normally.

- ✔️ HexChat compatibility requires careful handling of the protocol and replies — the server simulates a real IRC experience for GUI clients.

### 🧩 Parsing & Command Handling
- IRC communication relies heavily on line-based string parsing. I built a custom parser that:

- Splits commands and parameters according to the protocol (<command> [params] :<trailing>)

- Handles quoted strings and prefixes (e.g., :nick!user@host)

- Triggers internal handlers for each command (e.g., PRIVMSG, JOIN, PING)

- Maintains stateful tracking of users, channels, and modes

### 🧰 Internal Architecture
Client Structure:

- Socket file descriptor

- Nickname, username, real name

- Channel membership

- Authentication state

Channel Structure:

- Name

- List of clients

- Operators and topic (optional)

Main Loop:

- Uses poll() (or select()) to monitor sockets

- Accepts new connections

- Parses incoming lines and dispatches commands

- Sends responses based on command results

- Thread-Safe (or handled in a single-threaded event loop for simplicity)

### ⚠️ Challenges and Considerations
- Proper protocol parsing (RFC 1459 is nuanced)

- Keeping responses compatible with HexChat's expectations

- Maintaining clean disconnections to avoid dangling sockets

- Handling simultaneous JOIN/PART/PRIVMSG across channels

- Implementing error codes correctly (e.g., 433 for nick in use)

### 🧪 Example Commands
```
./ircserv

PASS mypass123
NICK alice
USER alice 0 * :Alice Example
JOIN #42
PRIVMSG #42 :Hello world!
PART #42

QUIT :Bye!
```
