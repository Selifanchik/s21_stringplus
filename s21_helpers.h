#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <stdarg.h>
#include "s21_string.h"
#include "math.h"

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
    unsigned long int_part;
    unsigned long fract_part;
    int order_fract;
    int exp_part;
    int sign_exp;
    int order_exp;
    int s21_nan;
    int s21_inf;
} ParseFloat;

typedef struct {
    int length;
    char spec[16];
} LenSpec;


typedef struct {
    int length_modifier;
    const char* specifiers;
} FormatSpecGroup;

int handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args, Callback* cb);
int handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args, Callback* cb);
int handler_unsigned_int(const char** ptr_str, FormatSpecifier* token, va_list* args, const Callback* cb);
void handler_n(const char* start_str, const char* ptr_str, va_list* args);
int handler_c(const char** ptr_str, FormatSpecifier* token, va_list* args);
int handler_s(const char** ptr_str, FormatSpecifier* token, va_list* args);
int handler_fegEG(const char** ptr_str, FormatSpecifier* token, va_list* args);
int handler_p(const char** ptr_str, FormatSpecifier* token, va_list* args);

int s21_is_oct_digit(const char* symbol);
int s21_is_dec_digit(const char* symbol);
int s21_is_hex_digit(const char* symbol);

int to_oct_dec(const char* num);
int to_hex(const char* hex_num);

int base_to_dec(const char** ptr_str, const Callback* cb, int* width, unsigned long* value);

int parse_specifier(const char** ptr_format, FormatSpecifier* token);
int parse_str_sep(const char** ptr_str, const char* ptr_separation);
char* parse_format_sep(const char* start_format, const char* ptr_specifier);
int parse_value(const char* str, const char** ptr_str, FormatSpecifier* token, va_list* args);

int s21_isspace(int symbol);
int is_sign(const char** ptr_str, int* width);
void skip_space(const char** ptr_str);
int is_empty_or_whitespace(const char* ptr_str);

void init_token(FormatSpecifier* token);
void init_parse_float(ParseFloat* number);
void init_format_spec_group(FormatSpecGroup* spec_groups);

void is_prefix_base16(const char** ptr_str, int* width);
int is_valid_exponent(const char *ptr_str, int width);
int is_write_specifier(FormatSpecifier* token);

int parse_float(const char** ptr_str, FormatSpecifier* token, ParseFloat* float_value);
long double to_float(ParseFloat float_value);
long double s21_pow10(int order);
int s21_is_nan_inf(const char** ptr_str, int* width, ParseFloat* parse_float);
void to_nan_inf(long double* value, ParseFloat float_value);

int parse_i(const char** ptr_str, Callback* cb, int* width);

void to_base8(Callback* cb);
void to_base10(Callback* cb);
void to_base16(Callback* cb);
#endif
