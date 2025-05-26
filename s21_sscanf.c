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
    switch (token->specifier) {
        case 'd':
            break;
        case 'c':
            break;
        case 'i':
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

int parse_specifier(char* ptr_format, FormatSpecifier* token) {
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

int s21_is_hex_digit(const char* symbol) {
    return ((*symbol >= '0' && *symbol <= '9') || (*symbol >= 'A' && *symbol <= 'F') || (*symbol >= '0' && *symbol <= 'f'));
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
    if (**ptr_str == '-' || **ptr_str == '+') {
        if (**ptr_str == '-') {
            sign = -1;
        }
        (*ptr_str)++;
    }
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

// надо подумать может тоже подавать строку, но может и так оставить, 
// тогда можно объединить две функции добавив аргумент base
int oct_to_dec(int oct_num) {
    int dec_num = 0;
    int base = 8;
    while (oct_num > 0) {
        dec_num = dec_num * base + (oct_num % 10);
        oct_num /= 10;
    }
    return dec_num;
}
// изменить условие до конца строки, будем подавать уже корректную строку
int hex_to_dec(const char* hex_num) {
    int dec_num = 0;
    int digit = 0;
    int base = 16;
    while (s21_is_hex_digit(hex_num)) {
        if (s21_isdigit(hex_num)) {
            digit = *hex_num - '0';
        } else if (*hex_num >= 'A' && *hex_num <= 'F') {
            digit = *hex_num - 'A' + 10;
        } else {
            digit = *hex_num - 'a' + 10;
        }
        dec_num = dec_num * base + digit;
        hex_num++;
    }
    return dec_num;
}

int s21_isspace(int symbol) {
    int space_arr[] = {9, 10, 11, 12, 13, 32};
    int result = 0;
    for (int i = 0; i < sizeof(space_arr)/sizeof(*space_arr); i++) {
        if (symbol == space_arr[i]) result = 1;
    }
    return result;
}

