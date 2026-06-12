# Setup Guide: TA-Lib C++ Project

This guide walks you through cloning, compiling, and installing the native C/C++ TA-Lib dependencies, followed by building the C++ example project.


## 1. Prerequisites & Dependencies

Before beginning, ensure your system has the necessary build tools (`make`, `cmake`, a C++17 compatible compiler) along with `automake` and `libtool` to generate TA-Lib's configuration scripts.

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake automake libtool git -y
```

## 2. Download and Build TA-Lib (Core C/C++ Library)

The GitHub repository contains the raw source code but lacks the pre-generated ./configure script. We use autogen.sh to bootstrap the build environment.

1. Clone the official TA-Lib repository

```bash
git clone git@github.com:TA-Lib/ta-lib.git
```

2. Navigate into the cloned directory

```bash
cd ta-lib/
```

3. Ensure the bootstrap script is executable and run it

```bash
chmod +x ./autogen.sh
./autogen.sh
```

4. Configure the build directory (installed to /usr/local by default)

```bash
./configure --prefix=/usr/local
```

5. Compile the library

```bash
make
```

6. Install the headers and binaries to your system

```bash
sudo make install
```


Linux Only: Update Linker Cache

On some Linux systems, you need to tell the dynamic linker to refresh its cache so it can immediately find the newly installed libta_lib.so

```bash
sudo ldconfig
```

## 3. Verify TA-Lib Installation

To quickly verify that TA-Lib successfully installed to your system paths, check for the headers and compiled libraries:

Verify headers exist

```bash
ls /usr/local/include/ta-lib/ta_libc.h
```

Verify binaries exist

```bash
ls /usr/local/lib/libta-lib.*
```

You should see files like libta-lib.a, libta-lib.la, libta-lib.so (depending on your OS).

## 4. Build Your Project

With the core dependency installed, navigate back to your own project directory containing your CMakeLists.txt and main.cpp.

Navigate to your project directory (if separate from the ta-lib source)

```bash
cd /path/to/your/project
```

1. Configure the project and generate the build system inside the out/build directory

```bash
cmake -S . -B out/build/
```

2. Compile the project

```bash
make -C out/build/
```

## 5. Running the Application

By default, the compiled binary will be placed inside the bin/ directory relative to your project root (as defined in your CMakeLists.txt).

Execute the generated binary

```bash
./bin/test
```
