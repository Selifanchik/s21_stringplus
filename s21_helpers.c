#include "s21_string.h"
#include <stdio.h>

int s21_is_dec_digit(const char* symbol) {
    return (*symbol >= '0' && *symbol <= '9');
}

int s21_is_hex_digit(const char* symbol) {
    return ((*symbol >= '0' && *symbol <= '9') || (*symbol >= 'A' && *symbol <= 'F') || (*symbol >= 'a' && *symbol <= 'f'));
}

int s21_is_oct_digit(const char* symbol) {
    return (*symbol >= '0' && *symbol <= '7');
}

long get_number(const char** ptr_str) {
    long result = 0;
    while (s21_is_dec_digit(*ptr_str)) {
        result = result * 10 + (**ptr_str - '0');
        (*ptr_str)++;
    }
    return result;
}

int s21_isspace(int symbol) {
    int space_arr[] = {9, 10, 11, 12, 13, 32};
    int result = 0;
    for (s21_size_t i = 0; i < sizeof(space_arr)/sizeof(*space_arr); i++) {
        if (symbol == space_arr[i]) result = 1;
    }
    return result;
}

// add width
// rename struct Callback
long base_to_dec(const char** num, Callback cb, int* width) {
    long dec_num = 0;
    int digit = 0;
    while (cb.is_digit(*num) && (*width == -1 || *width > 0)) {
        digit = cb.to_digit(*num);
        dec_num = dec_num * cb.base + digit;
        if (*width > 0) (*width)--;
        (*num)++;
    }
    return dec_num;
}

int to_oct_dec(const char* num) {
    return (*num - '0');
}

int to_hex(const char* hex_num) {
    int digit = 0;
    if (s21_is_dec_digit(hex_num)) {
        digit = *hex_num - '0';
    } else if (*hex_num >= 'A' && *hex_num <= 'F') {
        digit = *hex_num - 'A' + 10;
    } else {
        digit = *hex_num - 'a' + 10;
    }
    return digit;
}

int is_sign(const char** ptr_str, int* width) {
    int sign = 1;
    if ((**ptr_str == '-' || **ptr_str == '+') && (*width == -1 || *width > 1)) {
        if (**ptr_str == '-') {
            sign = -1;
        }
        (*ptr_str)++;
        if (*width > 1) ((*width)--);
    }
    return sign;
}

void init_token(FormatSpecifier* token) {
    token->length = 0;
    token->specifier = 0;
    token->suppress = 0;
    token->width = -1;
}

void skip_space(const char** ptr_str) {
    while (**ptr_str && s21_isspace(**ptr_str)) (*ptr_str)++;
}

void init_parse_float(ParseFloat* number) {
    number->sign_float = 1;
    number->int_part = 0;
    number->fract_part = 0;
    number->order_fract = 0;
    number->exp_part = 0;
    number->sign_exp = 1;
    number->order_exp = 0;
}

long double s21_pow10(int order) {
    long double result = 1.0;
    for (int i = 0; i < order; i++) {
        result *= 10.;
    }
    return result;
}

long double to_float(ParseFloat float_value) {
    long double value = 0;
    long double fraction = 1.0;
    for (int i = 0; i < float_value.order_fract; i++) {
        fraction = s21_pow10(float_value.order_fract);
    }
    value = (long double)float_value.int_part + ((long double)float_value.fract_part / fraction);
    if (float_value.exp_part) {
        long double exp = s21_pow10(float_value.order_exp);
        if (float_value.sign_exp > 0) {
            value *= exp;
        } else value /= exp;
    }
    return value;
}
