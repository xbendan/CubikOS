#include <macros.h>

typedef enum FileType
{
    FileTypeFile,
    FileTypeDirectory,
    FileTypeDisk
} file_type_t;

struct FsVolumn
{

};

struct FsPartition 
{

};

struct FsNode {};

struct FsFile
{
    char *m_Name;
    uint8_t m_UUID[16];
    enum FileType m_Type;
    uint64_t m_Size;
    uint64_t m_BytesTaken;
};

void Fs_Mount();