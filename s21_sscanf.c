#include "s21_sscanf.h"

#include <stdio.h>

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
  while ((ptr_specifier = s21_strchr(ptr_specifier, '%')) != S21_NULL &&
         !flag_end) {
    char* separation = parse_format_sep(ptr_format, ptr_specifier);
    if (*(ptr_specifier + 1) && separation) {
      ptr_specifier++;
      if (!parse_str_sep(&ptr_str, separation) &&
          !parse_specifier(&ptr_specifier, &token)) {
        if (token.specifier != 'c' && token.specifier != 'n')
          skip_space(&ptr_str);
        if (*ptr_str == '\0' && token.specifier != 'n' && read_count == 0)
          read_count = -1;
        if (!parse_value(str, &ptr_str, &token, &arg)) {
          ptr_format = ptr_specifier;
          if (is_write_specifier(&token)) read_count++;
        } else
          flag_end = 1;
      } else
        flag_end = 1;
      init_token(&token);
    }
    if (separation) free(separation);
  }
  va_end(arg);
  return read_count;
}

int parse_value(const char* str, const char** ptr_str, FormatSpecifier* token,
                va_list* args) {
  int flag_error = 0;
  DigitParser parser;
  switch (token->specifier) {
    case 'd':
      base_dec(&parser);
      if (handler_int(ptr_str, token, args, &parser)) flag_error = 1;
      ;
      break;
    case 'i':
      if (handler_int(ptr_str, token, args, &parser)) flag_error = 1;
      break;
    case 'e':
    case 'E':
    case 'f':
    case 'g':
    case 'G':
      if (handler_fegEG(ptr_str, token, args)) flag_error = 1;
      break;
    case 'o':
      base_oct(&parser);
      if (handler_unsigned_int(ptr_str, token, args, &parser)) flag_error = 1;
      break;
    case 'c':
      if (handler_cs(ptr_str, token, args, is_valid_c)) flag_error = 1;
      break;
    case 's':
      if (handler_cs(ptr_str, token, args, is_valid_s)) flag_error = 1;
      break;
    case 'u':
      base_dec(&parser);
      if (handler_unsigned_int(ptr_str, token, args, &parser)) flag_error = 1;
      break;
    case 'x':
    case 'X':
      base_hex(&parser);
      if (handler_unsigned_int(ptr_str, token, args, &parser)) flag_error = 1;
      break;
    case 'p':
      if (handler_p(ptr_str, token, args)) flag_error = 1;
      break;
    case 'n':
      handler_n(str, *ptr_str, args);
      break;
    case '%':
      if (**ptr_str == '%')
        (*ptr_str)++;
      else
        flag_error = 1;
      break;
  }
  return flag_error;
}

char* parse_format_sep(const char* start_format, const char* ptr_specifier) {
  s21_size_t length_sep = ptr_specifier - start_format;
  char* separation = (char*)calloc(length_sep + 1, sizeof(char));
  if (separation) {
    s21_strncpy(separation, start_format, length_sep);
  }
  return separation;
}

int parse_str_sep(const char** ptr_str, const char* ptr_sep) {
  int flag_end = 0;
  while (*ptr_sep && !flag_end) {
    if (s21_isspace(*ptr_sep)) {
      skip_space(ptr_str);
      ptr_sep++;
    } else if (**ptr_str != *ptr_sep)
      flag_end = 1;
    else {
      (*ptr_str)++;
      ptr_sep++;
    }
  }
  return flag_end;
}

int parse_specifier(const char** ptr_format, FormatSpecifier* token) {
  FormatSpecGroup spec_groups[4];
  init_format_spec_group(spec_groups);
  const char all_specifiers[] = "cdieEfgGosuxXpn%";
  const char lengths[] = "lLh";
  int flag_error = 0;
  // parse *
  if (**ptr_format == '*') {
    token->suppress = 1;
    (*ptr_format)++;
  }
  // parse width
  if (s21_is_dec_digit(*ptr_format)) {
    DigitParser parser = {s21_is_dec_digit, to_oct_dec, 10};
    int width = -1;
    unsigned long temp_width = 0;
    base_to_dec(ptr_format, &parser, &width, &temp_width);
    if (temp_width != 0) token->width = (int)temp_width;
  }
  // parse length
  if (!token->suppress) {
    const char* ptr_length = s21_strchr(lengths, **ptr_format);
    if (ptr_length != S21_NULL) {
      token->length = **ptr_format;
      (*ptr_format)++;
    }
  }
  // parse all specifiers
  if (!token->suppress && token->length == 0 && token->width == -1) {
    const char* ptr_specifier = s21_strchr(all_specifiers, **ptr_format);
    if (ptr_specifier != S21_NULL) {
      token->specifier = *ptr_specifier;
      (*ptr_format)++;
    }
    // parsing length dependent specifier
  } else {
    int flag_end = 0;
    for (int i = 0; i < 4 && !flag_end; i++) {
      if (token->length == spec_groups[i].length_modifier) {
        const char* ptr_specifier =
            s21_strchr(spec_groups[i].specifiers, **ptr_format);
        if (ptr_specifier != S21_NULL) {
          token->specifier = *ptr_specifier;
          (*ptr_format)++;
          flag_end = 1;
        } else
          flag_error = 1;
      }
    }
  }
  return flag_error;
}

int handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args,
                DigitParser* parser) {
  int flag_error = 0;
  int prefix = 0;
  int sign = s21_sign(ptr_str, &(token->width));
  if (token->specifier == 'i')
    prefix = parse_i(ptr_str, parser, &(token->width));
  unsigned long value = 0;
  if (!base_to_dec(ptr_str, parser, &(token->width), &value) || prefix) {
    if (sign == -1 && value == ULONG_MAX) value++;
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
  } else
    flag_error = 1;
  return flag_error;
}

int handler_unsigned_int(const char** ptr_str, FormatSpecifier* token,
                         va_list* args, const DigitParser* parser) {
  int flag_error = 0;
  int sign = s21_sign(ptr_str, &(token->width));
  int flag_prefix = 0;
  if (s21_strchr("xX", token->specifier))
    flag_prefix = is_prefix_base_hex(ptr_str, &(token->width));
  unsigned long value = 0;
  if (!base_to_dec(ptr_str, parser, &(token->width), &value) || flag_prefix) {
    if (sign == -1 && value == ULONG_MAX) sign = 1;
    if (!token->suppress) {
      if (token->length == 'l') {
        unsigned long* dest = va_arg(*args, unsigned long*);
        *dest = (unsigned long)value * sign;
      } else if (token->length == 'h') {
        unsigned short* dest = va_arg(*args, unsigned short*);
        *dest = (unsigned short)value * sign;
      } else {
        unsigned* dest = va_arg(*args, unsigned*);
        *dest = (unsigned)value * sign;
      }
    }
  } else
    flag_error = 1;
  return flag_error;
}

void handler_n(const char* start_str, const char* ptr_str, va_list* args) {
  if (*start_str || !*ptr_str) {
    int value = ptr_str - start_str;
    int* dest = va_arg(*args, int*);
    *dest = (int)value;
  }
}

int handler_cs(const char** ptr_str, FormatSpecifier* token, va_list* args,
               ValidatorFunc validator) {
  if (token->specifier == 'c' && token->width == -1) token->width = 1;
  int flag_error = 1;
  char* dest_c = (char*)S21_NULL;
  if (!token->suppress) {
    dest_c = va_arg(*args, char*);
  }
  int length_str = 0;
  while (validator(*ptr_str, token)) {
    if (!token->suppress) {
      *(dest_c + length_str) = *(*ptr_str);
    }
    (*ptr_str)++;
    flag_error = 0;
    if (token->width > 0) token->width--;
    length_str++;
  }
  if (!token->suppress && token->specifier == 's') {
    *(dest_c + length_str) = '\0';
  }
  return flag_error;
}

int handler_fegEG(const char** ptr_str, FormatSpecifier* token, va_list* args) {
  int flag_error = 0;
  ParseFloat float_value;
  int flag_end = 0;
  if ((flag_end = s21_is_nan_inf(ptr_str, &(token->width), &float_value)) ==
      0) {
    flag_error = parse_float(ptr_str, token, &float_value);
  } else if (flag_end == 2)
    flag_error = 1;
  if (!flag_error && !token->suppress) {
    long double value = 0.0;
    value = to_float(&float_value);
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
  DigitParser parser;
  base_hex(&parser);
  int sign = s21_sign(ptr_str, &(token->width));
  int prefix = is_prefix_base_hex(ptr_str, &(token->width));
  unsigned long value = 0;
  if (!base_to_dec(ptr_str, &parser, &(token->width), &value) || prefix) {
    if (sign == -1 && value == ULONG_MAX) sign = 1;
    if (!token->suppress) {
      void** dest = va_arg(*args, void**);
      *dest = (void*)(value * sign);
    }
  } else
    flag_error = 1;
  return flag_error;
}

int s21_is_oct_digit(const char* symbol) {
  return (*symbol >= '0' && *symbol <= '7');
}

int s21_is_dec_digit(const char* symbol) {
  return (*symbol >= '0' && *symbol <= '9');
}

int s21_is_hex_digit(const char* symbol) {
  return ((*symbol >= '0' && *symbol <= '9') ||
          (*symbol >= 'A' && *symbol <= 'F') ||
          (*symbol >= 'a' && *symbol <= 'f'));
}

int to_oct_dec(const char* num) { return (*num - '0'); }

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

void base_oct(DigitParser* parser) {
  parser->is_digit = s21_is_oct_digit;
  parser->to_digit = to_oct_dec;
  parser->base = 8;
}

void base_dec(DigitParser* parser) {
  parser->is_digit = s21_is_dec_digit;
  parser->to_digit = to_oct_dec;
  parser->base = 10;
}

void base_hex(DigitParser* parser) {
  parser->is_digit = s21_is_hex_digit;
  parser->to_digit = to_hex;
  parser->base = 16;
}

int base_to_dec(const char** ptr_str, const DigitParser* parser, int* width,
                unsigned long* value) {
  int flag_parse_error = 1;
  while (parser->is_digit(*ptr_str) && is_valid_width(width, 0)) {
    int digit = parser->to_digit(*ptr_str);
    if (*value > (ULONG_MAX - (unsigned long)digit) / 10UL) {
      *value = ULONG_MAX;
    } else {
      *value = *value * parser->base + digit;
    }
    if (*width > 0) (*width)--;
    (*ptr_str)++;
    flag_parse_error = 0;
  }
  return flag_parse_error;
}

int parse_i(const char** ptr_str, DigitParser* parser, int* width) {
  int prefix = 0;
  if (**ptr_str == '0' && (*(*ptr_str + 1) == 'x' || *(*ptr_str + 1) == 'X')) {
    prefix = is_prefix_base_hex(ptr_str, width);
    base_hex(parser);
  } else if (**ptr_str == '0') {
    base_oct(parser);
  } else {
    base_dec(parser);
  }
  return prefix;
}

int s21_isspace(int symbol) {
  int space_arr[] = {9, 10, 11, 12, 13, 32};
  int result = 0;
  for (s21_size_t i = 0; i < sizeof(space_arr) / sizeof(*space_arr); i++) {
    if (symbol == space_arr[i]) result = 1;
  }
  return result;
}

int is_valid_c(const char* ptr_str, const FormatSpecifier* token) {
  return *ptr_str && token->width > 0;
}

int is_valid_s(const char* ptr_str, const FormatSpecifier* token) {
  return *ptr_str && !s21_isspace(*ptr_str) &&
         is_valid_width(&(token->width), 0);
}

int is_valid_width(const int* width, short valid_width) {
  return (*width == -1 || *width > valid_width);
}

void skip_space(const char** ptr_str) {
  while (**ptr_str && s21_isspace(**ptr_str)) (*ptr_str)++;
}

int is_write_specifier(const FormatSpecifier* token) {
  return !token->suppress && token->specifier != '%' && token->specifier != 'n';
}

void init_token(FormatSpecifier* token) {
  token->length = 0;
  token->specifier = 0;
  token->suppress = 0;
  token->width = -1;
}

void init_parse_float(ParseFloat* number) {
  number->sign_float = 1;
  number->value = 0.0;
  number->exp_part = 0;
  number->sign_exp = 1;
  number->order_exp = 0;
  number->s21_nan = 0;
  number->s21_inf = 0;
}

void init_format_spec_group(FormatSpecGroup* spec_groups) {
  spec_groups[0].length_modifier = 0;
  spec_groups[0].specifiers = "csdiouxXceEgGfp";
  spec_groups[1].length_modifier = 'h';
  spec_groups[1].specifiers = "diouxX";
  spec_groups[2].length_modifier = 'l';
  spec_groups[2].specifiers = "csdiouxXceEgGf";
  spec_groups[3].length_modifier = 'L';
  spec_groups[3].specifiers = "eEgGf";
}

int s21_sign(const char** ptr_str, int* width) {
  int sign = 1;
  if ((**ptr_str == '-' || **ptr_str == '+') && is_valid_width(width, 0)) {
    if (**ptr_str == '-') {
      sign = -1;
    }
    (*ptr_str)++;
    if (*width > 0) ((*width)--);
  }
  return sign;
}

int is_prefix_base_hex(const char** ptr_str, int* width) {
  int ret = 0;
  if (**ptr_str == '0' &&
      ((*(*ptr_str + 1)) == 'x' || (*(*ptr_str + 1)) == 'X')) {
    if (is_valid_width(width, 1)) {
      (*ptr_str) += 2;
      if (*width > 1) *width -= 2;
      ret = 1;
    }
  }
  return ret;
}

int is_valid_exponent(const char* ptr_str, int width) {
  int ret = 0;
  int is_exp = (*ptr_str == 'E' || *ptr_str == 'e');
  if (is_exp && is_valid_width(&width, 0)) ret = 1;
  return ret;
}

int parse_float(const char** ptr_str, FormatSpecifier* token,
                ParseFloat* float_value) {
  int flag_error = 0;
  DigitParser parser;
  base_dec(&parser);
  int flag_digit = 0;
  flag_digit = parse_mantissa(ptr_str, &(token->width), &(float_value->value));
  if (flag_digit) {
    if (is_valid_exponent(*ptr_str, token->width)) {
      float_value->exp_part = 1;
      (*ptr_str)++;
      if (token->width > 0) token->width--;
      float_value->sign_exp = s21_sign(ptr_str, &(token->width));
      unsigned long temp_order_exp = 0;
      if (!base_to_dec(ptr_str, &parser, &(token->width), &temp_order_exp)) {
        float_value->order_exp = (int)temp_order_exp;
      }
    }
  } else
    flag_error = 1;
  return flag_error;
}

int parse_mantissa(const char** ptr_str, int* width, long double* value) {
  long double number = 0.0;
  int flag_digit = 0;
  long double fraction = 1.0;
  base_to_real(ptr_str, width, &number, &flag_digit);
  if (**ptr_str == '.' && (flag_digit || s21_is_dec_digit(*ptr_str + 1)) &&
      is_valid_width(width, 0)) {
    (*ptr_str)++;
    if (*width > 0) (*width)--;
    const char* start_fract = *ptr_str;
    base_to_real(ptr_str, width, &number, &flag_digit);
    fraction = s21_pow10(*ptr_str - start_fract);
  }
  if (flag_digit) *value = number / fraction;
  return flag_digit;
}

void base_to_real(const char** ptr_str, int* width, long double* value,
                  int* flag_digit) {
  while (s21_is_dec_digit(*ptr_str) && is_valid_width(width, 0)) {
    *value = *value * 10.0 + (**ptr_str - '0');
    (*ptr_str)++;
    if (*width > 0) (*width)--;
    *flag_digit = 1;
  }
}

long double to_float(const ParseFloat* float_value) {
  long double value = 0.0;
  if (float_value->s21_nan || float_value->s21_inf) {
    to_nan_inf(&value, *float_value);
  } else {
    value = float_value->value * float_value->sign_float;
    if (float_value->exp_part) {
      long double exp = s21_pow10(float_value->order_exp);
      if (float_value->sign_exp > 0) {
        value *= exp;
      } else
        value /= exp;
    }
  }
  return value;
}

long double s21_pow10(int order) {
  long double result = 1.0;
  for (int i = 0; i < order; i++) {
    result *= 10.;
  }
  return result;
}

int s21_is_nan_inf(const char** ptr_str, int* width, ParseFloat* float_value) {
  int flag_nan_inf = 0;
  init_parse_float(float_value);
  float_value->sign_float = s21_sign(ptr_str, width);
  if (is_valid_width(width, 2)) {
    int flag_err1 = 0, flag_err2 = 0, flag_err3 = 0;
    if ((flag_err1 = s21_strncmp_icase(*ptr_str, "nan", 3)) == 0) {
      float_value->s21_nan = 1;
      if (*width > 2) *width -= 3;
      (*ptr_str) += 3;
      flag_nan_inf = 1;
    } else if ((*width == 3 ||
                (*(*ptr_str + 3) != 'i' && *(*ptr_str + 3) != 'I')) &&
               (flag_err2 = s21_strncmp_icase(*ptr_str, "inf", 3)) == 0) {
      float_value->s21_inf = 1;
      if (*width > 2) *width -= 3;
      flag_nan_inf = 1;
      (*ptr_str) += 3;
    } else if (is_valid_width(width, 7) &&
               (flag_err3 = s21_strncmp_icase(*ptr_str, "infinity", 8)) == 0) {
      float_value->s21_inf = 1;
      if (*width > 7) *width -= 8;
      flag_nan_inf = 1;
      (*ptr_str) += 8;
    }
    if (flag_err1 == 2 || flag_err2 == 2 || flag_err3 == 2) {
      flag_nan_inf = 2;
    }
  }
  return flag_nan_inf;
}

void to_nan_inf(long double* value, ParseFloat float_value) {
  if (float_value.s21_nan) {
    *value = NAN;
  } else {
    *value = INFINITY;
  }
  if (float_value.sign_float == -1) *value = -*value;
}

int s21_strncmp_icase(const char* str1, const char* str2, int width) {
  int result = 2;
  char temp_compare[9] = {'\0'};
  s21_strncat(temp_compare, str1, width);
  char* temp = s21_to_lower(temp_compare);
  if (temp) {
    result = s21_strncmp(temp, str2, width);
    free(temp);
  }
  return result;
}
