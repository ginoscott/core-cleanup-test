#include "device/disk/devicedisk.h"

DeviceDisk::DeviceDisk()
{

}

// get the disk size percentage
double DeviceDisk::getAll()
{
#if defined COSMOS_LINUX_OS
    return getAll("/");
#endif

#if defined COSMOS_WIN_OS
    return getAll("C:");
#endif
}

double DeviceDisk::getAll(string path)
{
    Size = 0;
    Free = 0;
    Used = 0;
    FreePercent = 0;

#if defined COSMOS_LINUX_OS
    struct statvfs buf;

    statvfs(path.c_str(),&buf);

    // disk size in bytes
    // must add the casting otherwise there will be problems
    Size = (uint64_t)buf.f_frsize * (uint64_t)buf.f_blocks;
    Free = (uint64_t)buf.f_frsize * (uint64_t)buf.f_bfree;

#endif

#if defined COSMOS_MAC_OS
    // Open disk
     uint32_t dev = open("/dev/disk0", O_RDONLY);

     if (dev == -1) {
         perror("Failed to open disk");
         return -1;
     }

     uint64_t sector_count = 0;
     // Query the number of sectors on the disk
     ioctl(dev, DKIOCGETBLOCKCOUNT, &sector_count);

     uint32_t sector_size = 0;
     // Query the size of each sector
     ioctl(dev, DKIOCGETBLOCKSIZE, &sector_size);

     uint64_t disk_size = sector_count * sector_size;
     printf("%ld", disk_size);

     Size = disk_size;

     return 0;
#endif

#if defined COSMOS_WIN_OS
    uint64_t freeSpace;

    GetDiskFreeSpaceExA( (LPCSTR) path.c_str(),
                        (PULARGE_INTEGER)&freeSpace,
                        (PULARGE_INTEGER)&Size,
                        (PULARGE_INTEGER)&Free);

#endif

    Used = Size - Free;
    if (Size != 0.)
    {
        FreePercent = (double)Free / Size;
    }
    else {
        FreePercent = 0.;
    }

    // convert to GiB
    SizeGiB = Size/1073741824.;
    FreeGiB = Free/1073741824.;
    UsedGiB = Used/1073741824.;

    return FreePercent;
}

uint64_t DeviceDisk::getSize()
{
#if defined COSMOS_LINUX_OS
    return getSize("/");
#endif

#if defined COSMOS_WIN_OS
    return getSize("C:");
#endif
}

// get all disk information in bytes
uint64_t DeviceDisk::getSize(const string path)
{
    getAll(path);
    return Size;
}


double DeviceDisk::getSizeGiB(const string path)
{
    // convert from Byte to GiB
    double size = getSize(path)/1073741824.;
    return size;
}

double DeviceDisk::getSizeGiB()
{
    // convert from Byte to GiB
    double size = getSize()/1073741824.;
    return size;
}

uint64_t DeviceDisk::getUsed()
{
#if defined COSMOS_LINUX_OS
    return getUsed("/");
#endif

#if defined COSMOS_WIN_OS
    return getUsed("C:");
#endif
}

uint64_t DeviceDisk::getUsed(const string path)
{
    getAll(path);
    return (Used);
}


double DeviceDisk::getUsedGiB(const string path)
{
    // convert from Byte to GiB
    return (double)getUsed(path)/1073741824.;
}

double DeviceDisk::getUsedGiB()
{
    // convert from Byte to GiB
    return (double)getUsed()/1073741824.;
}

// get the free disk in bytes
uint64_t DeviceDisk::getFree()
{
#if defined COSMOS_LINUX_OS
    return getFree("/");
#endif

#if defined COSMOS_WIN_OS
    return getFree("C:");
#endif
}


uint64_t DeviceDisk::getFree(string path)
{
    getAll(path);

    return Free;
}


double DeviceDisk::getFreeGiB(string path)
{
    // convert from Byte to GiB
    return (double)getFree(path)/1073741824.;
}

double DeviceDisk::getFreeGiB()
{
    // convert from Byte to GiB
    return (double)getFree()/1073741824.;
}

vector <DeviceDisk::info> DeviceDisk::getInfo()
{
    vector <info> result;
    info tinfo;
#ifdef COSMOS_LINUX_OS
    string tdata;
    uint64_t tsize;

    int32_t iretn = data_execute("lsblk -fbl -o SIZE,MOUNTPOINT", tdata);
    vector<string> lines;
    if(iretn<0)	{
        iretn = data_execute("df", tdata);
        if(iretn<0)	{
            return result;
        }
        lines = string_split(tdata, "\n");
        for (string line : lines)
        {
            if (line.find("/") != string::npos)
            {
                char tmount[50];
                if (sscanf(line.c_str(), "%*s %lu %*s %*s %*s %s\n", &tsize, tmount) == 2 && tmount[0] == '/')
                {
                    tinfo.mount = tmount;
                    tinfo.size = tsize * 1024;
                    tinfo.used = getUsed(tinfo.mount);
                    tinfo.free = tinfo.size - tinfo.used;
                    result.push_back(tinfo);
                }
            }
        }
    } else	{
        lines = string_split(tdata, "\n");
        for (string line : lines)
        {
            if (line.find("/") != string::npos)
            {
                char tmount[50];
                if (sscanf(line.c_str(), "%lu %s\n", &tsize, tmount) == 2 && tmount[0] == '/')
                {
                    tinfo.mount = tmount;
                    tinfo.size = tsize;
                    tinfo.used = getUsed(tinfo.mount);
                    tinfo.free = tinfo.size - tinfo.used;
                    result.push_back(tinfo);
                }
            }
        }
    }
#endif
#ifdef COSMOS_WIN_OS
    getAll();
    tinfo.size = Size;
    tinfo.free = Free;
    tinfo.used = Used;
    tinfo.mount = "c:/";
    result.push_back(tinfo);
#endif
#ifdef COSMOS_MAC_OS
    getAll();
    tinfo.size = Size;
    tinfo.mount = "/";
    result.push_back(tinfo);
#endif
    return result;
}
