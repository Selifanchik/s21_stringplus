#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H

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
  int isp;
} flags;

int parsing(const char* format, int* i, flags* flag, va_list* arg);
void pars_flags_dlina(char chr, flags* flag);
int pars_width_tochnost(const char* format, int* i);
int define_specificator(char chr, flags flag, va_list* arg, char* buf);

int specificator_di(flags flag, va_list* arg, char* buf);
char* number_di_to_string(long number, flags flag);
int long_to_string(char* mas_for_number, int* index, long number);
char* rabota_tochnost(flags flag, int zero, int dlina, char* mas_for_number,
                      int index);
char* zapolnenie_mas_result(int dlina, long number, flags flag,
                            const char* mas_for_number);
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
void left_part_to_str(long double number, char* str, flags* flag);
void right_part_to_str(long double right_part, char* str, int toch);
void g_and_round(int tmp_tochnost, char* mas_for_left);
void net_toch_v_float(char* mas_for_left, flags flag);
void cpy_to_str(char* tmp_mas_for_round, char* mas_for_left, int dlina,
                int* flag_round);
#endif
