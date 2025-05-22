#include "s21_string.h"
#include <stdio.h>

int s21_sscanf(const char* str, const char* format, ...) {
    va_list arg;
    FormatSpecifier token = {};
    return 0;
}

int parse_format(char* ptr_format, FormatSpecifier* token) {
    const char id_spec[] = "cdieEfgGosuxXpn%";
    int error = 0;
    if (*ptr_format == '*')  {
        token->suppress = 1;
        ptr_format++;
    }
    if (s21_isdigit(ptr_format)) {
        token->width = get_number(&ptr_format);
    }
    // можно заменить потом на функцию поиска символа strchr
    if (*ptr_format == 'l' || *ptr_format == 'h' || *ptr_format == 'L') {
        token->length = *ptr_format;
        ptr_format++;
    }
    //  можно заменить потом на функцию поиска символа strchr
    if (*ptr_format == 'd') {
        token->specifier = *ptr_format;
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