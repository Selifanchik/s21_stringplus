#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdarg.h>
#include <stdlib.h>

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
int s21_sscanf(const char* str, const char* format, ...);


int s21_isdigit(const char* symbol);
int s21_is_hex_digit(const char* symbol);
int get_number(char** ptr_str);
int parse_specifier(char* ptr_format, FormatSpecifier* token);
int s21_atoi(char** ptr_str);
double s21_atof(char** ptr_str);
int oct_to_dec(int num);
int hex_to_dec(const char* hex_num);
int s21_isspace(int symbol);
void parse_format(const char* ptr_format, FormatSpecifier* token);
int parse_str(const char** ptr_str, const char* ptr_separation);
#endif