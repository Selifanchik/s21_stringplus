#include "s21_helpers.h"
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

int s21_isspace(int symbol) {
    int space_arr[] = {9, 10, 11, 12, 13, 32};
    int result = 0;
    for (s21_size_t i = 0; i < sizeof(space_arr)/sizeof(*space_arr); i++) {
        if (symbol == space_arr[i]) result = 1;
    }
    return result;
}

// rename struct Callback
int base_to_dec(const char** ptr_str, const Callback* cb, int* width, unsigned long* value) {
    int flag_parse_error = 1;
    while (cb->is_digit(*ptr_str) && (*width == -1 || *width > 0)) {
        int digit = cb->to_digit(*ptr_str);
        *value = *value * cb->base + digit;
        if (*width > 0) (*width)--;
        (*ptr_str)++;
        flag_parse_error = 0;
    }
    return flag_parse_error;
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
    if ((**ptr_str == '-' || **ptr_str == '+') && (*width == -1 || *width > 0)) {
        if (**ptr_str == '-') {
            sign = -1;
        }
        (*ptr_str)++;
        if (*width > 0) ((*width)--);
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
    number->s21_nan = 0;
    number->s21_inf = 0;
}

long double s21_pow10(int order) {
    long double result = 1.0;
    for (int i = 0; i < order; i++) {
        result *= 10.;
    }
    return result;
}

long double to_float(ParseFloat float_value) {
    long double value = 0.0;
    if (float_value.s21_nan || float_value.s21_inf) {
        to_nan_inf(&value, float_value);
    } else {
        long double fraction = 1.0;
        for (int i = 0; i < float_value.order_fract; i++) {
            fraction = s21_pow10(float_value.order_fract);
        }
        value = (long double)((float_value.int_part + ((long double)float_value.fract_part / fraction)) * float_value.sign_float);
        if (float_value.exp_part) {
            long double exp = s21_pow10(float_value.order_exp);
            if (float_value.sign_exp > 0) {
                value *= exp;
            } else value /= exp;
        }
    }
    return value;
}

int is_valid_exponent(const char *ptr_str, int width) {
    int flag_error = 1;
    int is_exp = (*ptr_str == 'E' || *ptr_str == 'e');
    int has_sign_and_digit = (*(ptr_str + 1) == '+' || *(ptr_str + 1) == '-') && s21_is_dec_digit(ptr_str + 2) && (width > 1 || width == -1);
    int has_digit_only = s21_is_dec_digit(ptr_str + 1) && (width > 0 || width == -1);
    if (is_exp && (has_sign_and_digit || has_digit_only)) flag_error = 0;
    return flag_error;
}

int is_write_specifier(FormatSpecifier* token) {
    return !token->suppress && token->specifier != '%';
}

int parse_i(const char** ptr_str, Callback* cb, int* width) {
    int flag_error = 0;
    if (**ptr_str == '0' && (*(*ptr_str + 1) == 'x' || *(*ptr_str + 1) == 'X')) {
        is_prefix_base16(ptr_str, width);
        to_base16(cb);
    } else if (**ptr_str == '0') {
        to_base8(cb);
    } else {
        to_base10(cb);
    }
    return flag_error;
  }

void to_base8(Callback* cb) {
    cb->is_digit = s21_is_oct_digit;
    cb->to_digit = to_oct_dec;
    cb->base = 8;
}

void to_base10(Callback* cb) {
    cb->is_digit = s21_is_dec_digit;
    cb->to_digit = to_oct_dec;
    cb->base = 10;
}


void to_base16(Callback* cb) {
    cb->is_digit = s21_is_hex_digit;
    cb->to_digit = to_hex;
    cb->base = 16;
}

void is_prefix_base16(const char** ptr_str, int* width) {
    if (**ptr_str == '0' && ((*(*ptr_str + 1)) == 'x' || (*(*ptr_str + 1)) == 'X')) {
        if (*width > 1 || *width == -1) {
            (*ptr_str) += 2;
            if (*width > 1) *width -= 2;
        }
    }
}

int s21_is_nan_inf(const char** ptr_str, int* width, ParseFloat* parse_float) {
    int flag_nan_inf = 0;
    init_parse_float(parse_float);
    if (*width == -1 || *width > 2) {
        if (s21_strncmp(*ptr_str, "nan", 3) == 0) {
            parse_float->s21_nan = 1;
            if (*width > 2) *width -= 3;
            (*ptr_str) += 3;
            flag_nan_inf = 1;
        } else if (s21_strncmp(*ptr_str, "inf", 3) == 0) {
            parse_float->s21_inf = 1;
            if (*width > 2) *width -= 3;
            flag_nan_inf = 1;
            (*ptr_str) += 3;
        }
    }
    return flag_nan_inf;
}

int parse_float(const char** ptr_str, FormatSpecifier* token, ParseFloat* float_value) {
    int flag_error = 0;
    Callback cb; 
    to_base10(&cb);
    float_value->sign_float = is_sign(ptr_str, &(token->width));
    int flag_digit = 0;
    if (s21_is_dec_digit(*ptr_str)) {
        unsigned long temp_int_part = 0;
        if (!base_to_dec(ptr_str, &cb, &(token->width), &temp_int_part)) {
            float_value->int_part = temp_int_part;
            flag_digit = 1;
        }
    }
    if (**ptr_str == '.' && (flag_digit || s21_is_dec_digit(*ptr_str + 1)) && (token->width == -1 || token->width > 0)) {
        (*ptr_str)++;
        if (token->width > 0) token->width--;
        const char* start_fract = *ptr_str;
        if (s21_is_dec_digit(*ptr_str)) {
            unsigned long temp_fract_part = 0;
            if (!base_to_dec(ptr_str, &cb, &(token->width), &temp_fract_part)) {
                float_value->fract_part = temp_fract_part;
                float_value->order_fract = *ptr_str - start_fract;
                flag_digit = 1;
            }
        }
    }
    if (flag_digit) {
        if (!is_valid_exponent(*ptr_str, token->width)) {
            float_value->exp_part = 1;
            (*ptr_str)++;
            float_value->sign_exp = is_sign(ptr_str, &(token->width));
            unsigned long temp_order_exp = 0;
            if (!base_to_dec(ptr_str, &cb, &(token->width), &temp_order_exp)) {
                float_value->order_exp = (int)temp_order_exp;
            }
        }
    } else flag_error = 1;
    return flag_error;
}

void to_nan_inf(long double* value, ParseFloat float_value) {
    if (float_value.s21_nan) {
        *value = float_value.sign_float * NAN;
    } else if (float_value.s21_inf) {
        *value = float_value.sign_float * INFINITY;
    }
}