#include "s21_string.h"

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
    char* separation = S21_NULL;
    while ((ptr_specifier = s21_strchr(ptr_specifier, '%')) != S21_NULL && !flag_end) {
        separation = parse_format_sep(ptr_format, ptr_specifier);
        if (*(ptr_specifier + 1) && separation) { 
            ptr_specifier++;
            if (!parse_str_sep(&ptr_str, separation) && !parse_specifier(&ptr_specifier, &token)) {
                if (!parse_value(str, &ptr_str, &token, &arg) && is_write_specifier(&token)) read_count++;
                ptr_format = ptr_specifier;
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
    Callback cb;
    if (token->specifier != 'c' && token->specifier != 'n') skip_space(ptr_str);
    switch (token->specifier) {
        case 'd':
            to_base10(&cb);
            if (handler_int(ptr_str, token, args, &cb)) flag_error = 1;;
            break;
        case 'i':
            if (handler_int(ptr_str, token, args, &cb)) flag_error = 1;
            break;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
            if (handler_fegEG(ptr_str, token, args)) flag_error = 1;
            break;
        case 'o':
            if (**ptr_str == '0') {
                if (token->width > 0 || token->width == -1) {
                    (*ptr_str)++;
                    if (token->width > 0) token->width--;
                } else flag_error = 1;
            }
            if (!flag_error) {
                to_base8(&cb);
                if (handler_unsigned_int(ptr_str, token, args, &cb)) flag_error = 1;
            }
            break;
        case 'c':
            if (handler_c(ptr_str, token, args)) flag_error = 1;
            break;
        case 's':
            if (handler_s(ptr_str, token, args)) flag_error = 1;
            break;
        case 'u':
            to_base10(&cb);
            if (handler_unsigned_int(ptr_str, token, args, &cb)) flag_error = 1;
            break;
        case 'x':
        case 'X':
            if (**ptr_str == '0' && ((*(*ptr_str + 1)) == 'x' || (*(*ptr_str + 1)) == 'X')) {
                if (token->width > 1 || token->width == -1) {
                    (*ptr_str) += 2;
                    if (token->width > 1) token->width -= 2;
                } else flag_error = 1;
            }
            if (!flag_error) {
                to_base16(&cb);
                if (handler_unsigned_int(ptr_str, token, args, &cb)) flag_error = 1;
            }
            break;
        case 'p':
            if (**ptr_str == '0' && ((*(*ptr_str + 1)) == 'x' || (*(*ptr_str + 1)) == 'X')) (*ptr_str) += 2;
            if (handler_p(ptr_str, token, args)) flag_error = 1;
            break;
        case 'n':
            handler_n(str, *ptr_str, args);
            break;
        case '%':
            if (**ptr_str == '%') (*ptr_str)++;
            else flag_error = 1;
            // иначе закончить считывание
            break;
    }
    return flag_error;
}

char* parse_format_sep(const char* start_format, const char* ptr_specifier) {
    s21_size_t length_sep = ptr_specifier - start_format;
    char* separation = (char*)calloc(length_sep + 1, sizeof(char));
    if (separation){
        // заменить на s21_strncpy
        s21_strncpy(separation, start_format, length_sep);
    }
    return separation;
}

int parse_str_sep(const char** ptr_str, const char* ptr_sep) {
    int flag_end = 0;
    while(*ptr_sep && !flag_end) {
        if (s21_isspace(*ptr_sep)) {
            skip_space(ptr_str);
            ptr_sep++;
        }
        else if (**ptr_str != *ptr_sep) flag_end = 1;
        else {
            (*ptr_str)++;
            ptr_sep++;
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
        unsigned long temp_width = 0;
        if (!base_to_dec(ptr_format, &cb, &width, &temp_width)) {
            token->width = (int)temp_width;
        }
    }
    // заменить strchr на s21_strchr
    const char* ptr_length = s21_strchr(lengths, **ptr_format);
    if (ptr_length != S21_NULL) {
        token->length = **ptr_format;
        (*ptr_format)++;
    }
    // заменить strchr на s21_strchr
    const char* ptr_specifier = s21_strchr(specifiers, **ptr_format);
    if (ptr_specifier != S21_NULL) {
        token->specifier = *ptr_specifier;
        (*ptr_format)++;
    } else {
        error = 1;
    }
    return error;
}

int handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args, Callback* cb) {
    int flag_error = 0;
    int sign = is_sign(ptr_str, &(token->width));
    if (token->specifier == 'i') flag_error = parse_i(ptr_str, cb, &(token->width));
    unsigned long value = 0;
    if (!flag_error && !base_to_dec(ptr_str, cb, &(token->width), &value)) {
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

int handler_c(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    int flag_error = 1;
    if (token->width == -1) token->width = 1;
    wchar_t* dest_w = (wchar_t*)S21_NULL;
    char* dest_c = (char*)S21_NULL;
    if (!token->suppress) {
        if (token->length == 'l') dest_w = va_arg(*args, wchar_t*);
        else dest_c = va_arg(*args, char*);
    }
    int i = 0;
    for (i = 0; i < token->width; i++) {
        if (**ptr_str) {
            if (!token->suppress) {
                if (token->length == 'l') *(dest_w + i) = (wchar_t)(unsigned char)*(*ptr_str);
                else *(dest_c + i) = *(*ptr_str);
            }
            (*ptr_str)++;
            flag_error = 0;
        }
    }
    if (token->width > 1 && !token->suppress) {
        if (token->length == 'l') *(dest_w + i) = L'\0';
        else *(dest_c + i) = '\0';
    }
    return flag_error;
}

int handler_s(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    int flag_error = 1;
    wchar_t* dest_w = (wchar_t*)S21_NULL;
    char* dest_c = (char*)S21_NULL;
    if (!token->suppress) {
        if (token->length == 'l') dest_w = va_arg(*args, wchar_t*);
        else dest_c = va_arg(*args, char*);
    }
    int i = 0;
    for (i = 0; ((i < token->width && token->width > 0) || token->width == -1) && !s21_isspace(**ptr_str); i++) {
        if (**ptr_str) {
            if (!token->suppress) {
                if (token->length == 'l') *(dest_w + i) = (wchar_t)(unsigned char)*(*ptr_str);
                else *(dest_c + i) = *(*ptr_str);
            }
            (*ptr_str)++;
            flag_error = 0;
        }
    }
    if (!token->suppress) {
        if (token->length == 'l') *(dest_w + i) = L'\0';
        else *(dest_c + i) = '\0';
    }
    return flag_error;
}

int parse_float(const char** ptr_str, FormatSpecifier* token, ParseFloat* float_value) {
    int flag_error = 0;
    Callback cb;
    to_base10(&cb);
    init_parse_float(float_value);
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

// вычисления перенести в отдельную переменную добавить ширину и занести в переменные типа
// пересмотреть функции всех типов
int handler_fegEG(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    int flag_error = 0;
    ParseFloat float_value;
    flag_error = parse_float(ptr_str, token, &float_value);
    if (!flag_error && !token->suppress) {
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
    return flag_error;
}

int handler_p(const char** ptr_str, FormatSpecifier* token, va_list* args) {
    int flag_error = 0;
    Callback cb;
    to_base16(&cb);
    unsigned long value = 0;
    if (!base_to_dec(ptr_str, &cb, &(token->width), &value)) {
        if (!token->suppress) {
            void** dest = va_arg(*args, void**);
            *dest = (void*)value;
        }
    } else flag_error = 1;
    return flag_error;
}
