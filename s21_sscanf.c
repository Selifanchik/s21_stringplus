#include "s21_helpers.h"
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
      free(separation);
      init_token(&token);
    }
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
      to_base10(&parser);
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
      to_base8(&parser);
      if (handler_unsigned_int(ptr_str, token, args, &parser)) flag_error = 1;
      break;
    case 'c':
      if (handler_c(ptr_str, token, args)) flag_error = 1;
      break;
    case 's':
      if (handler_s(ptr_str, token, args)) flag_error = 1;
      break;
    case 'u':
      to_base10(&parser);
      if (handler_unsigned_int(ptr_str, token, args, &parser)) flag_error = 1;
      break;
    case 'x':
    case 'X':
      is_prefix_base16(ptr_str, &(token->width));
      to_base16(&parser);
      if (handler_unsigned_int(ptr_str, token, args, &parser)) flag_error = 1;
      break;
    case 'p':
      is_prefix_base16(ptr_str, &(token->width));
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
    if (!base_to_dec(ptr_format, &parser, &width, &temp_width)) {
      token->width = (int)temp_width;
    }
  }
  // parse length
  if (!token->suppress && token->width != 0) {
    const char* ptr_length = s21_strchr(lengths, **ptr_format);
    if (ptr_length != S21_NULL) {
      token->length = **ptr_format;
      (*ptr_format)++;
    }
  }
  // parse all specifiers
  if (!token->suppress && !token->length && token->width == -1) {
    const char* ptr_specifier = s21_strchr(all_specifiers, **ptr_format);
    if (ptr_specifier != S21_NULL) {
      token->specifier = *ptr_specifier;
      (*ptr_format)++;
    }
    // parsing length dependent specifier
  } else if (token->width != 0) {
    int flag_end = 0;
    for (int i = 0; i < 4 && !flag_end; i++) {
      if (token->length == spec_groups[i].length_modifier) {
        const char* ptr_specifier =
            s21_strchr(spec_groups[i].specifiers, **ptr_format);
        if (ptr_specifier != S21_NULL) {
          token->specifier = *ptr_specifier;
          (*ptr_format)++;
          flag_end = 1;
        } else flag_error = 1;
      }
    }
  } else flag_error = 1;
  return flag_error;
}

int handler_int(const char** ptr_str, FormatSpecifier* token, va_list* args,
                DigitParser* parser) {
  int flag_error = 0;
  int sign = is_sign(ptr_str, &(token->width));
  if (token->specifier == 'i')
    flag_error = parse_i(ptr_str, parser, &(token->width));
  unsigned long value = 0;
  if (!flag_error && !base_to_dec(ptr_str, parser, &(token->width), &value)) {
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
  unsigned long value = 0;
  if (!base_to_dec(ptr_str, parser, &(token->width), &value)) {
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

int handler_c(const char** ptr_str, FormatSpecifier* token, va_list* args) {
  int flag_error = 1;
  if (token->width == -1) token->width = 1;
  wchar_t* dest_w = (wchar_t*)S21_NULL;
  char* dest_c = (char*)S21_NULL;
  if (!token->suppress) {
    if (token->length == 'l')
      dest_w = va_arg(*args, wchar_t*);
    else
      dest_c = va_arg(*args, char*);
  }
  int i = 0;
  for (i = 0; i < token->width; i++) {
    if (**ptr_str) {
      if (!token->suppress) {
        if (token->length == 'l')
          *(dest_w + i) = (wchar_t)(unsigned char)*(*ptr_str);
        else
          *(dest_c + i) = *(*ptr_str);
      }
      (*ptr_str)++;
      flag_error = 0;
    }
  }
  return flag_error;
}

int handler_s(const char** ptr_str, FormatSpecifier* token, va_list* args) {
  int flag_error = 1;
  wchar_t* dest_w = (wchar_t*)S21_NULL;
  char* dest_c = (char*)S21_NULL;
  if (!token->suppress) {
    if (token->length == 'l')
      dest_w = va_arg(*args, wchar_t*);
    else
      dest_c = va_arg(*args, char*);
  }
  int length_str = 0;
  while (**ptr_str && !s21_isspace(**ptr_str) &&
         is_valid_width(&(token->width), 0)) {
    if (!token->suppress) {
      if (token->length == 'l')
        *(dest_w + length_str) = (wchar_t)(unsigned char)*(*ptr_str);
      else
        *(dest_c + length_str) = *(*ptr_str);
    }
    (*ptr_str)++;
    flag_error = 0;
    if (token->width > 0) token->width--;
    length_str++;
  }
  if (!token->suppress) {
    if (token->length == 'l')
      *(dest_w + length_str) = L'\0';
    else
      *(dest_c + length_str) = '\0';
  }
  return flag_error;
}

int handler_fegEG(const char** ptr_str, FormatSpecifier* token, va_list* args) {
  int flag_error = 0;
  ParseFloat float_value;
  if (!s21_is_nan_inf(ptr_str, &(token->width), &float_value)) {
    flag_error = parse_float(ptr_str, token, &float_value);
  }
  if (!flag_error && !token->suppress) {
    long double value = 0.0;
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
  DigitParser parser;
  to_base16(&parser);
  unsigned long value = 0;
  if (!base_to_dec(ptr_str, &parser, &(token->width), &value)) {
    if (!token->suppress) {
      void** dest = va_arg(*args, void**);
      *dest = (void*)value;
    }
  } else
    flag_error = 1;
  return flag_error;
}
