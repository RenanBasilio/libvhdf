#include <vhdf.hpp>
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include <cstring>

#define ASSERT_TRUE {std::cout << "Passed" << std::endl; passed++;} else {std::cout << "Failed" << std::endl; failed++;}
#define ASSERT_FALSE {std::cout << "Failed" << std::endl; failed++;} else {std::cout << "Passed" << std::endl; passed++;}

void fillRandomBuffer(char* buff, size_t size) {
    for(size_t i = 0; i < size; i++)
        buff[i] = rand() % 256;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int passed = 0, failed = 0;
    int dsk;

    std::cout << "Test 1: Sparse Virtual Hard Disk creation... ";
    {
        /**
         * Test whether the library successfully creates a new disk file.
         */
        FILE* f;
        if ((f = fopen("testdisk.vhd", "r")) != NULL) { fclose(f); remove("testdisk.vhd"); }
        dsk = vhdf::openDisk("testdisk.vhd", 100000, false);
        if (dsk < 0) ASSERT_FALSE
    }

    std::cout << "Test 2: Closing Virtual Hard Disk handle... ";
    {
        /**
         * Test whether the library successfully closes the previously open 
         * disk.
         */
        if (vhdf::closeDisk(dsk) != 0 || remove("testdisk.vhd") != 0) ASSERT_FALSE
    }

    std::cout << "Test 3: Non-sparse Virtual Hard Disk creation... ";
    {
        /**
         * Check if the library successfully creates a non sparse data file
         * when supported by the operating system given that the flag is set.
         */
        dsk = vhdf::openDisk("testdisk.vhd", 100000, true);
        if (dsk < 0) ASSERT_FALSE
        vhdf::closeDisk(dsk);
        remove("testdisk.vhd");
    }

    std::cout << "Test 4: Fixed Block Write... ";
    {
        /**
         * Whether the library successfully writes a block to the disk file.
         */
        char wbuff[vhdf::BLOCK_SIZE], rbuff[vhdf::BLOCK_SIZE];

        dsk = vhdf::openDisk("testdisk.vhd", vhdf::BLOCK_SIZE*10, true);
        fillRandomBuffer(wbuff, vhdf::BLOCK_SIZE);
        if (dsk >= 0 && vhdf::writeBlock(dsk, 3, wbuff) == 0) {
            vhdf::closeDisk(dsk);
            std::ifstream stream("testdisk.vhd", std::ios::in | std::ios::binary);
            stream.seekg(vhdf::BLOCK_SIZE*3);
            stream.read(rbuff, vhdf::BLOCK_SIZE);
            stream.close();
            if( memcmp(wbuff, rbuff, vhdf::BLOCK_SIZE) == 0 ) ASSERT_TRUE;
        }
        remove("testdisk.vhd");
    }

    std::cout << "Test 5: Fixed Block Read... ";
    {
        /**
         * Whether the library successfully reads a block from the disk file.
         */
        char wbuff[vhdf::BLOCK_SIZE], rbuff[vhdf::BLOCK_SIZE];
        dsk = vhdf::openDisk("testdisk.vhd", vhdf::BLOCK_SIZE*12, true);
        vhdf::closeDisk(dsk);

        fillRandomBuffer(wbuff, vhdf::BLOCK_SIZE);
        std::ofstream stream("testdisk.vhd", std::ios::in | std::ios::out| std::ios::binary);
        stream.seekp(vhdf::BLOCK_SIZE*5);
        stream.write(wbuff, vhdf::BLOCK_SIZE);
        stream.close();

        dsk = vhdf::openDisk("testdisk.vhd");
        if (vhdf::readBlock(dsk, 5, rbuff) == 0 && memcmp(wbuff, rbuff, vhdf::BLOCK_SIZE) == 0) ASSERT_TRUE
        vhdf::closeDisk(dsk);
        remove("testdisk.vhd");
    }

    std::cout << "Test 6: Fixed Block Write/Read... ";
    {
        /**
         * Write a block and then read it back.
         */
        char wbuff[vhdf::BLOCK_SIZE], rbuff[vhdf::BLOCK_SIZE];
        fillRandomBuffer(wbuff, vhdf::BLOCK_SIZE);
        dsk = vhdf::openDisk("testdisk.vhd", vhdf::BLOCK_SIZE*10, true);
        vhdf::writeBlock(dsk, 9, wbuff);
        vhdf::readBlock(dsk, 9, rbuff);
        vhdf::closeDisk(dsk);
        if (memcmp(wbuff, rbuff, vhdf::BLOCK_SIZE) == 0) ASSERT_TRUE
    }
    
    std::cout << "Test 7: Bounds Checking... ";
    {
        /**
         * Determine whether the library refuses to write past the bounds of the
         * disk file.
         */
        char buff[vhdf::BLOCK_SIZE];
        dsk = vhdf::openDisk("testdisk.vhd", vhdf::BLOCK_SIZE*10, true);
        if( vhdf::readBlock(dsk, 11, buff ) == EOF && vhdf::writeBlock(dsk, 11, buff) == EOF ) ASSERT_TRUE
        vhdf::closeDisk(dsk);
        remove("testdisk.vhd");
    }

    std::cout << "Test 8: Persistency... ";
    {
        /**
         * Open disk, write some data to it and then close it. Open it again and
         * verify that the data is still there.
         */
        char wbuff[vhdf::BLOCK_SIZE], rbuff[vhdf::BLOCK_SIZE];
        dsk = vhdf::openDisk("testdisk.vhd", vhdf::BLOCK_SIZE*10, true);
        fillRandomBuffer(wbuff, vhdf::BLOCK_SIZE);
        vhdf::writeBlock(dsk, 3, wbuff);
        vhdf::closeDisk(dsk);
        dsk = vhdf::openDisk("testdisk.vhd");
        vhdf::readBlock(dsk, 3, rbuff);
        vhdf::closeDisk(dsk);
        if (memcmp(wbuff, rbuff, vhdf::BLOCK_SIZE) == 0) ASSERT_TRUE
        remove("testdisk.vhd");
    }

    std::cout << "Test 9: Random Write/Read... ";
    {
        /**
         * Write to and read from random blocks.
         */
        bool fail = false;
        dsk = vhdf::openDisk("testdisk.vhd", vhdf::BLOCK_SIZE*100, true);
        for (int i = 0; i < 100; i++) {
            char wbuff[vhdf::BLOCK_SIZE], rbuff[vhdf::BLOCK_SIZE];
            fillRandomBuffer(wbuff, vhdf::BLOCK_SIZE);
            size_t pos = rand() % 100;
            vhdf::writeBlock(dsk, pos, wbuff);
            vhdf::readBlock(dsk, pos, rbuff);
            if (memcmp(wbuff, rbuff, vhdf::BLOCK_SIZE) != 0) {fail = true; break;} 
        }
        if (!fail) ASSERT_TRUE;
        vhdf::closeDisk(dsk);
        remove("testdisk.vhd");
    }

    std::cout << "Test 10: Full Disk Persistent Write/Read... ";
    {
        /**
         * Write the entire file, then read it back.
         */
        bool fail = false;
        dsk = vhdf::openDisk("testdisk.vhd", vhdf::BLOCK_SIZE*100, true);
        
        char wbuff[100][vhdf::BLOCK_SIZE], rbuff[vhdf::BLOCK_SIZE];
        for (int i = 0; i < 100; i++) {
            fillRandomBuffer(wbuff[i], vhdf::BLOCK_SIZE);
            vhdf::writeBlock(dsk, i, wbuff[i]);
        }
        vhdf::closeDisk(dsk);

        dsk = vhdf::openDisk("testdisk.vhd");
        for (int i = 0; i < 100; i++) {
            vhdf::readBlock(dsk, i, rbuff);
            if (memcmp(wbuff[i], rbuff, vhdf::BLOCK_SIZE) != 0) {fail = true; break;}
        }
        vhdf::closeDisk(dsk);

        if (!fail) ASSERT_TRUE
        remove("testdisk.vhd");
        
    }


    std::cout << std::endl << "Finished. Passed " << passed << "/" << passed+failed << " tests." << std::endl;
    
    if (failed > 0) return 1;
    else return 0;
}
