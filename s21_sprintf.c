#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>//потом убрать и использовать только наши функ.(в коде много оригинальных ЗАМЕНИТЬ!!!)

typedef struct{
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
} flags;

void parsing(const char *format, int *i, flags *flag, va_list *arg);
void pars_flags_dlina(char chr, flags *flag);
int pars_width_tochnost(const char *format, int *i);
void define_specificator(char chr, flags flag, va_list *arg, char* buf);
void specificator_di(flags flag, va_list *arg, char* buf);
char* longtostring(long number, flags flag);

int s21_sprintf(char *str, const char *format, ...){
    flags flag = {0};
    va_list arg;             // указатель на параметр
    va_start(arg, format);   // получение адреса первого вариадического параметра (то есть ссылка на последний не варидлический)
    strcpy(str, "\0");
   
    //%[флаги][ширина][.точность][длина]
    //идем по форматной строке
    for(int i = 0; i < (int)strlen(format); i++){//заменить потом strlen на нашу //также использовать наш size_t
        if(format[i] != '%'){//пока не спецификатор просто печатаем
            char tmp[2] = {format[i], '\0'};
            strcat(str, tmp);
            continue;
        }
        i++;
        if(format[i] == '%'){//если у нас "%%"" то просто печатаем и продолжаем
            char tmp[2] = {format[i], '\0'};
            strcat(str, tmp);
            continue;
        }

        memset(&flag, 0, sizeof(flags));//обнуление структуры //заменить потом на наш memset

        parsing(format, &i, &flag, &arg);

        //печатаем для теста
        //printf("flags:\n-:%d\n+:%d\n :%d\n#:%d\n0:%d\nwidth:%d\ntoch:%d\nh:%d\nl:%d\nL:%d\n", flag.minus, flag.plus, flag.space, flag.reshetka, flag.zero, flag.width, flag.tochnost, flag.h, flag.l, flag.L);
        //printf("spec:%c\n", format[i]);

        define_specificator(format[i], flag, &arg, str);

    }
    va_end(arg); // завершение процедуры перебора вариадических параметров
    printf("%s", str);

    return 1;
}

void parsing(const char *format, int *i, flags *flag, va_list *arg){
    while(format[*i] && !strchr("cdieEfgGosuxXpn", format[*i])){//добавить остальные спецификаторы   //пока не спецификатор идем посимвольно
        
        if(strchr("-+ #0", format[*i])){//заменить на наш
            pars_flags_dlina(format[*i], flag);
        }

        else if(strchr("123456789*", format[*i])){//заменить на наш
            if(format[*i] == '*'){
                flag->width = va_arg(*arg, int);
            }
            else{
                flag->width = pars_width_tochnost(format, i);
                continue;
            }
        }

        else if(format[*i] == '.'){
            (*i)++;
            if(format[*i] == '*'){
                flag->tochnost = va_arg(*arg, int);
                flag->istochnost = 1;
            }
            else{
                flag->tochnost = pars_width_tochnost(format, i);
                flag->istochnost = 1;
                continue;
            }
        }

        if(strchr("lhL", format[*i])){//заменить на наш
            pars_flags_dlina(format[*i], flag);
        }
        
        (*i)++;
    }
}

void pars_flags_dlina(char chr, flags *flag){
    switch(chr){
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
        case 'L': 
            flag->L = 1;
            break;
    }
}

int pars_width_tochnost(const char *format, int *i){
    int sum = 0;
    while(strchr("0123456789", format[*i])){//заменить на наш
        sum = sum * 10 + (format[*i] - '0');
        (*i)++;
    }
    return sum;
}

void define_specificator(char chr, flags flag, va_list *arg, char* buf){
    switch(chr){
        case 'd': 
            specificator_di(flag, arg, buf);
            break;
        case 'i': 
            specificator_di(flag, arg, buf);
            break;
        case 'f': 
            //printf("%f", va_arg(arg, double));
            break;
        case 's': 
            //printf("%s", va_arg(arg, const char*));
            break;
        case 'c': 
            //printf("%c", va_arg(arg, int));
            break;
        //default:
            //printf("%c", format[i]);
    }
}

void specificator_di(flags flag, va_list *arg, char *buf){
    long value;
    if(flag.h){
        value = (short)va_arg(*arg, int);
    }
    else if(flag.l){
        value = va_arg(*arg, long);
    }
    else{
        value = va_arg(*arg, int);
    }
    char* string = longtostring(value, flag);
    int dlina = (int)strlen(string); //поменять на size_t нашу
    if(dlina < flag.width){
        int raznica = flag.width - dlina;
        string = realloc(string, dlina + raznica);//подумать если вдруг будет NULL
        if(flag.minus){
            memset(string + dlina, ' ', raznica);//
        }
        else{
            int znak = 0;
            if(strchr("+-", string[0]) && flag.zero && !flag.tochnost){
                znak++;
            }
            int count = dlina - 1;
            while(count >= znak){
                string[count + raznica] = string[count];
                count--;
            }
            memset(string + znak, flag.zero && !flag.tochnost ? '0' : ' ', raznica);
        }
    }
    strcat(buf, string);
    free(string);
}

char* longtostring(long number, flags flag){//уменить функцию. Можешь будет что то общее с x, X, o, u
    int tmp_number = number;
    if(tmp_number < 0) tmp_number = tmp_number * -1;
    char mas_for_number[20];
    int index = 0;
    do{
        mas_for_number[index] = (char)((tmp_number % 10) + '0');
        tmp_number /= 10;
        index++;
    } while(tmp_number != 0);
    mas_for_number[index] = '\0';
    int dlina = strlen(mas_for_number);//поменять на наш size_t

    for(int i = 0; i < index / 2; i++){
        int tmp1 = mas_for_number[i];
        mas_for_number[i] = mas_for_number[dlina - i - 1];
        mas_for_number[dlina - i - 1] = tmp1;
    }

    if(flag.istochnost && !flag.tochnost && number == 0){
        strcpy(mas_for_number, "\0");
        dlina = strlen(mas_for_number);
    }
    else if(flag.istochnost && dlina < flag.tochnost){
        int count = dlina - 1;
        int raznica = flag.tochnost - dlina;
        while(count >= 0){
            mas_for_number[count + raznica] = mas_for_number[count];
            count--;
        }
        for(int i = 0; i < raznica; i++){
            mas_for_number[i] = '0';
        }

        mas_for_number[index + raznica] = '\0';
        dlina = strlen(mas_for_number);
    }

    char* result = malloc(dlina + 2);
    int flag_0 = 0;
    if(number < 0){
        result[0] = '-';
        flag_0++;
    }
    else if(number >= 0 && flag.plus){
        result[0] = '+';
        flag_0++;
    }
    else if(number > 0 && flag.space){
        result[0] = ' ';
        flag_0++;
    }

    for(int i = 0; i < dlina; i++){
        result[i + flag_0] = mas_for_number[i];
    }
    result[dlina + flag_0] = '\0';

    return result;
}

int main(){
    char buf[100];
    s21_sprintf(buf, "%+5.5d asdjhlfjkvh %.0i Hjhs^sh %%", 42, 30);

    char buf1[100];
    sprintf(buf1, "%+5.5d asdjhlfjkvh %.0i Hjhs^sh %%", 42, 30);
    printf("\n%s", buf1);

    printf("\n%d", strcmp(buf, buf1));//тест

    return 0;
}