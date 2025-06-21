#include "s21_helpers.h"

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

void to_base8(DigitParser* parser) {
  parser->is_digit = s21_is_oct_digit;
  parser->to_digit = to_oct_dec;
  parser->base = 8;
}

void to_base10(DigitParser* parser) {
  parser->is_digit = s21_is_dec_digit;
  parser->to_digit = to_oct_dec;
  parser->base = 10;
}

void to_base16(DigitParser* parser) {
  parser->is_digit = s21_is_hex_digit;
  parser->to_digit = to_hex;
  parser->base = 16;
}

// rename struct DigitParser
int base_to_dec(const char** ptr_str, const DigitParser* parser, int* width,
                unsigned long* value) {
  int flag_parse_error = 1;
  while (parser->is_digit(*ptr_str) && is_valid_width(width, 0)) {
    int digit = parser->to_digit(*ptr_str);
    *value = *value * parser->base + digit;
    if (*width > 0) (*width)--;
    (*ptr_str)++;
    flag_parse_error = 0;
  }
  return flag_parse_error;
}

int parse_i(const char** ptr_str, DigitParser* parser, int* width, int* prefix) {
  int flag_error = 0;
  if (**ptr_str == '0' && (*(*ptr_str + 1) == 'x' || *(*ptr_str + 1) == 'X')) {
    *prefix = is_prefix_base16(ptr_str, width);
    to_base16(parser);
  } else if (**ptr_str == '0') {
    to_base8(parser);
  } else {
    to_base10(parser);
  }
  return flag_error;
}

int s21_isspace(int symbol) {
  int space_arr[] = {9, 10, 11, 12, 13, 32};
  int result = 0;
  for (s21_size_t i = 0; i < sizeof(space_arr) / sizeof(*space_arr); i++) {
    if (symbol == space_arr[i]) result = 1;
  }
  return result;
}

int is_valid_width(const int* width, short valid_width) {
  return (*width == -1 || *width > valid_width);
}

void skip_space(const char** ptr_str) {
  while (**ptr_str && s21_isspace(**ptr_str)) (*ptr_str)++;
}

int is_write_specifier(FormatSpecifier* token) {
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
  number->int_part = 0;
  number->fract_part = 0;
  number->order_fract = 0;
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

int is_sign(const char** ptr_str, int* width) {
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

int is_prefix_base16(const char** ptr_str, int* width) {
  int flag_error = 1;
  if (**ptr_str == '0' &&
      ((*(*ptr_str + 1)) == 'x' || (*(*ptr_str + 1)) == 'X')) {
    if (is_valid_width(width, 1)) {
      (*ptr_str) += 2;
      if (*width > 1) *width -= 2;
      flag_error = 0;
    }
  }
  return flag_error;
}

int is_valid_exponent(const char* ptr_str, int width) {
  int flag_error = 1;
  int is_exp = (*ptr_str == 'E' || *ptr_str == 'e');
  if (is_exp && is_valid_width(&width, 0)) flag_error = 0;
  return flag_error;
}

int parse_float(const char** ptr_str, FormatSpecifier* token,
                ParseFloat* float_value) {
  int flag_error = 0;
  DigitParser parser;
  to_base10(&parser);
  int flag_digit = 0;
  if (s21_is_dec_digit(*ptr_str)) {
    unsigned long temp_int_part = 0;
    if (!base_to_dec(ptr_str, &parser, &(token->width), &temp_int_part)) {
      float_value->int_part = temp_int_part;
      flag_digit = 1;
    }
  }
  if (**ptr_str == '.' && (flag_digit || s21_is_dec_digit(*ptr_str + 1)) &&
      is_valid_width(&(token->width), 0)) {
    (*ptr_str)++;
    if (token->width > 0) token->width--;
    const char* start_fract = *ptr_str;
    if (s21_is_dec_digit(*ptr_str)) {
      unsigned long temp_fract_part = 0;
      if (!base_to_dec(ptr_str, &parser, &(token->width), &temp_fract_part)) {
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
      if (token->width > 0) token->width--;
      float_value->sign_exp = is_sign(ptr_str, &(token->width));
      unsigned long temp_order_exp = 0;
      if (!base_to_dec(ptr_str, &parser, &(token->width), &temp_order_exp)) {
        float_value->order_exp = (int)temp_order_exp;
      }
    }
  } else
    flag_error = 1;
  return flag_error;
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
    value = (long double)((float_value.int_part +
                           ((long double)float_value.fract_part / fraction)) *
                          float_value.sign_float);
    if (float_value.exp_part) {
      long double exp = s21_pow10(float_value.order_exp);
      if (float_value.sign_exp > 0) {
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
  float_value->sign_float = is_sign(ptr_str, width);
  if (is_valid_width(width, 2)) {
    if (s21_strncmp_icase(*ptr_str, "nan", 3) == 0) {
      float_value->s21_nan = 1;
      if (*width > 2) *width -= 3;
      (*ptr_str) += 3;
      flag_nan_inf = 1;
    } else if ((*width == 3 ||
                (*(*ptr_str + 3) != 'i' && *(*ptr_str + 3) != 'I')) &&
               s21_strncmp_icase(*ptr_str, "inf", 3) == 0) {
      float_value->s21_inf = 1;
      if (*width > 2) *width -= 3;
      flag_nan_inf = 1;
      (*ptr_str) += 3;
    } else if (is_valid_width(width, 7) &&
               s21_strncmp_icase(*ptr_str, "infinity", 8) == 0) {
      float_value->s21_inf = 1;
      if (*width > 7) *width -= 8;
      flag_nan_inf = 1;
      (*ptr_str) += 8;
    }
  }
  return flag_nan_inf;
}

void to_nan_inf(long double* value, ParseFloat float_value) {
  if (float_value.s21_nan) {
    *value = NAN;
  } else if (float_value.s21_inf) {
    *value = INFINITY;
  }
  if (float_value.sign_float == -1) *value = -*value;
}

int s21_strncmp_icase(const char* str1, const char* str2, int width) {
  char temp_compare[9] = {'\0'};
  s21_strncat(temp_compare, str1, width);
  char* temp = (char*)s21_to_lower(temp_compare);
  int result = s21_strncmp(temp, str2, width);
  free(temp);
  return result;
}

int valid_c(const char* ptr_str, FormatSpecifier* token) {
  return *ptr_str && token->width > 0;
}

int valid_s(const char* ptr_str, FormatSpecifier* token) {
  return *ptr_str && !s21_isspace(*ptr_str) &&
         is_valid_width(&(token->width), 0);
}