#include "s21_string.h"
#include <stdio.h>
#include <string.h>

int s21_sscanf(const char* str, const char* format, ...) {
    va_list arg;
    FormatSpecifier token = {};
    const char* ptr_str = str;
    const char* ptr_format = format;
    const char* ptr_specifier = strchr(ptr_format, '%');
    if (ptr_specifier) {
        char* separation = parse_format_sep(ptr_format, ptr_specifier);
        if (!parse_str_sep(&ptr_str, separation) && !parse_specifier(++ptr_specifier, &token)) {

        }
        // отдаем парсить спецификатор от которого нам нужна структура
        // parse_specifier(++ptr_specifier, token);
        // а потом парсим строку по спецификатору
        free(separation);
        printf("separation = %s\n", ptr_str);
    }
    return 0;
}


void parse_value(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    char tmp[12] = "-+123456789";
    switch (token->specifier) {
        case 'd':
            handler_int(ptr_str, token, args);
            break;
        case 'c':
            break;
        case 'i':
            if (strchr(tmp, **ptr_str)) {
                handler_int(ptr_str, token, args);
            }
            break;
        case 'e':
            break;
        case 'E':
            break;
        case 'f':
            break;
        case 'g':
            break;
        case 'G':
            break;
        case 'o':
            break;
        case 's':
            break;
        case 'u':
            break;
        case 'x':
        case 'X':
            break;
        case 'p':
            break;
        case 'n':
            break;
        case '%':
            break;
        default:
            break;
    }
    return;
}

char* parse_format_sep(const char* start_format, const char* ptr_specifier) {
    s21_size_t length_sep = ptr_specifier - start_format;
    char* separation = (char*)calloc(length_sep + 1, sizeof(char));
    // заменить на s21_strncpy
    strncpy(separation, start_format, length_sep);
    return separation;
}

int parse_str_sep(const char** ptr_str, const char* ptr_separation) {
    int flag_end = 0;
    while (**ptr_str && s21_isspace(**ptr_str)) (*ptr_str)++;
    for (; *ptr_separation && !flag_end; ) {
        if (**ptr_str != *ptr_separation) flag_end = 1;
        else {
            (*ptr_str)++;
            ptr_separation++;
        }
    }
    return flag_end;
}

int parse_specifier(const char* ptr_format, FormatSpecifier* token) {
    const char specifiers[] = "cdieEfgGosuxXpn%";
    const char lengths[] = "lLh";
    int error = 0;
    if (*ptr_format == '*')  {
        token->suppress = 1;
        ptr_format++;
    }
    if (s21_is_dec_digit(ptr_format)) {
        token->width = (int)get_number(&ptr_format);
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



void handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    Callback cb = {s21_is_dec_digit, to_oct_dec};
    long value = base_to_dec(ptr_str, 10, cb, token->width);
    if (token->length == 'l') {
        long* dest = va_arg(*args, long*);
        *dest = (long)value;
    } else if (token->length == 'h') {
        short* dest = va_arg(*args, short*);
        *dest = (short)value;
    } else {
        int* dest = va_arg(*args, int*);
        *dest = (int)value;
    }
}

long s21_strtol(const char** ptr_str) { //добавить ширину и возможно систему счисления (для целых чисел)
    long result = 0;
    long sign = 1;
    while (**ptr_str == ' ') (*ptr_str)++;
    if (**ptr_str == '-' || **ptr_str == '+') {
        if (**ptr_str == '-') {
            sign = -1;
        }
        (*ptr_str)++;
    }
    result = sign * get_number(ptr_str);
    return result;

}
