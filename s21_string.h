#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdarg.h>

typedef unsigned long s21_size_t;

#define S21_NULL ((void *)0)

typedef struct {
    int suppress;
    int width;
    int precision;
    char length;
    char specifier;
} FormatSpecifier;

void *s21_memchr(const void *str, int c, s21_size_t n);
int sscanf(const char* str, const char* format, ...);
int s21_isdigit(const char* symbol);
int s21_atoi(char** ptr_str);
void parse_format(char* ptr_format, FormatSpecifier* token);

#endif