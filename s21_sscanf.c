#include "s21_string.h"
#include <stdio.h>
#include <string.h>

int s21_sscanf(const char* str, const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    FormatSpecifier token = {};
    token.width = -1;
    int flag_end = 0;
    const char* ptr_str = str;
    const char* ptr_format = format;
    const char* ptr_specifier = ptr_format;
    char* separation = S21_NULL;
    int count = 0;
    while ((ptr_specifier = strchr(ptr_format, '%')) != S21_NULL && !flag_end) {
        separation = parse_format_sep(ptr_format, ptr_specifier);
        ptr_specifier++;
        if (!parse_str_sep(&ptr_str, separation) && !parse_specifier(&ptr_specifier, &token)) {
            parse_value(&ptr_str, &token, &arg);
            ptr_format = ptr_specifier;
        } else flag_end = 1;
        free(separation);
    }
    va_end(arg);
    return 0;
}

void parse_value(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    Callback cb = {};
    switch (token->specifier) {
        case 'd':
            cb.base = 10;
            cb.is_digit = s21_is_dec_digit;
            cb.to_digit = to_oct_dec;
            handler_int(ptr_str, token, args, cb);
            break;
        case 'i':
            if (**ptr_str == '0' && *(*ptr_str + 1) == 'x') {
                (*ptr_str) += 2;
                cb.base = 16;
                cb.is_digit = s21_is_hex_digit;
                cb.to_digit = to_hex;
                handler_unsigned_int(ptr_str, token, args, cb);
            } else if (**ptr_str == '0') {
                (*ptr_str)++;
                cb.base = 8;
                cb.is_digit = s21_is_oct_digit;
                cb.to_digit = to_oct_dec;
                handler_unsigned_int(ptr_str, token, args, cb);
            } else {
                cb.base = 10;
                cb.is_digit = s21_is_dec_digit;
                cb.to_digit = to_oct_dec;
                token->specifier = 'd';
                handler_int(ptr_str, token, args, cb);
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
            (*ptr_str)++;
            cb.base = 8;
            cb.is_digit = s21_is_oct_digit;
            cb.to_digit = to_oct_dec;
            handler_unsigned_int(ptr_str, token, args, cb);
            break;
        case 'c':
            break;
        case 's':
            break;
        case 'u':
            cb.base = 10;
            cb.is_digit = s21_is_dec_digit;
            cb.to_digit = to_oct_dec;
            handler_int(ptr_str, token, args, cb);
            break;
        case 'x':
        case 'X':
            (*ptr_str) += 2;
            cb.base = 16;
            cb.is_digit = s21_is_hex_digit;
            cb.to_digit = to_hex;
            handler_unsigned_int(ptr_str, token, args, cb);
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
    char my_space[] = {9, 10, 11, 12, 13, 32, 0};
    separation = trim(separation, my_space);
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
    while (**ptr_str && s21_isspace(**ptr_str)) (*ptr_str)++;
    return flag_end;
}

int parse_specifier(const char** ptr_format, FormatSpecifier* token) {
    const char specifiers[] = "cdieEfgGosuxXpn%";
    const char lengths[] = "lLh";
    int error = 0;
    if (**ptr_format == '*')  {
        token->suppress = 1;
        (*ptr_format)++;
    }
    if (s21_is_dec_digit(*ptr_format)) {
        Callback cb = {s21_is_dec_digit, to_oct_dec, 10};
        token->width = (int)base_to_dec(ptr_format, cb, -1);
    }
    // заменить strchr на s21_strchr
    char* ptr_length = strchr(lengths, **ptr_format);
    if (ptr_length != S21_NULL) {
        token->length = **ptr_format;
        (*ptr_format)++;
    }
    // заменить strchr на s21_strchr
    char* ptr_specifier = strchr(specifiers, **ptr_format);
    if (ptr_specifier != S21_NULL) {
        token->specifier = *ptr_specifier;
        (*ptr_format)++;
    } else {
        error = 1;
    }
    return error;
}

void handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args, Callback cb) {
    int sign = 1;
    sign = is_sign(ptr_str, &(token->width));
    long value = sign * base_to_dec(ptr_str, cb, token->width);
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


void handler_unsigned_int(const char** ptr_str, FormatSpecifier* token, va_list* args, Callback cb) {
    long value = base_to_dec(ptr_str, cb, token->width);
    if (token->length == 'l') {
        unsigned long* dest = va_arg(*args,unsigned long*);
        *dest = (unsigned long)value;
    } else if (token->length == 'h') {
        unsigned short* dest = va_arg(*args,unsigned short*);
        *dest = (unsigned short)value;
    } else {
        unsigned int* dest = va_arg(*args, unsigned int*);
        *dest = (unsigned int)value;
    }
}
