#pragma once
#include <cstddef>

#if defined(_MSC_VER)
    #ifdef LIBVHDF_EXPORTS
        #define LIBVHDF_API __declspec(dllexport)
    #else
        #define LIBVHDF_API __declspec(dllimport)
    #endif
#elif defined(__GNUC__)
    #ifdef LIBVHDF_EXPORTS
        #define LIBVHDF_API __attribute__((visibility("default")))
    #else
        #define LIBVHDF_API 
    #endif
#else
    #define LIBVHDF_API
#endif

namespace vhdf
{
    const static int BLOCK_SIZE = 512;

    /**
     * Open the disk file with "filename", or creates it if it does not exist.
     * @param {filename} The name of the file containing the disk image.
     * @param {nbytes} If creating the disk, the size of the disk to make. Will be rounded down to the nearest multiple of BLOCK_SIZE.
     * @param {nosparce} Whether to force writing garbage data to the created file. This will keep the system from treating it as sparse and avoid fragmentation as it grows.
     * @return A descriptor for the opened disk.
    */
    LIBVHDF_API int openDisk(const char* filename, size_t nbytes = 0, bool nosparce = false);

    /**
     * Closes the disk described by "disk".
     * @param {disk} The disk descriptor.
     * @return 0 on success, -1 on failure.
     */
    LIBVHDF_API int closeDisk(int disk);

    /**
     * Reads data from the disk and places it on the provided buffer.
     * @param {disk} The descriptor for the disk to read from.
     * @param {blocknr} The number of the block to read.
     * @param {buff} The buffer in which to place read bytes.
     * @return 0 on success, EOF on blocknr out of bounds.
     */
    LIBVHDF_API int readBlock(int disk, size_t blocknr, void* buff);

    /**
     * Writes data on the provided buffer to the disk.
     * @param {disk} The descriptor for the disk to be written.
     * @param {blocknr}  The number of the block to write to.
     * @param {buff} The buffer containing the data to write.
     * @return 0 on success, EOF on blocknr out of bounds.
     */
    LIBVHDF_API int writeBlock(int disk, size_t blocknr, void* buff);

    /**
     * Forces all outstanding writes.
     */
    LIBVHDF_API void syncDisk();   

}