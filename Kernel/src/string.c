#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void strcpy(char* dest, const char* src) {
    while (*src) {
        *(dest++) = *(src++);
    }
    *dest = 0;
}

void strncpy(char* dest, const char* src, size_t n) {
    while (n-- && *src) {
        *(dest++) = *(src++);
    }
    *dest = 0;
}

size_t strlen(const char* str) {
    size_t i = 0;
    while (str[i])
        i++;
    return i;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 == *s2) {
        if (!*(s1++)) {
            return 0; // Null terminator
        }

        s2++;
    }
    return (*s1) - *(s2);
}

int strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i]) {
            return s1[i] < s2[i] ? -1 : 1;
        } else if (s1[i] == '\0') {
            return 0;
        }
    }

    return 0;
}

// strchr - Get pointer to first occurance of c in string s
char* strchr(const char* s, int c) {
    while (*s != (char)c)
        if (!*s++)
            return nullptr;
    return (char*)s;
}

// strnchr - Get pointer to first occurance of c in string s, searching at most n characters
char* strnchr(const char* s, int c, size_t n) {
    while (n-- && *s != (char)c)
        if (!*s++)
            return nullptr;

    if (n <= 0) {
        return nullptr;
    }

    return (char*)(s);
}

// strrchr - Get pointer to last occurance of c in string s
char* strrchr(const char* s, int c) {
    const char* occ = nullptr;
    while (*s)
        if (*s++ == c)
            occ = s;
    return (char*)(occ);
}

// strspn - Get initial length of s1 including only the characters of s2
size_t strspn(const char* s1, const char* s2) {
    size_t ret = 0;
    while (*s1 && strchr(s2, *s1++))
        ret++;
    return ret;
}

// strspn - Get initial length of s1 excluding the characters of s2
size_t strcspn(const char* s1, const char* s2) {
    size_t ret = 0;
    while (*s1)
        if (strchr(s2, *s1))
            return ret;
        else
            s1++, ret++;
    return ret;
}

// strtok - reentrant strtok
char* strtok_r(char* str, const char* delim, char** saveptr) {
    char* p = *saveptr;
    if (str)
        p = str;
    else if (!p)
        return 0;
    str = p + strspn(p, delim);
    p = str + strcspn(str, delim);
    if (p == str)
        return p = 0;
    p = *p ? * p = 0, p + 1 : 0;
    return str;
}

char* strcat(char* dest, const char* src) {
    strcpy(dest + strlen(dest), src);
    return dest;
}

char* strncat(char* dest, const char* src, size_t n) {
    strncpy(dest + strlen(dest), src, n);
    return dest;
}

char touppercase(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 'A';
    else
        return c;
}

char* strupr(char* s) {
    for (size_t i = 0; s[i] != '\0'; i++) {
        s[i] = touppercase(s[i]);
    }
    return s;
}

char* strnupr(char* s, size_t n) {

    for (size_t i = 0; s[i] && i < n; i++) {
        if (!(i < n))
            break;
        s[i] = touppercase(s[i]);
    }

    return s;
}

/*
char* strdup(const char* s) {
    char* buf = new char[strlen(s) + 1];

    strcpy(buf, s);

    return buf;
}
*/