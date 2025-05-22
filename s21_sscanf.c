#include "s21_string.h"
#include <stdio.h>
#include <string.h>

int s21_sscanf(const char* str, const char* format, ...) {
    va_list arg;
    FormatSpecifier token = {};
    return 0;
}

int parse_format(char* ptr_format, FormatSpecifier* token) {
    const char specifiers[] = "cdieEfgGosuxXpn%";
    const char lengths[] = "lLh";
    int error = 0;
    if (*ptr_format == '*')  {
        token->suppress = 1;
        ptr_format++;
    }
    if (s21_isdigit(ptr_format)) {
        token->width = get_number(&ptr_format);
    }
    // заменить strchr на s21_strchr
    char* ptr_length = strchr(lengths, *ptr_format);
    if (ptr_length != S21_NULL) {
        token->length = *ptr_format;
        ptr_format++;
    }
    // заменить strchr на s21_strchr
    char* ptr_specifier = strchr(specifiers, *ptr_format);
    if (ptr_specifier != S21_NULL) {
        token->specifier = *ptr_specifier;
        ptr_format++;
    } else {
        error = 1;
    }
    return error;
}

int s21_isdigit(const char* symbol) {
    return (*symbol >= '0' && *symbol <= '9');
}

int get_number(char** ptr_str) {
    int result = 0;
    while (s21_isdigit(*ptr_str)) {
        result = result * 10 + (**ptr_str - '0');
        (*ptr_str)++;
    }
    return result;
}

int s21_atoi(char** ptr_str) {
    int result = 0;
    int sign = 1;
    while (**ptr_str == ' ') (*ptr_str)++;
    if (**ptr_str == '-') {
        sign = -1;
    }
    if (**ptr_str == '-' || **ptr_str == '+') (*ptr_str)++;
    result = sign * get_number(ptr_str);
    return result;
}

double s21_atof(char** ptr_str) {
    double result = 0.0;
    double number = 0.0;
    double fraction = 1.0;
    double sign = 1.0;
    while (**ptr_str == ' ') (*ptr_str)++;
    if (**ptr_str == '-') {
        sign = -1.0;
    }
    if (**ptr_str == '-' || **ptr_str == '+') (*ptr_str)++;
    while (s21_isdigit(*ptr_str)) {
        number = number * 10.0 + (**ptr_str - '0');
        (*ptr_str)++;
    }
    if (**ptr_str == '.') {
        (*ptr_str)++;
        while (s21_isdigit(*ptr_str)) {
            number = number * 10.0 + (**ptr_str - '0');
            fraction *= 10.0;
            (*ptr_str)++;
        }
        result = sign * (number / fraction);
    }
    return result;
}