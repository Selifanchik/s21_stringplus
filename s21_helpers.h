#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <stdarg.h>

typedef struct {
    int suppress;
    int width;
    char length;
    char specifier;
} FormatSpecifier;

typedef struct {
    int (*is_digit)(const char*);
    int (*to_digit)(const char*);
    int base;
} Callback;

typedef struct {
    int sign_float;
    long int_part;
    int fract_part;
    int order_fract;
    int exp_part;
    int sign_exp;
    int order_exp;
} ParseFloat;

int s21_is_dec_digit(const char* symbol);
int s21_is_hex_digit(const char* symbol);
long get_number(const char** ptr_str);
int parse_specifier(const char** ptr_format, FormatSpecifier* token);
int s21_isspace(int symbol);
int parse_str_sep(const char** ptr_str, const char* ptr_separation);
char* parse_format_sep(const char* start_format, const char* ptr_specifier);
void parse_value(const char* str, const char** ptr_str, FormatSpecifier* token, va_list* args);
void handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args, Callback cb);
int base_to_dec(const char** ptr_str, Callback cb, int* width, unsigned long* value);
int to_oct_dec(const char* num);
int to_hex(const char* hex_num);
int s21_is_oct_digit(const char* symbol);
int is_sign(const char** ptr_str, int* width);
void handler_unsigned_int(const char** ptr_str, FormatSpecifier* token, va_list* args, Callback cb);
void handler_n(const char* start_str, const char* ptr_str, va_list* args);
void handler_c(const char** ptr_str, FormatSpecifier* token, va_list* args);
void handler_s(const char** ptr_str, FormatSpecifier* token, va_list* args);
void handler_fegEG(const char** ptr_str, FormatSpecifier* token, va_list* args);
void handler_p(const char** ptr_str, FormatSpecifier* token, va_list* args);
void init_token(FormatSpecifier* token);
void init_parse_float(ParseFloat* number);
void skip_space(const char** ptr_str);
void parse_float(const char** ptr_str, FormatSpecifier* token, ParseFloat* float_value);
long double to_float(ParseFloat float_value);
long double s21_pow10(int order);
#endif
