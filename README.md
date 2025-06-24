# Garbage-Collector-in-C
# 🧠 Garbage-Collected Virtual Machine (GC VM) in C

This project implements a simple Garbage-Collected Virtual Machine (GC VM) using the C programming language. It simulates dynamic memory management and automatic garbage collection as seen in modern language runtimes and compiler backends.

## 📚 Overview

The VM supports dynamic allocation of:
- Integers
- Strings
- Pairs (similar to Lisp-style cons cells)

Memory management is handled by a custom **Mark-and-Sweep Garbage Collector**, which identifies and frees unreachable objects automatically. A root stack simulates live references, protecting objects from being collected prematurely.

## 🛠️ Features

- Manual and automatic garbage collection
- Object linking and reference tracking
- Root stack for live object simulation
- Heap printing and diagnostics
- Fully modular C implementation

## 🧱 Project Structure

| File         | Description                                      |
|--------------|--------------------------------------------------|
| `main.c`     | User interface and test program                  |
| `vm.c/h`     | Virtual machine logic and object management      |
| `object.c/h` | Object type definitions and allocation logic     |
| `gc.c`       | Mark-and-sweep garbage collection implementation |

## 🔧 Build Instructions

### Requirements
- GCC or any C compiler
- Make (optional, for easier build)

### Build using terminal:
```bash
gcc -o gc_test main.c vm.c object.c gc.c
./gc_test
```

Or use:
```bash
make
./gc_test
```

## 🚀 How to Use

1. Run the program.
2. Input integers and strings to allocate memory.
3. Trigger garbage collection manually.
4. View memory before and after collection.
5. All memory is freed on program exit.

## 🧹 Garbage Collection Process

- **Mark Phase**: Traverse and mark all reachable objects from the root stack.
- **Sweep Phase**: Free all unmarked (unreachable) objects.

## 🧠 Compiler Design Relevance

This project represents the **runtime system** phase of a compiler:
- Simulates memory management after code generation
- Demonstrates how compilers track and collect unused memory
- Models how language runtimes (Java, Python) work internally

## 👥 Team Roles

- Member 1: VM logic and root stack
- Member 2: Object allocation and GC integration
- Member 3: Garbage collection implementation
- Member 4: User interface, testing, and cleanup

## 📄 License

This project is for educational purposes only.
