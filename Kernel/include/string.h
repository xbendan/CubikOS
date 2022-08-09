typedef char* string_t;

#include <macros.h>

void strcpy(char* dest, const char* src);
void strncpy(char* dest, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);

char* strtok_r(char* str, const char* delim, char** saveptr);

size_t strlen(const char* str);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);

int strncmp(const char* s1, const char* s2, size_t n);

char* strupr(char* s);
char* strnupr(char* s, size_t n);

char* strchr(const char* s, int c);
char* strnchr(const char* s, int c, size_t n);
char* strrchr(const char* s, int c);

char* strdup(const char* s);