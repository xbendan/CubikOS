namespace FileSystem
{
    typedef enum VolumeType
    {
        LocalDisk,
        Network
    } volume_type_t;

    typedef enum FileSystemType
    {
        FileSystemExFAT,
        FileSystemFAT32,
        FileSystemNTFS,
        FileSystemISO9660,
        FileSystem
    } fs_type_t;

    typedef struct Volume
    {
        char letter;
        char name[256];
        uint64_t capacity;
        uint64_t spaceUsed;
        uint64_t spaceFree;
        bool mount;
        volume_type_t volumeType;
        fs_type_t fsType;
    } volume_t;
    
}