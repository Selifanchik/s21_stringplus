#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdarg.h>
#include <stdlib.h>

typedef unsigned long s21_size_t;
#define S21_NULL ((void *)0)

typedef struct {
  int suppress;
  int width;
  char length;
  char specifier;
} FormatSpecifier;

typedef struct {
  int (*is_digit)(const char *);
  int (*to_digit)(const char *);
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

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_sscanf(const char *str, const char *format, ...);

int s21_is_dec_digit(const char *symbol);
int s21_is_hex_digit(const char *symbol);
long get_number(const char **ptr_str);
int parse_specifier(const char **ptr_format, FormatSpecifier *token);
double s21_atof(char **ptr_str);
int s21_isspace(int symbol);
int parse_str_sep(const char **ptr_str, const char *ptr_separation);
char *parse_format_sep(const char *start_format, const char *ptr_specifier);
void parse_value(const char *str, const char **ptr_str, FormatSpecifier *token,
                 va_list *args);
void handler_int(const char **ptr_str, FormatSpecifier *token, va_list *args,
                 Callback cb);
long base_to_dec(const char **num, Callback cb, int *width);
int to_oct_dec(const char *num);
int to_hex(const char *hex_num);
int s21_is_oct_digit(const char *symbol);
int is_sign(const char **ptr_str, int *width);
void handler_unsigned_int(const char **ptr_str, FormatSpecifier *token,
                          va_list *args, Callback cb);
void handler_n(const char *start_str, const char *ptr_str, va_list *args);
void handler_c(const char **ptr_str, FormatSpecifier *token, va_list *args);
void handler_s(const char **ptr_str, FormatSpecifier *token, va_list *args);
void handler_fegEG(const char **ptr_str, FormatSpecifier *token, va_list *args);
void handler_p(const char **ptr_str, FormatSpecifier *token, va_list *args);
void init_token(FormatSpecifier *token);
void init_parse_float(ParseFloat *number);
void skip_space(const char **ptr_str);
void parse_float(const char **ptr_str, FormatSpecifier *token,
                 ParseFloat *float_value);
long double to_float(ParseFloat float_value);
long double s21_pow10(int order);
const char *ltrim(const char *src, const char *trim_chars);
const char *rtrim(const char *src, const char *trim_chars);
void *trim(const char *src, const char *trim_chars);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
int s21_sprintf(char *str, const char *format, ...);

#endif