#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

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
void NanAndInf(char chr, long double number, char* buf, flags flag, int flag_inf_valgd);
char* rabota_mantisa(char* string, char chr, int mantis);
int expanent(long double* number, int mantis);
int remove_0(char* string);
void round_my(char* tmp_mas_for_round, int dlina, int flag_bank);
void reverse_str(char* str);
void left_part_to_str(long double val, char* str, flags* flag);
void right_part_to_str(long double frac, char* str, int precision);
void g_and_round(int tmp_tochnost, char* mas_for_left);
void net_toch_v_float(char* mas_for_left, flags flag);
void cpy_to_str(char* tmp_mas_for_round, char* mas_for_left, int dlina, int* flag_round);

int s21_sprintf(char* str, const char* format, ...) {
  flags flag = {0};
  va_list arg;
  va_start(arg, format);
  
  int error = 0;
  int result = -1;
  s21_strncpy(str, "\0", 1);
  int dlina = s21_strlen(format);
  for (int i = 0; i < dlina && error != 1; i++) {
    if (format[i] != '%') {
      char tmp[2] = {format[i], '\0'};
      s21_strncat(str, tmp, s21_strlen(tmp) + 1);
      continue;
    }
    i++;
    if (format[i] == '%') {
      char tmp[2] = {format[i], '\0'};
      s21_strncat(str, tmp, s21_strlen(tmp) + 1);
      continue;
    }

    s21_memset(&flag, 0, sizeof(flags));

    if (parsing(format, &i, &flag, &arg)) {
      error = 1;
    }

    if (flag.zero && flag.istochnost &&
        s21_strchr("feEgG", format[i]) == S21_NULL) {
      error = 1;
    }

    if (define_specificator(format[i], flag, &arg, str)) {
      error = 1;
    }
  }
  va_end(arg);
  
  if(!error){
    result = s21_strlen(str);
  }
  return result;
}

int parsing(const char* format, int* i, flags* flag, va_list* arg) {
  int error = 0, m_flags = 1, m_width = 1, m_toch = 1, m_dlina = 1;
  while (format[*i] != '\0' && !s21_strchr("cdieEfgGosuxXpn", format[*i]) &&
         !error) {
    if (s21_strchr("-+ #0", format[*i]) && m_flags) {
      error = pars_flags_dlina(format[*i], flag);
    }

    else if (s21_strchr("123456789*", format[*i]) && m_width) {
      m_flags = 0;
      if (format[*i] == '*') {
        flag->width = va_arg(*arg, int);
      } else {
        flag->width = pars_width_tochnost(format, i);
        continue;
      }
    }

    else if (format[*i] == '.' && m_toch) {
      m_flags = 0;
      m_width = 0;
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

    else if (s21_strchr("lhL", format[*i]) && m_dlina) {
      m_flags = 0;
      m_width = 0;
      m_toch = 0;
      m_dlina = 0;
      error = pars_flags_dlina(format[*i], flag);
    }

    else {
      error = 1;
    }

    (*i)++;
  }
  if (format[*i] == '\0') {
    error = 1;
  }
  return error;
}

int pars_flags_dlina(char chr, flags* flag) {
  int error = 0;
  switch (chr) {
    case '-':
      flag->minus = 1;
      if (flag->zero) {
        error = 1;
      }
      break;
    case '+':
      flag->plus = 1;
      if (flag->space) {
        error = 1;
      }
      break;
    case ' ':
      flag->space = 1;
      if (flag->plus) {
        error = 1;
      }
      break;
    case '#':
      flag->reshetka = 1;
      break;
    case '0':
      flag->zero = 1;
      if (flag->minus) {
        error = 1;
      }
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
  return error;
}

int pars_width_tochnost(const char* format, int* i) {
  int sum = 0;
  while (format[*i] != '\0' && s21_strchr("0123456789", format[*i])) {
    sum = sum * 10 + (format[*i] - '0');
    (*i)++;
  }
  return sum;
}

int define_specificator(char chr, flags flag, va_list* arg, char* buf) {
  int error = 0;
  switch (chr) {
    case 'd':
    case 'i':
      if (flag.L || flag.reshetka) {
        error = 1;
      } else {
        error = specificator_di(flag, arg, buf);
      }
      break;
    case 'u':
      if (flag.L || flag.reshetka || flag.plus || flag.space) {
        error = 1;
      } else {
        error = specificator_uxXo(flag, arg, buf, chr);
      }
      break;
    case 'x':
    case 'X':
    case 'o':
      if (flag.L || flag.plus || flag.space) {
        error = 1;
      } else {
        error = specificator_uxXo(flag, arg, buf, chr);
      }
      break;
    case 's':
      if (flag.h || flag.L || flag.zero || flag.reshetka || flag.plus ||
          flag.space) {
        error = 1;
      } else {
        error = specificator_s(flag, arg, buf);
      }
      break;
    case 'c':
      if (flag.h || flag.L || flag.zero || flag.reshetka || flag.plus ||
          flag.space || flag.istochnost) {
        error = 1;
      } else {
        error = specificator_c(flag, arg, buf);
      }
      break;
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      if (flag.l || flag.h) {
        error = 1;
      } else {
        error = specificator_feEgG(flag, arg, buf, chr);
      }
      break;
    case 'n':
      if (flag.istochnost || flag.L || flag.l || flag.h || flag.minus ||
          flag.plus || flag.reshetka || flag.space || flag.width || flag.zero) {
        error = 1;
      } else {
        specificator_n(arg, buf);
      }
      break;
    // case 'p':
    default:
      if (flag.istochnost || flag.L || flag.l || flag.h || flag.plus ||
          flag.reshetka || flag.space || flag.zero) {
        error = 1;
      } else {
        error = specificator_p(arg, buf, flag);
      }
      break;
  }
  return error;
}

void NanAndInf(char chr, long double number, char* buf, flags flag, int flag_inf_valgd) {
  int up = 0;
  if (chr == 'E' || chr == 'G') {
    up = 1;
  }
  if (isinf(number) || flag_inf_valgd) {
    if (signbit(number)) {
      s21_strncat(buf, up ? "-INF\0" : "-inf\0", 5);
    } else if (flag.plus) {
      s21_strncat(buf, up ? "+INF\0" : "+inf\0", 5);
    } else {
      s21_strncat(buf, up ? "INF\0" : "inf\0", 4);
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

int specificator_feEgG(flags flag, va_list* arg, char* buf, char chr) {
  int error = 0;
  char mas_for_left[6100];
  long double number;
  if (flag.L) {
    number = va_arg(*arg, long double);
  } else {
    number = va_arg(*arg, double);
  }
  int flag_nan_inf = 0;
  int flag_inf_valgd = 0;
  if (number >= 1.189731e+4932L || number <= -1.189731e+4932L) {
    flag_inf_valgd = 1;
  }
  if (isinf(number) || isnan(number) || flag_inf_valgd) {
    NanAndInf(chr, number, buf, flag, flag_inf_valgd);
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
    if(string != S21_NULL){
      int dlina = s21_strlen(string);
      if (dlina < flag.width) {
        string = rabota_width(flag, string, dlina);
      }

      if(string != S21_NULL){
        s21_strncat(buf, string, s21_strlen(string) + 1);
        free(string);
      }
      else{
        error = 1;
      }
    }
    else{
      error = 1;
    }
  }
  return error;
}

char* specificator_gG(flags flag, long double number, char* mas_for_left, char chr) {
  long double number_copy = number;
  if (number < 0) {
    number = number * -1;
  }
  int mantis = 0;
  mantis = expanent(&number, mantis);
  char* string = S21_NULL;
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

char* specificator_feE(flags flag, long double number, char* mas_for_left, char chr) {
  char* string = S21_NULL;
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

  if(dlina != -1){
    if (!flag.reshetka && flag.isg) {
      dlina = remove_0(mas_for_left);
    }
    string = zapolnenie_mas_result(dlina, znak, flag, mas_for_left);
    if(string != S21_NULL){
      if (chr == 'e' || chr == 'E') {
        string = rabota_mantisa(string, chr, mantis);
      }
    }
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
  string = realloc(string, dlina + 7);
  if(string != S21_NULL){
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
    s21_strncpy(str, "0\0", 2);
    if (flag->isg) {
      flag->tochnost--;
    }
  } else {
    char buf[5000] = {0};
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
    s21_strncpy(str, buf, i + 1);
    str[i] = '\0';
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

void g_and_round(int tmp_tochnost, char* mas_for_left){
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

void net_toch_v_float(char* mas_for_left, flags flag){
  int k = 0;
  while (mas_for_left[k] != '.') {
    k++;
  }
  if (flag.reshetka) {
    mas_for_left[k + 1] = '\0';
  } else {
    mas_for_left[k] = '\0';
  }
}

void cpy_to_str(char* tmp_mas_for_round, char* mas_for_left, int dlina, int* flag_round){
  if (tmp_mas_for_round[0] == '0') {
    s21_strncpy(mas_for_left, tmp_mas_for_round + 1, dlina - 1);
    mas_for_left[dlina - 2] = '\0';
  } else {
    s21_strncpy(mas_for_left, tmp_mas_for_round, dlina);
    mas_for_left[dlina - 1] = '\0';
    *flag_round = 1;
  }
}

int float_to_string(long double number, char* mas_for_left, flags* flag, char chr, int* mantis) {
  long double intpart;
  int result;
  int tmp_tochnost = flag->tochnost;
  int flag_round = 0;
  int flag_bank = 0;
  char int_str[5000] = {0};
  char frac_str[1024] = {0};
  long double frac = modfl(number, &intpart);
  if (frac == 0.5L) {
    flag_bank = 1;
  }

  left_part_to_str(intpart, int_str, flag);
  right_part_to_str(frac, frac_str, flag->tochnost);

  char* tmp_mas_for_round =
      malloc(s21_strlen(int_str) + s21_strlen(frac_str) + 4);

  if(tmp_mas_for_round != S21_NULL){
    s21_strncpy(tmp_mas_for_round, "0\0", 2);
    s21_strncat(tmp_mas_for_round, int_str, s21_strlen(int_str) + 1);
    s21_strncat(tmp_mas_for_round, ".\0", 2);
    s21_strncat(tmp_mas_for_round, frac_str, s21_strlen(frac_str) + 1);
    tmp_mas_for_round[s21_strlen(int_str) + s21_strlen(frac_str) + 3] = '\0';
    int dlina = s21_strlen(tmp_mas_for_round);

    round_my(tmp_mas_for_round, dlina, flag_bank);

    tmp_mas_for_round[dlina - 1] = '\0';

    cpy_to_str(tmp_mas_for_round, mas_for_left, dlina, &flag_round);

    dlina = s21_strlen(mas_for_left);

    if (s21_strchr("eE", chr) && mas_for_left[2] == '.') {
      *mantis += 1;
      mas_for_left[2] = mas_for_left[1];
      mas_for_left[1] = '.';
      mas_for_left[dlina - 1] = '\0';
    }

    if (flag->isg && flag_round) {
      g_and_round(tmp_tochnost, mas_for_left);
    }

    if (!tmp_tochnost) {
      net_toch_v_float(mas_for_left, *flag);
    }

    result = s21_strlen(mas_for_left);
  }
  else{
    result = -1;
  }

  free(tmp_mas_for_round);
  return result;
}

void specificator_n(va_list* arg, char* buf) {
  int dlina = s21_strlen(buf);
  int* ptr = va_arg(*arg, int*);
  *ptr = dlina;
}

int specificator_p(va_list* arg, char* buf, flags flag) {
  void* ptr = va_arg(*arg, void*);
  int error = 0;
  unsigned long number = (unsigned long)ptr;
  flag.reshetka = 1;
  char* string = number_uxXo_to_string(number, flag, 16, 'x');
  if(string != S21_NULL){
    int dlina = s21_strlen(string);
    if (dlina < flag.width) {
      string = rabota_width(flag, string, dlina);
    }
    
    if(string != S21_NULL){
      s21_strncat(buf, string, s21_strlen(string) + 1);
      free(string);
    }
    else{
      error = 1;
    }
  }
  else{
    error = 1;
  }
  return error;
}

int specificator_c(flags flag, va_list* arg, char* buf) {
  int error = 0;
  char* string = calloc(2, sizeof(char));
  if (string != S21_NULL) {

  int value_int = va_arg(*arg, int);
  string[0] = value_int;
  string[1] = '\0';

  if (flag.width >= 2) {
    int dlina = s21_strlen(string);
    int raznica = flag.width - dlina;
    string = realloc(string, dlina + raznica + 2);
    if (string != S21_NULL) {
      s21_memset(string + dlina, 0, raznica + 2);
      if (flag.minus) {
        s21_memset(string + dlina, ' ', raznica);
      } else {
        char* tmp_mas = malloc(dlina + 1);
        if (tmp_mas != S21_NULL) {
          s21_strncpy(tmp_mas, string, dlina);
          tmp_mas[dlina] = '\0';
          s21_memset(string, ' ', raznica);
          string[raznica] = '\0';
          s21_strncat(string, tmp_mas, s21_strlen(tmp_mas));
          string[dlina + raznica] = '\0';
          free(tmp_mas);
        }
        else{
          error = 1;
        }
      }

      string[dlina + raznica + 1] = '\0';
    }
    else{
      error = 1;
    }
  }
  s21_strncat(buf, string, s21_strlen(string) + 1);
  free(string);
  }
  else{
    error = 1;
  }
  return error;
}

char* s_null(char* string, int* flag_null){
  string = malloc(8);
  if(string != S21_NULL){
    s21_strncpy(string, "(null)", 6);
    string[6] = '\0';
    *flag_null = 1;
  }
  return string;
}

char* s_width(char* string, int dlina, flags flag){
  int raznica = flag.width - dlina;
  string = realloc(string, dlina + raznica + 2);
  if (string != S21_NULL) {
    if (flag.minus) {
      s21_memset(string + dlina, ' ', raznica);  //
    } else {
      char* tmp_mas = malloc(dlina + 1);
      if (tmp_mas != S21_NULL) {
        s21_strncpy(tmp_mas, string, dlina);
        tmp_mas[dlina] = '\0';
        s21_memset(string, ' ', raznica);
        string[raznica] = '\0';
        s21_strncat(string, tmp_mas, s21_strlen(tmp_mas));
        free(tmp_mas);
      }
    }
    string[dlina + raznica] = '\0';
  }
  return string;
}

int specificator_s(flags flag, va_list* arg, char* buf) {
  char* string = S21_NULL;
  int flag_null = 0;
  int error = 0;
  
  const char* tmp = va_arg(*arg, const char*);
  if (tmp == NULL) {
    string = s_null(string, &flag_null);
  } else {
    string = malloc(s21_strlen(tmp) + 1);
    if(string != S21_NULL){
      s21_strncpy(string, tmp, s21_strlen(tmp));
      string[s21_strlen(tmp)] = '\0';
    }
  }
  
  if(string != S21_NULL){
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
      string = s_width(string, dlina, flag);
    }
    if(string != S21_NULL){
      s21_strncat(buf, string, s21_strlen(string) + 1);
      free(string);
    }
    else{
      error = 1;
    }
  }
  else{
    error = 1;
  }
  return error;
}

int specificator_uxXo(flags flag, va_list* arg, char* buf, char chr) {
  int error = 0;
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
  if(string != S21_NULL){
    int dlina = s21_strlen(string);
    if (dlina < flag.width) {
      string = rabota_width(flag, string, dlina);
    }

    if(string != S21_NULL){
      s21_strncat(buf, string, s21_strlen(string) + 1);
      free(string);
    }
    else{
      error = 1;
    }
  }
  else{
    error = 1;
  }
  return error;
}

char* number_uxXo_to_string(unsigned long number, flags flag, int base, char chr) {
  char* mas_for_number = malloc(64);
  char* result = S21_NULL;
  if (mas_for_number != S21_NULL) {
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

    reverse_str(mas_for_number);

    int dlina = s21_strlen(mas_for_number);

    if (flag.istochnost) {
      mas_for_number =
          rabota_tochnost(flag, number_copy == 0, dlina, mas_for_number, index);
    }

    if(mas_for_number != S21_NULL){
      dlina = s21_strlen(mas_for_number);

      if (flag.reshetka) {
        dlina = rabota_reshetka(dlina, base, mas_for_number, chr, number_copy);
      }

      result = malloc(dlina + 2);
      if (result != S21_NULL) {
        for (int i = 0; i < dlina; i++) {
          result[i] = mas_for_number[i];
        }
        result[dlina] = '\0';

        free(mas_for_number);
      }
    }
  }
  return result;
}

int rabota_reshetka(int dlina, int base, char* mas_for_number, char chr, unsigned long number) {
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
    mas_for_number[dlina] = '\0';
  }

  return dlina;
}

int specificator_di(flags flag, va_list* arg, char* buf) {
  int error = 0;
  long value;
  if (flag.h) {
    value = (short)va_arg(*arg, int);
  } else if (flag.l) {
    value = va_arg(*arg, long);
  } else {
    value = va_arg(*arg, int);
  }
  char* string = number_di_to_string(value, flag);
  if(string != S21_NULL){
    int dlina = s21_strlen(string);
    if (dlina < flag.width) {
      string = rabota_width(flag, string, dlina);
    }

    if(string != S21_NULL){
      s21_strncat(buf, string, s21_strlen(string) + 1);
      free(string);
    }
    else{
      error = 1;
    }
  }
  else{
    error = 1;
  }
  return error;
}

char* number_di_to_string(long number, flags flag) {
  long tmp_number = number;
  char* result = S21_NULL;
  int flag_min = 0;
  if (number == (-9223372036854775807 - 1)) {
    flag_min = 1;
    tmp_number = (unsigned long)9223372036854775807;
  } else {
    tmp_number = (number < 0) ? -number : number;
  }
  char* mas_for_number = malloc(20);
  if (mas_for_number != S21_NULL) {

    int index = 0;

    int dlina = long_to_string(mas_for_number, &index, tmp_number);

    if (flag_min) {
      mas_for_number[dlina - 1] = '8';
    }

    if (flag.istochnost) {
      mas_for_number =
          rabota_tochnost(flag, number == 0, dlina, mas_for_number, index);
    }

    if(mas_for_number != S21_NULL){
      dlina = s21_strlen(mas_for_number);
      result = zapolnenie_mas_result(dlina, number, flag, mas_for_number);
    }
  }
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

  reverse_str(mas_for_number);

  return dlina;
}

char* rabota_tochnost(flags flag, int zero, int dlina, char* mas_for_number, int index) {
  if (!flag.tochnost && zero) {
    s21_strncpy(mas_for_number, "\0", 1);
  } else if (dlina < flag.tochnost) {
    int count = dlina - 1;
    int raznica = flag.tochnost - dlina;
    mas_for_number = realloc(mas_for_number, dlina + raznica + 2);
    if (mas_for_number != S21_NULL) {
      while (count >= 0) {
        mas_for_number[count + raznica] = mas_for_number[count];
        count--;
      }
      for (int i = 0; i < raznica; i++) {
        mas_for_number[i] = '0';
      }

      mas_for_number[index + raznica] = '\0';
    }
  }
  return mas_for_number;
}

char* zapolnenie_mas_result(int dlina, long number, flags flag, char* mas_for_number) {
  char* result = malloc(dlina + 3);
  if (result != S21_NULL) {
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
  }
  return result;
}

char* rabota_width(flags flag, char* string, int dlina) {
  int raznica = flag.width - dlina;
  string = realloc(string, dlina + raznica + 3);
  if (string != S21_NULL) {
    if (flag.minus) {
      s21_memset(string + dlina, ' ', raznica);
      string[dlina + raznica] = '\0';
    } else {
      int znak = 0;
      if (s21_strchr("+- ", string[0]) && flag.zero) {
        znak++;
      }
      if (flag.reshetka && s21_strlen(string) > 1 &&
          s21_strchr("xX", string[1]) && flag.zero) {
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
  }
  return string;
}
