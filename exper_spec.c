#include "s21_helpers.h"
#include "s21_string.h"
#include <stdio.h>

// typedef struct {
//     int length;
//     char spec[16];
// } LenSpec;

int my_parse_specifier(const char** ptr_format, FormatSpecifier* token) {
    LenSpec array[4];
    array[0].length = 0;
    s21_strncpy(array[0].spec, "csdiouxXceEgGfp", 16);
    array[1].length = 'h';
    s21_strncpy(array[1].spec, "diouxX", 7);
    array[2].length = 'l';
    s21_strncpy(array[2].spec, "csdiouxXceEgGf", 15);
    array[3].length = 'L';
    s21_strncpy(array[3].spec, "eEgGf", 6);
    const char specifiers[] = "cdieEfgGosuxXpn%";
    const char lengths[] = "lLh";
    int flag_error = 0;
    if (**ptr_format == '*')  {
        token->suppress = 1;
        (*ptr_format)++;
    }
    if (s21_is_dec_digit(*ptr_format)) {
        Callback cb = {s21_is_dec_digit, to_oct_dec, 10};
        int width = -1;
        unsigned long temp_width = 0;
        if (!base_to_dec(ptr_format, &cb, &width, &temp_width)) {
            token->width = (int)temp_width;
        }
    }
    if (!token->suppress) {
        printf("%s\n", *ptr_format);
        const char* ptr_length = s21_strchr(lengths, **ptr_format);
        if (ptr_length != S21_NULL) {
            token->length = **ptr_format;
            (*ptr_format)++;
        }
    }
    if (!token->suppress && !token->length && token->width == -1) {
        const char* ptr_specifier = s21_strchr(specifiers, **ptr_format);
        if (ptr_specifier != S21_NULL) {
            token->specifier = *ptr_specifier;
            (*ptr_format)++;
        }
    } else {
        int flag_end = 0;
        for (int i = 0; i < 4 && !flag_end; i++) {
            if (token->length == array[i].length) {
                printf("lentok=%d\n", token->length);
                printf("lenarr=%d\n", array[i].length);
                const char* ptr_specifier = s21_strchr(array[i].spec, **ptr_format);
                if (ptr_specifier != S21_NULL) {
                    token->specifier = *ptr_specifier;
                    (*ptr_format)++;
                    flag_end = 1;
                } else flag_error = 1;
            }
        }
    }
    return flag_error;
}

int main() {
    FormatSpecifier token;
    init_token(&token);
    const char* str1 = "5d";
    my_parse_specifier(&str1, &token);
    printf("sup=%d, width=%d, len=%d, sec=%c\n", token.suppress, token.width, token.length, token.specifier);
}