#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

//для lc, ls
#include <locale.h>
#include <wchar.h>

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

void parsing(const char* format, int* i, flags* flag, va_list* arg);
void pars_flags_dlina(char chr, flags* flag);
int pars_width_tochnost(const char* format, int* i);
void define_specificator(char chr, flags flag, va_list* arg, char* buf);

void specificator_di(flags flag, va_list* arg, char* buf);
char* number_di_to_string(long number, flags flag);
int long_to_string(char* mas_for_number, int* index, long number);
char* rabota_tochnost(flags flag, int number, int dlina, char* mas_for_number,
                      int index);
char* zapolnenie_mas_result(int dlina, long number, flags flag,
                            char* mas_for_number);
char* rabota_width(flags flag, char* string, int dlina);

void specificator_uxXo(flags flag, va_list* arg, char* buf, char chr);
char* number_uxXo_to_string(unsigned long number, flags flag, int base,
                            char chr);
int rabota_reshetka(int dlina, int base, char* mas_for_number, char chr,
                    unsigned long number);

void specificator_c(flags flag, va_list* arg, char* buf);
void specificator_s(flags flag, va_list* arg, char* buf);
int tochnost_ls(wchar_t* value, flags flag);

void specificator_n(va_list* arg, char* buf);
void specificator_p(va_list* arg, char* buf, flags flag);

void specificator_feEgG(flags flag, va_list* arg, char* buf, char chr);
char* specificator_feE(flags flag, long double number, char* mas_for_left,
                       char chr);
char* specificator_gG(flags flag, long double number, char* mas_for_left,
                      char chr);
int float_to_string(long double number, char* mas_for_left, flags* flag,
                    char chr, int* mantis);
char* rabota_mantisa(char* string, char chr, int mantis);
int expanent(long double* number, int mantis);
int remove_0(char* string);
void round_my(char* tmp_mas_for_round, int dlina, int flag_bank);
void reverse_str(char* str);
void left_part_to_str(long double val, char* str, flags* flag);
void right_part_to_str(long double frac, char* str, int precision);

int s21_sprintf(char* str, const char* format, ...) {
  flags flag = {0};
  va_list arg;            // указатель на параметр
  va_start(arg, format);  // получение адреса первого вариадического параметра
                          // (то есть ссылка на последний не варидлический)
  s21_strncpy(str, "\0", 1);
  int dlina = s21_strlen(format);
  for (int i = 0; i < dlina; i++) {
    if (format[i] != '%') {  //пока не спецификатор просто печатаем
      char tmp[2] = {format[i], '\0'};
      s21_strncat(str, tmp, s21_strlen(tmp));
      continue;
    }
    i++;
    if (format[i] == '%') {  //если у нас "%%"" то просто печатаем и продолжаем
      char tmp[2] = {format[i], '\0'};
      s21_strncat(str, tmp, s21_strlen(tmp));
      continue;
    }

    s21_memset(&flag, 0, sizeof(flags));  //обнуление структуры

    parsing(format, &i, &flag, &arg);

    define_specificator(format[i], flag, &arg, str);
  }
  va_end(arg);  // завершение процедуры перебора вариадических параметров

  return 1;
}

void parsing(const char* format, int* i, flags* flag, va_list* arg) {
  while (!s21_strchr("cdieEfgGosuxXpn",
                     format[*i])) {  //добавить остальные спецификаторы   //пока
                                     //не спецификатор идем посимвольно

    if (s21_strchr("-+ #0", format[*i])) {
      pars_flags_dlina(format[*i], flag);
    }

    else if (s21_strchr("123456789*", format[*i])) {
      if (format[*i] == '*') {
        flag->width = va_arg(*arg, int);
      } else {
        flag->width = pars_width_tochnost(format, i);
        continue;
      }
    }

    else if (format[*i] == '.') {
      (*i)++;
      if (format[*i] == '*') {
        flag->tochnost = va_arg(*arg, int);
        flag->istochnost = 1;
      } else {
        flag->tochnost = pars_width_tochnost(format, i);
        flag->istochnost = 1;
        continue;
      }
    }

    if (s21_strchr("lhL", format[*i])) {
      pars_flags_dlina(format[*i], flag);
    }

    (*i)++;
  }
}

void pars_flags_dlina(char chr, flags* flag) {
  switch (chr) {
    case '-':
      flag->minus = 1;
      break;
    case '+':
      flag->plus = 1;
      break;
    case ' ':
      flag->space = 1;
      break;
    case '#':
      flag->reshetka = 1;
      break;
    case '0':
      flag->zero = 1;
      break;
    case 'h':
      flag->h = 1;
      break;
    case 'l':
      flag->l = 1;
      break;
    // case 'L':
    default:
      flag->L = 1;
      break;
  }
}

int pars_width_tochnost(const char* format, int* i) {
  int sum = 0;
  while (s21_strchr("0123456789", format[*i])) {
    sum = sum * 10 + (format[*i] - '0');
    (*i)++;
  }
  return sum;
}

void define_specificator(char chr, flags flag, va_list* arg, char* buf) {
  switch (chr) {
    case 'd':
      specificator_di(flag, arg, buf);
      break;
    case 'i':
      specificator_di(flag, arg, buf);
      break;
    case 'u':
      specificator_uxXo(flag, arg, buf, chr);
      break;
    case 'x':
      specificator_uxXo(flag, arg, buf, chr);
      break;
    case 'X':
      specificator_uxXo(flag, arg, buf, chr);
      break;
    case 'o':
      specificator_uxXo(flag, arg, buf, chr);
      break;
    case 's':
      specificator_s(flag, arg, buf);
      break;
    case 'c':
      specificator_c(flag, arg, buf);
      break;
    case 'f':
      specificator_feEgG(flag, arg, buf, chr);
      break;
    case 'e':
      specificator_feEgG(flag, arg, buf, chr);
      break;
    case 'E':
      specificator_feEgG(flag, arg, buf, chr);
      break;
    case 'g':
      specificator_feEgG(flag, arg, buf, chr);
      break;
    case 'G':
      specificator_feEgG(flag, arg, buf, chr);
      break;
    case 'n':
      specificator_n(arg, buf);
      break;
    // case 'p':
    default:
      specificator_p(arg, buf, flag);
      break;
  }
}

void NanAndInf(char chr, long double number, char* buf, flags flag) {
  int up = 0;
  if (chr == 'E' || chr == 'G') {
    up = 1;
  }
  if (isinf(number)) {
    if (signbit(number)) {
      s21_strncat(buf, up ? "-INF\0" : "-inf\0", 5);
    } else if (flag.plus) {
      s21_strncat(buf, up ? "+INF\0" : "+inf\0", 5);
    } else {
      s21_strncat(buf, up ? "INF" : "inf\0", 4);
    }
  }

  if (isnan(number)) {
    if (signbit(number)) {
      s21_strncat(buf, up ? "-NAN\0" : "-nan\0", 5);
    } else if (flag.plus) {
      s21_strncat(buf, up ? "+NAN\0" : "+nan\0", 5);
    } else {
      s21_strncat(buf, up ? "NAN\0" : "nan\0", 4);
    }
  }
}

void specificator_feEgG(flags flag, va_list* arg, char* buf, char chr) {
  char mas_for_left[8192];
  long double number;
  if (flag.L) {
    number = va_arg(*arg, long double);
  } else {
    number = va_arg(*arg, double);
  }
  int flag_nan_inf = 0;
  if (isinf(number) || isnan(number)) {
    NanAndInf(chr, number, buf, flag);
    flag_nan_inf = 1;
  }
  if (!flag.istochnost) {
    flag.istochnost = 1;
    flag.tochnost = 6;
  }

  if (!flag_nan_inf) {
    char* string;
    if (chr == 'f') {
      string = specificator_feE(flag, number, mas_for_left, chr);
    } else if (chr == 'e' || chr == 'E') {
      string = specificator_feE(flag, number, mas_for_left, chr);
    } else {
      string = specificator_gG(flag, number, mas_for_left, chr);
    }

    int dlina = s21_strlen(string);
    if (dlina < flag.width) {
      string = rabota_width(flag, string, dlina);
    }
    s21_strncat(buf, string, s21_strlen(string));
    free(string);
  }
}

char* specificator_gG(flags flag, long double number, char* mas_for_left,
                      char chr) {
  long double number_copy = number;
  if (number < 0) {
    number = number * -1;
  }
  int mantis = 0;
  mantis = expanent(&number, mantis);
  char* string;
  if (flag.tochnost == 0) {
    flag.tochnost++;
  }
  if (mantis >= -4 && mantis < flag.tochnost) {  // f
    chr = 'f';
    flag.isg = 1;

    int tmp_mantis = 0;
    if (mantis < 0) {
      tmp_mantis = mantis * -1;
    }

    flag.tochnost = flag.tochnost + tmp_mantis;
    string = specificator_feE(flag, number_copy, mas_for_left, chr);
  } else {  // e
    chr = chr == 'g' ? 'e' : 'E';
    flag.isg = 1;
    string = specificator_feE(flag, number_copy, mas_for_left, chr);
  }
  return string;
}

int remove_0(char* string) {
  int dlina = s21_strlen(string);
  dlina--;
  while (s21_strchr("123456789", string[dlina]) == S21_NULL) {
    dlina--;
    if (string[dlina + 1] == '.') {
      break;
    }
  }
  string[dlina + 1] = '\0';
  dlina++;  //
  return dlina;
}

char* specificator_feE(flags flag, long double number, char* mas_for_left,
                       char chr) {
  int znak = 1;
  if (signbit(number)) {
    number = -number;
    znak = -1;
  }

  int mantis = 0;

  if (chr == 'e' || chr == 'E') {
    mantis = expanent(&number, mantis);
  }

  int dlina = float_to_string(number, mas_for_left, &flag, chr, &mantis);

  if (!flag.reshetka && flag.isg) {
    dlina = remove_0(mas_for_left);
  }
  char* string = zapolnenie_mas_result(dlina, znak, flag, mas_for_left);
  if (chr == 'e' || chr == 'E') {
    string = rabota_mantisa(string, chr, mantis);
  }
  return string;
}

int expanent(long double* number, int mantis) {
  if (*number != 0.0) {
    mantis = (int)floorl(log10l(*number));
    *number = *number / powl(10.0L, mantis);
  }
  return mantis;
}

char* rabota_mantisa(char* string, char chr, int mantis) {
  int dlina = s21_strlen(string);
  string = realloc(string, dlina + 6);
  if (string == S21_NULL) {
    exit(0);
  }
  string[dlina++] = chr == 'e' ? 'e' : 'E';
  string[dlina++] = mantis < 0 ? '-' : '+';
  if (mantis < 0) mantis = mantis * -1;
  if (mantis < 10) {
    string[dlina++] = '0';
    string[dlina++] = '0' + mantis;
    string[dlina] = '\0';
  } else {
    string[dlina] = '\0';
    char mas_for_mantis[5];
    int index_mantis = 0;
    int dlina_mantis = long_to_string(mas_for_mantis, &index_mantis, mantis);
    s21_strncat(string, mas_for_mantis, s21_strlen(mas_for_mantis));
    string[dlina + dlina_mantis] = '\0';
  }
  return string;
}

void reverse_str(char* str) {
  int len = s21_strlen(str);
  for (int i = 0; i < len / 2; ++i) {
    char tmp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = tmp;
  }
}

void left_part_to_str(long double number, char* str, flags* flag) {
  if (number == 0.0L) {
    s21_strncpy(str, "0", 1);
    if (flag->isg) {
      flag->tochnost--;
    }
  } else {
    char buf[4096] = {0};
    int i = 0;
    while (number >= 1.0L) {
      long digit = (long)fmodl(number, 10.0L);
      buf[i++] = '0' + digit;
      number = floorl(number / 10.0L);
    }
    buf[i] = '\0';
    reverse_str(buf);
    if (flag->isg) {
      flag->tochnost = flag->tochnost - i;
    }
    s21_strncpy(str, buf, s21_strlen(buf));
  }
}

void right_part_to_str(long double right_part, char* str, int toch) {
  int i = 0;
  while (i < toch + 1) {
    right_part *= 10.0L;
    int digit = (int)right_part;
    str[i++] = '0' + digit;
    right_part -= digit;
  }
  str[toch + 1] = '\0';
}

void round_my(char* tmp_mas_for_round, int dlina, int flag_bank) {
  if (flag_bank) {
    int tochka = 0;
    while (tmp_mas_for_round[tochka] != '.') {
      tochka++;
    }
    if ((tmp_mas_for_round[tochka - 1] - '0') % 2 != 0) {
      for (int j = tochka - 1; j >= 0; j--) {
        if (tmp_mas_for_round[j] < '9') {
          tmp_mas_for_round[j]++;
          break;
        } else {
          tmp_mas_for_round[j] = '0';
        }
      }
    }
  } else if (tmp_mas_for_round[dlina - 1] >= '5') {
    for (int j = dlina - 2; j >= 0; j--) {
      if (tmp_mas_for_round[j] == '.') {
        continue;
      } else if (tmp_mas_for_round[j] < '9') {
        tmp_mas_for_round[j]++;
        break;
      } else {
        tmp_mas_for_round[j] = '0';
      }
    }
  }
}

int float_to_string(long double number, char* mas_for_left, flags* flag,
                    char chr, int* mantis) {
  long double intpart;
  int tmp_tochnost = flag->tochnost;
  int flag_round = 0;
  int flag_bank = 0;
  char int_str[4096] = {0};
  char frac_str[4096] = {0};

  long double frac = modfl(number, &intpart);
  if (frac == 0.5L) {
    flag_bank = 1;
  }

  left_part_to_str(intpart, int_str, flag);
  right_part_to_str(frac, frac_str, flag->tochnost);

  char* tmp_mas_for_round =
      malloc(s21_strlen(int_str) + s21_strlen(frac_str) + 3);

  s21_strncpy(tmp_mas_for_round, "0\0", 2);
  s21_strncat(tmp_mas_for_round, int_str, s21_strlen(int_str));
  s21_strncat(tmp_mas_for_round, ".", 1);
  s21_strncat(tmp_mas_for_round, frac_str, s21_strlen(frac_str));
  tmp_mas_for_round[s21_strlen(int_str) + s21_strlen(frac_str) + 3] = '\0';
  int dlina = s21_strlen(tmp_mas_for_round);

  round_my(tmp_mas_for_round, dlina, flag_bank);

  tmp_mas_for_round[dlina - 1] = '\0';

  if (tmp_mas_for_round[0] == '0') {
    s21_strncpy(mas_for_left, tmp_mas_for_round + 1, dlina - 1);
    mas_for_left[dlina - 2] = '\0';
  } else {
    s21_strncpy(mas_for_left, tmp_mas_for_round, dlina);
    mas_for_left[dlina - 1] = '\0';
    flag_round = 1;
  }

  dlina = s21_strlen(mas_for_left);

  if (s21_strchr("eE", chr) && mas_for_left[2] == '.') {
    *mantis += 1;
    mas_for_left[2] = mas_for_left[1];
    mas_for_left[1] = '.';
    mas_for_left[dlina - 1] = '\0';
  }

  if (flag->isg && flag_round) {
    int k = 0;
    int kolvo = tmp_tochnost;
    while (kolvo > 0) {
      if (mas_for_left[k] == '.') {
        k++;
        continue;
      }
      k++;
      kolvo--;
    }
    mas_for_left[k] = '\0';
  }

  if (!tmp_tochnost) {
    int k = 0;
    while (mas_for_left[k] != '.') {
      k++;
    }
    if (flag->reshetka) {
      mas_for_left[k + 1] = '\0';
    } else {
      mas_for_left[k] = '\0';
    }
  }

  free(tmp_mas_for_round);
  return s21_strlen(mas_for_left);
}

void specificator_n(va_list* arg, char* buf) {
  int dlina = s21_strlen(buf);
  int* ptr = va_arg(*arg, int*);
  *ptr = dlina;
}

void specificator_p(va_list* arg, char* buf, flags flag) {
  void* ptr = va_arg(*arg, void*);
  unsigned long number = (unsigned long)ptr;
  flag.reshetka = 1;
  char* string = number_uxXo_to_string(number, flag, 16, 'x');
  int dlina = s21_strlen(string);
  if (dlina < flag.width) {
    string = rabota_width(flag, string, dlina);
  }
  s21_strncat(buf, string, s21_strlen(string));
  free(string);
}

void specificator_c(flags flag, va_list* arg, char* buf) {
  char* string = malloc(MB_CUR_MAX + 1);
  if (string == S21_NULL) {
    exit(0);
  }
  if (flag.l) {
    wchar_t value = va_arg(*arg, wchar_t);
    int len = wcrtomb(string, value, S21_NULL);
    string[len] = '\0';
  } else {
    int value_int = va_arg(*arg, int);
    string[0] = value_int;
    string[1] = '\0';
  }

  if (flag.width >= 2) {
    int dlina = s21_strlen(string);
    int raznica = flag.width - dlina;
    string = realloc(string, dlina + raznica + 1);
    if (string == S21_NULL) {
      exit(0);
    }
    if (flag.minus) {
      s21_memset(string + dlina, ' ', raznica);
    } else {
      char* tmp_mas = malloc(dlina);
      if (tmp_mas == S21_NULL) {
        exit(0);
      }
      s21_strncpy(tmp_mas, string, s21_strlen(string));
      s21_memset(string, ' ', raznica);
      string[raznica] = '\0';
      s21_strncat(string, tmp_mas, s21_strlen(tmp_mas));
      string[dlina + raznica] = '\0';
      free(tmp_mas);
    }
    string[dlina + raznica + 1] = '\0';
  }
  s21_strncat(buf, string, s21_strlen(string));
  free(string);
}

void specificator_s(flags flag, va_list* arg, char* buf) {
  char* string = S21_NULL;
  int flag_null = 0;
  if (flag.l) {
    wchar_t* value = va_arg(*arg, wchar_t*);
    if (value == NULL) {
      string = malloc(7);
      s21_strncpy(string, "(null)\0", 7);
      flag_null = 1;
    } else {
      int len = wcstombs(S21_NULL, value, 0);
      if (flag.istochnost && flag.tochnost < len) {
        len = tochnost_ls(value, flag);
      }
      string = malloc(len + 1);
      if (string == S21_NULL) {
        exit(0);
      }
      wcstombs(string, value, len);
      string[len] = '\0';
    }
  } else {
    const char* tmp = va_arg(*arg, const char*);
    if (tmp == NULL) {
      string = malloc(7);
      s21_strncpy(string, "(null)\0", 7);
      flag_null = 1;
    } else {
      string = malloc(s21_strlen(tmp) + 1);
      if (string == S21_NULL) {
        exit(0);
      }
      s21_strncpy(string, tmp, s21_strlen(tmp));
      string[s21_strlen(tmp)] = '\0';
    }
  }

  int dlina = s21_strlen(string);
  if (flag_null && flag.tochnost < dlina) {
    s21_strncpy(string, "\0", 1);
  } else if (flag.istochnost && !flag.l) {
    if (flag.tochnost < dlina) {
      string[flag.tochnost] = '\0';
    }
  }

  dlina = s21_strlen(string);
  if (flag.width && dlina < flag.width) {
    int raznica = flag.width - dlina;
    string = realloc(string, dlina + raznica + 1);
    if (string == S21_NULL) {
      exit(0);
    }
    if (flag.minus) {
      s21_memset(string + dlina, ' ', raznica);  //
    } else {
      char* tmp_mas = malloc(dlina);
      if (tmp_mas == S21_NULL) {
        exit(0);
      }
      s21_strncpy(tmp_mas, string, s21_strlen(string));
      s21_memset(string, ' ', raznica);
      string[raznica] = '\0';
      s21_strncat(string, tmp_mas, s21_strlen(tmp_mas));
      free(tmp_mas);
    }
    string[dlina + raznica] = '\0';
  }
  s21_strncat(buf, string, s21_strlen(string));
  free(string);
}

int tochnost_ls(wchar_t* value, flags flag) {
  char* string = malloc(MB_CUR_MAX);
  if (string == S21_NULL) {
    exit(0);
  }
  int dlina = 0;
  int i = 0;
  while (1) {
    int len = wcrtomb(string, value[i], S21_NULL);
    if (dlina + len > flag.tochnost) {
      break;
    }
    dlina += len;
    i++;
  }
  free(string);
  return dlina;
}

void specificator_uxXo(flags flag, va_list* arg, char* buf, char chr) {
  unsigned long value;
  if (flag.h) {
    value = (unsigned short)va_arg(*arg, unsigned int);
  } else if (flag.l) {
    value = va_arg(*arg, unsigned long);
  } else {
    value = va_arg(*arg, unsigned int);
  }
  char* string;
  if (chr == 'o') {
    string = number_uxXo_to_string(value, flag, 8, chr);
  } else if (chr == 'x' || chr == 'X') {
    string = number_uxXo_to_string(value, flag, 16, chr);
  } else {  // u
    string = number_uxXo_to_string(value, flag, 10, chr);
  }

  int dlina = s21_strlen(string);
  if (dlina < flag.width) {
    string = rabota_width(flag, string, dlina);
  }
  s21_strncat(buf, string, s21_strlen(string));
  free(string);
}

char* number_uxXo_to_string(unsigned long number, flags flag, int base,
                            char chr) {
  char* mas_for_number = malloc(64);
  if (mas_for_number == S21_NULL) {
    exit(0);
  }
  int index = 0;
  unsigned long number_copy = number;
  do {
    int number_tmp = number % base;
    if (base == 16) {
      mas_for_number[index] =
          (char)((number_tmp < 10) ? number_tmp + '0'
                                   : ((chr == 'x') ? number_tmp - 10 + 'a'
                                                   : number_tmp - 10 + 'A'));
    } else {
      mas_for_number[index] = (char)(number_tmp + '0');
    }
    number /= base;
    index++;
  } while (number != 0);

  mas_for_number[index] = '\0';

  int dlina = s21_strlen(mas_for_number);

  for (int i = 0; i < index / 2; i++) {
    int tmp1 = mas_for_number[i];
    mas_for_number[i] = mas_for_number[dlina - i - 1];
    mas_for_number[dlina - i - 1] = tmp1;
  }

  if (flag.istochnost) {
    mas_for_number =
        rabota_tochnost(flag, number_copy == 0, dlina, mas_for_number, index);
    dlina = s21_strlen(mas_for_number);
  }

  if (flag.reshetka) {
    dlina = rabota_reshetka(dlina, base, mas_for_number, chr, number_copy);
  }

  char* result = malloc(dlina + 1);
  if (result == S21_NULL) {
    exit(0);
  }
  for (int i = 0; i < dlina; i++) {
    result[i] = mas_for_number[i];
  }
  result[dlina] = '\0';

  free(mas_for_number);
  return result;
}

int rabota_reshetka(int dlina, int base, char* mas_for_number, char chr,
                    unsigned long number) {
  int flag_resh = 1;
  if (base == 8 && mas_for_number[0] == '0') {
    flag_resh = 0;
  }

  if (base == 16 && number == 0) {
    flag_resh = 0;
  }

  if (flag_resh) {
    int count = dlina - 1;
    int raznica = base == 8 ? 1 : 2;
    while (count >= 0) {
      mas_for_number[count + raznica] = mas_for_number[count];
      count--;
    }
    if (base == 16) {
      mas_for_number[1] = chr == 'x' ? 'x' : 'X';
      mas_for_number[0] = '0';
      dlina += 2;
    } else {
      mas_for_number[0] = '0';
      dlina += 1;
    }
  }

  return dlina;
}

void specificator_di(flags flag, va_list* arg, char* buf) {
  long value;
  if (flag.h) {
    value = (short)va_arg(*arg, int);
  } else if (flag.l) {
    value = va_arg(*arg, long);
  } else {
    value = va_arg(*arg, int);
  }
  char* string = number_di_to_string(value, flag);
  int dlina = s21_strlen(string);
  if (dlina < flag.width) {
    string = rabota_width(flag, string, dlina);
  }
  s21_strncat(buf, string, s21_strlen(string));
  free(string);
}

char* number_di_to_string(long number, flags flag) {
  long tmp_number = number;
  int flag_min = 0;
  if (number == (-9223372036854775807 - 1)) {
    flag_min = 1;
    tmp_number = (unsigned long)9223372036854775807;
  } else {
    tmp_number = (number < 0) ? -number : number;
  }
  char* mas_for_number = malloc(20);
  if (mas_for_number == S21_NULL) {
    exit(0);
  }
  int index = 0;

  int dlina = long_to_string(mas_for_number, &index, tmp_number);

  if (flag_min) {
    mas_for_number[dlina - 1] = '8';
  }

  if (flag.istochnost) {
    mas_for_number =
        rabota_tochnost(flag, number == 0, dlina, mas_for_number, index);
    dlina = s21_strlen(mas_for_number);
  }

  char* result = zapolnenie_mas_result(dlina, number, flag, mas_for_number);

  free(mas_for_number);
  return result;
}

int long_to_string(char* mas_for_number, int* index, long number) {
  do {
    mas_for_number[*index] = (char)((number % 10) + '0');
    number /= 10;
    (*index)++;
  } while (number != 0);
  mas_for_number[*index] = '\0';
  int dlina = s21_strlen(mas_for_number);

  for (int i = 0; i < *index / 2; i++) {
    int tmp1 = mas_for_number[i];
    mas_for_number[i] = mas_for_number[dlina - i - 1];
    mas_for_number[dlina - i - 1] = tmp1;
  }

  return dlina;
}

char* rabota_tochnost(flags flag, int zero, int dlina, char* mas_for_number,
                      int index) {
  if (!flag.tochnost && zero) {
    s21_strncpy(mas_for_number, "\0", 1);
    dlina = s21_strlen(mas_for_number);
  } else if (dlina < flag.tochnost) {
    int count = dlina - 1;
    int raznica = flag.tochnost - dlina;
    mas_for_number = realloc(mas_for_number, dlina + raznica + 1);
    if (mas_for_number == S21_NULL) {
      exit(0);
    }
    while (count >= 0) {
      mas_for_number[count + raznica] = mas_for_number[count];
      count--;
    }
    for (int i = 0; i < raznica; i++) {
      mas_for_number[i] = '0';
    }

    mas_for_number[index + raznica] = '\0';
  }
  return mas_for_number;
}

char* zapolnenie_mas_result(int dlina, long number, flags flag,
                            char* mas_for_number) {
  char* result = malloc(dlina + 2);
  if (result == S21_NULL) {
    exit(0);
  }
  int flag_0 = 0;
  if (number < 0) {
    result[0] = '-';
    flag_0++;
  } else if (flag.plus) {
    result[0] = '+';
    flag_0++;
  } else if (flag.space) {
    result[0] = ' ';
    flag_0++;
  }

  for (int i = 0; i < dlina; i++) {
    result[i + flag_0] = mas_for_number[i];
  }
  result[dlina + flag_0] = '\0';
  return result;
}

char* rabota_width(flags flag, char* string, int dlina) {
  int raznica = flag.width - dlina;
  string = realloc(string, dlina + raznica + 1);
  if (string == S21_NULL) {
    exit(0);
  }
  if (flag.minus) {
    s21_memset(string + dlina, ' ', raznica);  //
    string[dlina + raznica] = '\0';
  } else {
    int znak = 0;
    if (s21_strchr("+- ", string[0]) && flag.zero) {
      znak++;
    }
    if (flag.reshetka && s21_strchr("xX", string[1]) && flag.zero) {
      znak += 2;
    }

    int count = dlina - 1;
    while (count >= znak) {
      string[count + raznica] = string[count];
      count--;
    }
    s21_memset(string + znak, flag.zero ? '0' : ' ', raznica);
    string[dlina + raznica] = '\0';
  }
  return string;
}

// int main(){
//     setlocale(LC_ALL, "C.UTF-8"); //для lc, ls
//     char buf1[1024], buf2[1024];
//     //не работает
//     //"%.300e %+30.20e", 1e-308, 1.7976931348623157e+308

//     //19 до точки
//     //18 после точки

//     sprintf(buf1, "!%#x!", 15);
//     s21_sprintf(buf2, "!%#x!", 15);

//     printf("%s\n", buf1);
//     printf("%s\n", buf2);

//     printf("%d\n", s21_strncmp(buf1, buf2, s21_strlen(buf1)));

//     return 0;
// }
