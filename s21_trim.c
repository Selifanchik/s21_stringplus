#include "s21_string.h"
#include <string.h>

const char *ltrim(const char *src, const char *trim_chars) {
    while (*src && strchr(trim_chars, *src)) {
        src++;
    }
    return src;
}

const char *rtrim(const char *src, const char *trim_chars) {
    const char *end = src + strlen(src) - 1;
    while (end >= src && strchr(trim_chars, *end)) {
        end--;
    }
    return end;
}

void *trim(const char *src, const char *trim_chars) {
    if (!src || !trim_chars) return NULL;

    const char *start = ltrim(src, trim_chars);
    char *result;
    if (*start == '\0') {
        result = malloc(1);
        if (result) result[0] = '\0';
    } else {

        const char *end = rtrim(start, trim_chars);
        size_t len = end - start + 1;
        result = malloc(len + 1);
        if (!result) return NULL;

        strncpy(result, start, len);
        result[len] = '\0';
    }
    return result;
}
