#include "s21_string.h"
#include <stdio.h>
#include <string.h>

int s21_sscanf(const char* str, const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    FormatSpecifier token;
    init_token(&token);
    int read_count = 0;
    int flag_end = 0;
    const char* ptr_str = str;
    const char* ptr_format = format;
    const char* ptr_specifier = ptr_format;
    char* separation = NULL;
    while ((ptr_specifier = strchr(ptr_specifier, '%')) != S21_NULL && !flag_end) {
        separation = parse_format_sep(ptr_format, ptr_specifier);
        if (*(ptr_specifier + 1) && separation) { 
            ptr_specifier++;
            if (!parse_str_sep(&ptr_str, separation) && !parse_specifier(&ptr_specifier, &token)) {
                parse_value(str, &ptr_str, &token, &arg);
                ptr_format = ptr_specifier;
                read_count++;
            } else flag_end = 1;
            free(separation);
            init_token(&token);
        }
    }
    va_end(arg);
    return read_count;
}

int parse_value(const char* str, const char** ptr_str, FormatSpecifier* token, va_list* args) {
    int flag_error = 0;
    Callback cb = {0};
    switch (token->specifier) {
        case 'd':
            skip_space(ptr_str);
            cb.base = 10;
            cb.is_digit = s21_is_dec_digit;
            cb.to_digit = to_oct_dec;
            if (handler_int(ptr_str, token, args, &cb)) flag_error = 1;;
            break;
        case 'i':
            skip_space(ptr_str);
            if (**ptr_str == '0' && (*(*ptr_str + 1) == 'x' || *(*ptr_str + 1) == 'X')) {
                (*ptr_str) += 2;
                cb.base = 16;
                cb.is_digit = s21_is_hex_digit;
                cb.to_digit = to_hex;
                if (handler_int(ptr_str, token, args, &cb)) flag_error = 1;
            } else if (**ptr_str == '0') {
                (*ptr_str)++;
                cb.base = 8;
                cb.is_digit = s21_is_oct_digit;
                cb.to_digit = to_oct_dec;
                if (handler_int(ptr_str, token, args, &cb)) flag_error = 1;
            } else {
                cb.base = 10;
                cb.is_digit = s21_is_dec_digit;
                cb.to_digit = to_oct_dec;
                if (handler_int(ptr_str, token, args, &cb)) flag_error = 1;
            }
            break;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
            skip_space(ptr_str);
            handler_fegEG(ptr_str, token, args);
            break;
        case 'o':
            skip_space(ptr_str);
            if (**ptr_str == '0') (*ptr_str)++;
            cb.base = 8;
            cb.is_digit = s21_is_oct_digit;
            cb.to_digit = to_oct_dec;
            handler_unsigned_int(ptr_str, token, args, &cb);
            break;
        case 'c':
            handler_c(ptr_str, token, args);
            break;
        case 's':
            skip_space(ptr_str);
            if (token->width > 0) handler_c(ptr_str, token, args);
            else handler_s(ptr_str, token, args);
            break;
        case 'u':
            skip_space(ptr_str);
            cb.base = 10;
            cb.is_digit = s21_is_dec_digit;
            cb.to_digit = to_oct_dec;
            handler_unsigned_int(ptr_str, token, args, &cb);
            break;
        case 'x':
        case 'X':
            skip_space(ptr_str);
            if (**ptr_str == '0' && ((*(*ptr_str + 1)) == 'x' || (*(*ptr_str + 1)) == 'X')) (*ptr_str) += 2;
            cb.base = 16;
            cb.is_digit = s21_is_hex_digit;
            cb.to_digit = to_hex;
            handler_unsigned_int(ptr_str, token, args, &cb);
            break;
        case 'p':
            skip_space(ptr_str);
            if (**ptr_str == '0' && ((*(*ptr_str + 1)) == 'x' || (*(*ptr_str + 1)) == 'X')) (*ptr_str) += 2;
            handler_p(ptr_str, token, args);
            break;
        case 'n':
            handler_n(str, *ptr_str, args);
            break;
        case '%':
            skip_space(ptr_str);
            if (**ptr_str == '%') (*ptr_str)++;
            // иначе закончить считывание
            break;
        default:
            break;
    }
    return flag_error;
}

char* parse_format_sep(const char* start_format, const char* ptr_specifier) {
    s21_size_t length_sep = ptr_specifier - start_format;
    char* separation = (char*)calloc(length_sep + 1, sizeof(char));
    if (separation){
        // заменить на s21_strncpy
        strncpy(separation, start_format, length_sep);
        char my_space[] = {9, 10, 11, 12, 13, 32, 0};
        separation = (char*)s21_trim(separation, my_space);
    }
    return separation;
}

int parse_str_sep(const char** ptr_str, const char* ptr_separation) {
    int flag_end = 0;
    skip_space(ptr_str);
    for (; *ptr_separation && !flag_end; ) {
        if (**ptr_str != *ptr_separation) flag_end = 1;
        else {
            (*ptr_str)++;
            ptr_separation++;
        }
    }
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
        int width = -1;
        unsigned long temp_width;
        if (!base_to_dec(ptr_format, &cb, &width, &temp_width)) {
            token->width = (int)temp_width;
        }
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

int handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args, const Callback* cb) {
    int flag_error = 0;
    int sign = is_sign(ptr_str, &(token->width));
    unsigned long value = 0;
    if (!base_to_dec(ptr_str, cb, &(token->width), &value)) {
        if (!token->suppress) {
            if (token->length == 'l') {
                long* dest = va_arg(*args, long*);
                *dest = (long)value * sign;
            } else if (token->length == 'h') {
                short* dest = va_arg(*args, short*);
                *dest = (short)value * sign;
            } else {
                int* dest = va_arg(*args, int*);
                *dest = (int)value * sign;
            }
        }
    } else flag_error = 1;
    return flag_error;
}


int handler_unsigned_int(const char** ptr_str, FormatSpecifier* token, va_list* args, const Callback* cb) {
    int flag_error = 0;
    unsigned long value = 0;
    if (!base_to_dec(ptr_str, cb, &(token->width), &value)) {
        if (!token->suppress) {
            if (token->length == 'l') {
                unsigned long* dest = va_arg(*args, unsigned long*);
                *dest = (unsigned long)value;
            } else if (token->length == 'h') {
                unsigned short* dest = va_arg(*args, unsigned short*);
                *dest = (unsigned short)value;
            } else {
                unsigned* dest = va_arg(*args, unsigned*);
                *dest = (unsigned)value;
            }
        }
    } else flag_error = 1;
    return flag_error;
}

void handler_n(const char* start_str, const char* ptr_str, va_list* args) {
    int value = ptr_str - start_str;
    int* dest = va_arg(*args, int*);
    *dest = (int)value;
}

void handler_c(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    if (token->width == -1) token->width = 1;
    //пока так но надо подумать как сделать с длиной
    char* dest;
    if (!token->suppress) dest = va_arg(*args, char*);
    int i = 0;
    for (i = 0; i < token->width; i++) {
        if (**ptr_str) {
            if (!token->suppress) *(dest + i) = *(*ptr_str);
            (*ptr_str)++;
        } else {
            break; //заменить на что то другое возможно что будет отслеживать ошибку
        }
    }
    if (token->width > 1 && !token->suppress) *(dest + i) = '\0';
}

void handler_s(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    char* dest;
    if (!token->suppress) dest = va_arg(*args, char*);
    int i = 0;
    while (**ptr_str && s21_isspace(**ptr_str)) {
        if(!token->suppress) dest = va_arg(*args, char*);
        (*ptr_str)++;
        i++;
    } //обработку ошибок добавить и длину
    *(dest + i) = '\0';
}

void parse_float(const char** ptr_str, FormatSpecifier* token, ParseFloat* float_value) {
    Callback cb;
    cb.is_digit = s21_is_dec_digit;
    cb.to_digit = to_oct_dec;
    cb.base = 10;
    init_parse_float(float_value);
    float_value->sign_float = is_sign(ptr_str, &(token->width));
    //char* temp = S21_NULL;
    int flag_digit = 0;
    if (s21_is_dec_digit(*ptr_str)) {
        unsigned long temp_int_part = 0;
        if (!base_to_dec(ptr_str, &cb, &(token->width), &temp_int_part)) {
            float_value->int_part = (long)temp_int_part;
            flag_digit = 1;
        }
    }
    if (**ptr_str == '.' && (flag_digit || s21_is_dec_digit(*ptr_str + 1))) {
        (*ptr_str)++;
        const char* start_fract = *ptr_str;
        if (s21_is_dec_digit(*ptr_str)) {
            unsigned long temp_fract_part = 0;
            if (!base_to_dec(ptr_str, &cb, &(token->width), &temp_fract_part)) {
                float_value->fract_part = (int)temp_fract_part;
                float_value->order_fract = *ptr_str - start_fract;
                flag_digit = 1;
            }
        }
    }
    if (flag_digit && (**ptr_str == 'E' || **ptr_str == 'e')) {
        if ((*(*ptr_str + 1) == '-' || *(*ptr_str + 1) == '+') && s21_is_dec_digit(*ptr_str + 2)) {
            float_value->exp_part = 1;
            (*ptr_str)++;
            float_value->sign_exp = is_sign(ptr_str, &(token->width));
            unsigned long temp_order_exp = 0;
            if (!base_to_dec(ptr_str, &cb, &(token->width), &temp_order_exp)) {
                float_value->order_exp = (int)temp_order_exp;
            }
        } else if (s21_is_dec_digit(*ptr_str + 1)) {
            float_value->exp_part = 1;
            (*ptr_str)++;
            unsigned long temp_order_exp = 0;
            if (!base_to_dec(ptr_str, &cb, &(token->width), &temp_order_exp)) {
                float_value->order_exp = (int)temp_order_exp;
            }
        }
    }
}

// вычисления перенести в отдельную переменную добавить ширину и занести в переменные типа
// пересмотреть функции всех типов
void handler_fegEG(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    ParseFloat float_value;
    parse_float(ptr_str, token, &float_value);
    if (!token->suppress) {
        long double value = 0;
        value = to_float(float_value);
        if (token->length == 'l') {
            double* dest = va_arg(*args, double*);
            *dest = (double)value;
        } else if (token->length == 'L') {
            long double* dest = va_arg(*args, long double*);
            *dest = (long double)value;
        } else {
            float* dest = va_arg(*args, float*);
            *dest = (float)value;
        }
    }
}


void handler_p(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    Callback cb;
    cb.is_digit = s21_is_hex_digit;
    cb.to_digit = to_hex;
    cb.base = 16;

    unsigned long value = 0;
    if (!token->suppress && !base_to_dec(ptr_str, &cb, &(token->width), &value)) {
        void** dest = va_arg(*args, void**);
        *dest = (void*)value;
    }
}
