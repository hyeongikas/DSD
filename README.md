**DSD - A Custom, Lightweight Unix-like Operating System**
==========================================================

A simple, Unix-like operating system written in C, inspired by BSD.

**Table of Contents**
---------------------

* [Overview](#overview)
* [Features](#features)
* [Building and Running](#building-and-running)
* [Contributing](#contributing)
* [License](#license)
* [Acknowledgments](#acknowledgments)

<a name="overview"></a>

## Overview
---------

DSD is a minimalist, Unix-like operating system designed for educational purposes. It provides a basic process management system, allowing developers to learn about OS internals and experiment with custom OS development.

<a name="features"></a>

## Features
---------

* **Process Management**: Supports up to 10 processes, each with its own stack and program counter.
* **Process States**: Processes can be in one of three states: ready, running, or waiting.
* **Process Scheduling**: A simple round-robin scheduler is implemented to switch between processes.
* **Process Creation**: New processes can be created using the `process_create` function, which takes an entry point function as an argument.

<a name="building-and-running"></a>

## Building and Running
---------------------

To build and run DSD, you'll need:

* A C compiler (e.g., GCC)
* A x86-64 architecture (e.g., QEMU or a virtual machine)

Follow these steps:

1. Clone this repository.
2. Compile the OS using `make` or your preferred build tool.
3. Run the OS using a emulator or virtual machine (e.g., `qemu-system-x86_64`).

<a name="contributing"></a>

## Contributing
-------------

Contributions are welcome! If you'd like to help improve DSD, please fork this repository and submit a pull request.

<a name="license"></a>

## License
--------

DSD is licensed under the MIT License. See `LICENSE` for details.

<a name="acknowledgments"></a>

## Acknowledgments
----------------

DSD is inspired by various Unix-like operating systems, including BSD. Thanks to all the developers who have contributed to these systems over the years!

### Process Header File (`process.h`)
```c
#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESSES 10
#define PROCESS_STACK_SIZE 4096

typedef struct {
    uint64_t* stack;
    uint64_t rip;
    uint64_t rsp;
    uint32_t pid;
    uint8_t state; // 0: ready, 1: running, 2: waiting
} process_t;

void process_init();
void process_create(void (*entry_point)());
void process_switch();
void process_scheduler();

#endif
