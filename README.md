# Command-line SQL Database Engine

A lightweight SQL database engine for command-line usage implemented in C++.

# Prerequisites

- CMake (version 3.10 or higher) and Make
- GCC or Clang with C++17 support

Run the following commands to install the prerequisites on Debian-based systems (ex. Ubuntu):

```
sudo apt update
sudo apt install build-essential cmake
```

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
echo \
"CREATE TABLE students (id, name, age);
INSERT INTO students (id, name, age) VALUES (1, Alice, 20);
SELECT * FROM students;" \
| ./db_engine
```

Output:
```
=== Custom Database Engine ===
Type 'help' for commands, 'quit' to exit

db> Query executed successfully

db> Query executed successfully

db> Query executed successfully. Found 1 rows.
1 | Alice | 20
Query executed successfully

db> Goodbye!
```