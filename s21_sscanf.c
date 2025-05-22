#include "s21_string.h"
#include <stdio.h>

int s21_sscanf(const char* str, const char* format, ...) {
    va_list arg;
    FormatSpecifier token = {};
    return 0;
}

void parse_format(char* ptr_format, FormatSpecifier* token) {
    if (*ptr_format == '*')  {
        token->suppress = 1;
        ptr_format++;
    }
    // подумать как отдать отдельной функции чтобы сразу несколько чисел распарсить и превратить в число
    if (s21_isdigit(ptr_format)) {
        token->width = s21_atoi(&ptr_format);
    }
    // запихнуть l, h, L в массив или структуру
    if (*ptr_format == 'l' || *ptr_format == 'h' || *ptr_format == 'L') {
        token->length = *ptr_format;
        ptr_format++;
    }
    // запихнуть спецификаторы в структуру или массив чтоб не писать каждый раз
    if (*ptr_format == 'd') {
        token->specifier = *ptr_format;
        ptr_format++;
    } else {
        printf("%s\n", ptr_format);
        printf("unknown format\n");
    }
}

int s21_isdigit(const char* symbol) {
    return (*symbol >= '0' && *symbol <= '9');
}

int s21_atoi(char** ptr_str) {
    int number = 0;
    while (s21_isdigit(*ptr_str)) {
        number *= 10;
        number += (**ptr_str - '0');
        (*ptr_str)++;
    }
    return number;
}