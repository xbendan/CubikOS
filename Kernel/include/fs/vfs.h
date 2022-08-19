#include <macros.h>

typedef enum FileType
{
    FileTypeFile,
    FileTypeDirectory,
    FileTypeDisk
} file_type_t;

typedef struct File
{
    char *name;
    uint8_t contentId[16];
    enum FileType fileType;
    uint64_t size;
    uint64_t bytesTaken;
} file_t;