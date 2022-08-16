typedef enum FileType
{

} file_type_t;

typedef struct File
{
    char *name;
    uint8_t[16] contentId;
    struct FileType fileType;
    uint64_t size;
    uint64_t bytesTaken;
} file_t;