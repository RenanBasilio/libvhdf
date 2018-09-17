# Virtual Hard Disk on File Library

[![Build status](https://ci.appveyor.com/api/projects/status/t4mdl7sg2ar38bsu/branch/master?svg=true)](https://ci.appveyor.com/project/RenanBasilio/libvhdf/branch/master)
[![Build Status](https://travis-ci.org/RenanBasilio/libvhdf.svg?branch=master)](https://travis-ci.org/RenanBasilio/libvhdf)


This project is a C++ shared library designed to provide a virtual disk interface for programs that need it.

* [Building From Source](#building-from-source)
* [Usage](#usage)
  * [Open Disk](#open-disk)
  * [Close Disk](#close-disk)
  * [Read Block](#read-block)
  * [Write Block](#write-block)
  * [Sync Disk](#sync-disk)
* [Suggestions and Bug Reports](#suggestions-and-bug-reports)

# Building From Source

Requirements:
* CMake
* A supported C++ compiler

## CMake

In your terminal of choice, call CMake to construct the build scripts and build.
```
cmake . -Bbuild
cmake --build build --config Release
```
Optionally, you may set the `BUILD_TESTS` option to `ON` in order to build assertion tests. The `--config Release` flag is optional on unix based systems.

## Linking

To link with this library, simply `#include <vhdf.hpp>` in your source file and link with the `-lvhdf` flag, provided that the library is installed in the system PATH.

# Usage

The library provides the following methods in the `vhdf` namespace:

## Open Disk
```c++
int openDisk(char* filename, size_t nbytes = 0, bool nosparse = false);
```
 Open the disk file with "filename", or creates it if it does not exist. Returns a descriptor for the opened disk on success or -1 on failure.

Parameters:
 * **filename** The name of the file containing the disk image.
 * **nbytes** If creating the disk, the size of the disk to make. Will be rounded down to the nearest multiple of BLOCK_SIZE.
 * **nosparce** Whether to force writing garbage data to the created file. This will keep the system from treating it as sparse and avoid fragmentation as it grows.


## Close Disk
```c++
int closeDisk(int disk);
```
Closes the disk described by "disk". Returns 0 on success, -1 on failure. 
 
Parameters:
 * **disk** The disk descriptor.

## Read Block
```c++
int readBlock(int disk, size_t blocknr, void* buff);
```
Reads data from the disk and places it on the provided buffer. Returns 0 on success, or EOF on _blocknr_ out of bounds.

Parameters:
 * **disk** The descriptor for the disk to read from.
 * **blocknr** The number of the block to read.
 * **buff** The buffer in which to place read bytes.

## Write Block
```c++
int writeBlock(int disk, size_t blocknr, void* buff);
```
Writes data on the provided buffer to the disk. Returns 0 on success, or EOF on _blocknr_ out of bounds.

Parameters:
 * **disk** The descriptor for the disk to be written.
 * **blocknr**  The number of the block to write to.
 * **buff** The buffer containing the data to write.

## Sync Disk
```c++
void syncDisk();
```
Forces all outstanding writes to disk. Currently useless as write caching is not implemented.

# Suggestions and Bug Reports

If you happen to encounter any bugs while using this library, or have any suggestion of additional features you would like to see implemented, please open an [issue on GitHub](https://github.com/RenanBasilio/libvhdf/issues).