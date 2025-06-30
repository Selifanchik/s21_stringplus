#ifndef S21_SSCANF_H
#define S21_SSCANF_H

#include "s21_string.h"

#define ULONG_MAX (unsigned long)-1

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
} DigitParser;

typedef struct {
  long double value;
  int sign_float;
  int exp_part;
  int sign_exp;
  int order_exp;
  int s21_nan;
  int s21_inf;
} ParseFloat;

typedef struct {
  int length_modifier;
  const char* specifiers;
} FormatSpecGroup;

typedef int (*ValidatorFunc)(const char*, const FormatSpecifier*);

char* parse_format_sep(const char* start_format, const char* ptr_specifier);
int parse_str_sep(const char** ptr_str, const char* ptr_sep);
int parse_specifier(const char** ptr_format, FormatSpecifier* token);
int parse_value(const char* str, const char** ptr_str, FormatSpecifier* token,
                va_list* args);

int handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args,
                DigitParser* parser);
int handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args,
                DigitParser* parser);
int handler_unsigned_int(const char** ptr_str, FormatSpecifier* token,
                         va_list* args, const DigitParser* parser);
void handler_n(const char* start_str, const char* ptr_str, va_list* args);
int handler_fegEG(const char** ptr_str, FormatSpecifier* token, va_list* args);
int handler_p(const char** ptr_str, FormatSpecifier* token, va_list* args);
int handler_cs(const char** ptr_str, FormatSpecifier* token, va_list* args,
               ValidatorFunc validator);

int parse_i(const char** ptr_str, DigitParser* parser, int* width);

int parse_float(const char** ptr_str, FormatSpecifier* token,
                ParseFloat* float_value);
int parse_mantissa(const char** ptr_str, int* width, long double* value);
void base_to_real(const char** ptr_str, int* width, long double* value,
                  int* flag_digit);
long double to_float(const ParseFloat* float_value);
int s21_is_nan_inf(const char** ptr_str, int* width, ParseFloat* float_value);
void to_nan_inf(long double* value, ParseFloat float_value);

int s21_sign(const char** ptr_str, int* width);
int s21_isspace(int symbol);
void skip_space(const char** ptr_str);
int is_prefix_base_hex(const char** ptr_str, int* width);

int s21_is_oct_digit(const char* symbol);
int s21_is_dec_digit(const char* symbol);
int s21_is_hex_digit(const char* symbol);

int to_oct_dec(const char* num);
int to_hex(const char* hex_num);

void base_oct(DigitParser* parser);
void base_dec(DigitParser* parser);
void base_hex(DigitParser* parser);

int base_to_dec(const char** ptr_str, const DigitParser* parser, int* width,
                unsigned long* value);

int is_valid_c(const char* ptr_str, const FormatSpecifier* token);
int is_valid_s(const char* ptr_str, const FormatSpecifier* token);
int is_valid_width(const int* width, short valid_width);
int is_write_specifier(const FormatSpecifier* token);
int is_valid_exponent(const char* ptr_str, int width);
long double s21_pow10(int order);
int s21_strncmp_icase(const char* str1, const char* str2, int width);

void init_token(FormatSpecifier* token);
void init_parse_float(ParseFloat* number);
void init_format_spec_group(FormatSpecGroup* spec_groups);
#endif