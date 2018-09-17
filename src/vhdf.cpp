#include <vhdf.hpp>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <map>

namespace vhdf
{

    struct VHD
    {
        const char* name;
        size_t size;            // The size of the disk in bytes
        FILE* file;             // The file pointer to the disk file
    };

    std::vector<VHD*> disks = std::vector<VHD*>();
    std::map<std::string, int> disks_open = std::map<std::string, int>();

    int openDisk(const char* filename, size_t nbytes, bool nosparse) {
        
        if(disks_open.find(filename) != disks_open.end()) return disks_open[filename];

        FILE* disk;
        if((disk = fopen(filename, "r+b")) == NULL) disk = fopen(filename, "w+b");
        if (disk == NULL) return -1;
        
        VHD* vhd = new VHD();
        vhd->name = filename;
        
        fseek(disk, 0, SEEK_END);
        long int size;
        if ((size = ftell(disk)) > 0) {
            vhd->size = size;
        } 
        else if (size == 0) {
            size_t blocks = (size_t)(std::floor((double)nbytes/BLOCK_SIZE));
            size = blocks*BLOCK_SIZE;
            if (nosparse) {
                char buff[BLOCK_SIZE];
                memset(buff, 0, BLOCK_SIZE);
                for (size_t i = 0; i < blocks ; i++) fwrite(buff, BLOCK_SIZE, 1, disk);
            } else {
                fseek(disk, size-1, SEEK_SET);
                fputc('\0', disk);
            }
            vhd->size = size;
        }
        else {
            delete vhd;
            fclose(disk);
            return -1;
        }

        vhd->file = disk;

        disks.push_back(vhd);

        return disks.size()-1;
    }

    int closeDisk(int disk) {
        VHD* vhd = disks[disk];
        disks.erase(disks.begin() + disk);
        disks_open.erase(vhd->name);
        if (fclose(vhd->file) != EOF) {
            delete vhd;
            return 0;
        }
        else return -1;
    }

    int writeBlock(int disk, size_t blocknr, void* buff) {
        VHD* dsk = disks[disk];
        size_t loc = blocknr*BLOCK_SIZE;
        if (loc >= dsk->size) return EOF;
        
        fseek(dsk->file, loc, SEEK_SET);
        fwrite(buff, BLOCK_SIZE, 1, dsk->file);
        return 0;
    }

    int readBlock(int disk, size_t blocknr, void* buff) {
        VHD* dsk = disks[disk];
        size_t loc = blocknr*BLOCK_SIZE;
        if (loc >= dsk->size) return EOF;
        
        fseek(dsk->file, loc, SEEK_SET);
        fread(buff, BLOCK_SIZE, 1, dsk->file);

        return 0;
    }

    void syncDisk() {};
}