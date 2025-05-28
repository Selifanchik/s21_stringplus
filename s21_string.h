#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdarg.h>
#include <stdlib.h>

typedef unsigned long s21_size_t;
#define S21_NULL ((void *)0)

typedef struct {
    int suppress;
    int width;
    char length;
    char specifier;
} FormatSpecifier;

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_sscanf(const char* str, const char* format, ...);


int s21_isdigit(const char* symbol);
int s21_is_hex_digit(const char* symbol);
long get_number(const char** ptr_str);
int parse_specifier(const char* ptr_format, FormatSpecifier* token);
double s21_atof(char** ptr_str);
long oct_to_dec(const char** oct_num);
long hex_to_dec(const char** hex_num);
int s21_isspace(int symbol);
int parse_str_sep(const char** ptr_str, const char* ptr_separation);
char* parse_format_sep(const char* start_format, const char* ptr_specifier);
void parse_value(const char** ptr_str, FormatSpecifier* token, va_list* args);
void handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args);
long s21_strtol(const char** ptr_str);
#endif