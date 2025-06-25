#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include "math.h"
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
  int length_modifier;
  const char* specifiers;
} FormatSpecGroup;

typedef struct {
  int minus;
  int plus;
  int space;
  int reshetka;
  int zero;
  int width;
  int istochnost;
  int tochnost;
  int h;
  int l;
  int L;
  int isg;
} flags;

typedef int (*ValidatorFunc)(const char*, FormatSpecifier*);

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
int parse_i(const char** ptr_str, DigitParser* parser, int* width);

int parse_specifier(const char** ptr_format, FormatSpecifier* token);
int parse_str_sep(const char** ptr_str, const char* ptr_sep);
char* parse_format_sep(const char* start_format, const char* ptr_specifier);
int parse_value(const char* str, const char** ptr_str, FormatSpecifier* token,
                va_list* args);

int s21_isspace(int symbol);
void skip_space(const char** ptr_str);
int is_write_specifier(FormatSpecifier* token);

void init_token(FormatSpecifier* token);
void init_parse_float(ParseFloat* number);
void init_format_spec_group(FormatSpecGroup* spec_groups);

int s21_sign(const char** ptr_str, int* width);
int is_prefix_base_hex(const char** ptr_str, int* width);
int is_valid_exponent(const char* ptr_str, int width);

int parse_float(const char** ptr_str, FormatSpecifier* token,
                ParseFloat* float_value);
long double to_float(ParseFloat float_value);
long double s21_pow10(int order);
int s21_is_nan_inf(const char** ptr_str, int* width, ParseFloat* parse_float);
void to_nan_inf(long double* value, ParseFloat float_value);

int is_valid_width(const int* width, short valid_width);
int s21_strncmp_icase(const char* str1, const char* str2, int width);

int valid_c(const char* ptr_str, FormatSpecifier* token);
int valid_s(const char* ptr_str, FormatSpecifier* token);

// sprintf
int parsing(const char* format, int* i, flags* flag, va_list* arg);
int pars_flags_dlina(char chr, flags* flag);
int pars_width_tochnost(const char* format, int* i);
int define_specificator(char chr, flags flag, va_list* arg, char* buf);

int specificator_di(flags flag, va_list* arg, char* buf);
char* number_di_to_string(long number, flags flag);
int long_to_string(char* mas_for_number, int* index, long number);
char* rabota_tochnost(flags flag, int number, int dlina, char* mas_for_number,
                      int index);
char* zapolnenie_mas_result(int dlina, long number, flags flag,
                            char* mas_for_number);
char* rabota_width(flags flag, char* string, int dlina);

int specificator_uxXo(flags flag, va_list* arg, char* buf, char chr);
char* number_uxXo_to_string(unsigned long number, flags flag, int base,
                            char chr);
int rabota_reshetka(int dlina, int base, char* mas_for_number, char chr,
                    unsigned long number);

int specificator_c(flags flag, va_list* arg, char* buf);
int specificator_s(flags flag, va_list* arg, char* buf);
char* s_null(char* string, int* flag_null);
char* s_width(char* string, int dlina, flags flag);

void specificator_n(va_list* arg, char* buf);
int specificator_p(va_list* arg, char* buf, flags flag);

int specificator_feEgG(flags flag, va_list* arg, char* buf, char chr);
char* specificator_feE(flags flag, long double number, char* mas_for_left,
                       char chr);
char* specificator_gG(flags flag, long double number, char* mas_for_left,
                      char chr);
int float_to_string(long double number, char* mas_for_left, flags* flag,
                    char chr, int* mantis);
void NanAndInf(char chr, long double number, char* buf, flags flag,
               int flag_inf_valgd);
char* rabota_mantisa(char* string, char chr, int mantis);
int expanent(long double* number, int mantis);
int remove_0(char* string);
void round_my(char* tmp_mas_for_round, int dlina, int flag_bank);
void reverse_str(char* str);
void left_part_to_str(long double val, char* str, flags* flag);
void right_part_to_str(long double frac, char* str, int precision);
void g_and_round(int tmp_tochnost, char* mas_for_left);
void net_toch_v_float(char* mas_for_left, flags flag);
void cpy_to_str(char* tmp_mas_for_round, char* mas_for_left, int dlina,
                int* flag_round);
#endif
