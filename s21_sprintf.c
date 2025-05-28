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
char* numbertostring(long number, flags flag);
void specificator_uxXo(flags flag, va_list *arg, char *buf, char chr);
char* rabota_width(flags flag, char* string, int dlina);
int dec_to_string(char* mas_for_number, int *index, long number);
char* dec_to_oct(unsigned long number, flags flag);
int rabota_tochnost(flags flag, long number, int dlina, char* mas_for_number, int index);
char* dec_to_hex(unsigned long number, flags flag, char chr);

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
            //specificator_s();
            //printf("%s", va_arg(arg, const char*));
            break;
        case 'c': 
            //specificator_c();
            //printf("%c", va_arg(arg, int));
            break;
        case 'f': 
            //printf("%f", va_arg(arg, double));
            break;
        //default:
            //printf("%c", format[i]);
    }
}

/*void specificator_c(){
    long value;
    if(flag.l){
        value = va_arg(*arg, long);
    }
    else{
        value = va_arg(*arg, int);
    }
}*/

void specificator_uxXo(flags flag, va_list *arg, char *buf, char chr){
    unsigned long value;
    if(flag.h){
        value = (unsigned short)va_arg(*arg, unsigned int);
    }
    else if(flag.l){
        value = va_arg(*arg, unsigned long);
    }
    else{
        value = va_arg(*arg, unsigned int);
    }

    char* string;
    if(chr == 'o'){
        string = dec_to_oct(value, flag);
    }
    else if(chr == 'x' || chr == 'X'){
        string = dec_to_hex(value, flag, chr);
    }
    else{//u
        string = numbertostring(value, flag);
    }
    int dlina = (int)strlen(string); //поменять на size_t нашу
    if(dlina < flag.width){
        string = rabota_width(flag, string, dlina);
    }
    strcat(buf, string);
    free(string);
}

char* dec_to_hex(unsigned long number, flags flag, char chr){
    char mas_for_number[64];
    int index = 0;
    do{
        int number_tmp = number % 16;
        mas_for_number[index] = (char)((number_tmp < 10) ? number_tmp + '0' : ((chr == 'x') ? number_tmp - 10 + 'a' : number_tmp - 10 + 'A'));
        number /= 16;
        index++;
    } while(number != 0);

    int dlina = strlen(mas_for_number);

    for(int i = 0; i < index / 2; i++){
        int tmp1 = mas_for_number[i];
        mas_for_number[i] = mas_for_number[dlina - i - 1];
        mas_for_number[dlina - i - 1] = tmp1;
    }

    if(flag.istochnost){
        dlina = rabota_tochnost(flag, number, dlina, mas_for_number, index);
    }

    if(flag.reshetka){
        int count = dlina - 1;
        int raznica = 2;
        while(count >= 0){
            mas_for_number[count + raznica] = mas_for_number[count];
            count--;
        }
        mas_for_number[1] = chr == 'x' ? 'x' : 'X';
        mas_for_number[0] = '0';
        dlina += 2;
    }

    char* result = malloc(dlina + 1);
    for(int i = 0; i < dlina; i++){
        result[i] = mas_for_number[i];
    }
    result[dlina] = '\0';
    //printf("!%s!%d", mas_for_number, flag.l);
    return result;
}

char* dec_to_oct(unsigned long number, flags flag){
    char mas_for_number[64];
    int index = 0;
    do{
        mas_for_number[index] = (char)((number % 8) + '0');
        number /= 8;
        index++;
    } while(number != 0);

    if(flag.reshetka){//переместить это ниже, после обработки точности
        mas_for_number[index] = '0';
        index++;
    }

    int dlina = strlen(mas_for_number);

    for(int i = 0; i < index / 2; i++){
        int tmp1 = mas_for_number[i];
        mas_for_number[i] = mas_for_number[dlina - i - 1];
        mas_for_number[dlina - i - 1] = tmp1;
    }

    if(flag.istochnost){
        dlina = rabota_tochnost(flag, number, dlina, mas_for_number, index);
    }

    char* result = malloc(dlina + 1);
    for(int i = 0; i < dlina; i++){
        result[i] = mas_for_number[i];
    }
    result[dlina] = '\0';
    //printf("!%s!%d", mas_for_number, flag.l);
    return result;
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
    char* string = numbertostring(value, flag);
    int dlina = (int)strlen(string); //поменять на size_t нашу
    if(dlina < flag.width){
        string = rabota_width(flag, string, dlina);
    }
    strcat(buf, string);
    free(string);
}

char* rabota_width(flags flag, char* string, int dlina){
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
    return string;
}

int rabota_tochnost(flags flag, long number, int dlina, char* mas_for_number, int index){
    if(!flag.tochnost && number == 0){
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
    return dlina;
}

int dec_to_string(char* mas_for_number, int *index, long number){
    do{
        mas_for_number[*index] = (char)((number % 10) + '0');
        number /= 10;
        (*index)++;
    } while(number != 0);
    mas_for_number[*index] = '\0';
    int dlina = strlen(mas_for_number);//поменять на наш size_t

    for(int i = 0; i < *index / 2; i++){
        int tmp1 = mas_for_number[i];
        mas_for_number[i] = mas_for_number[dlina - i - 1];
        mas_for_number[dlina - i - 1] = tmp1;
    }

    return dlina;
}

char* zapolnenie_mas_result(int dlina, long number, flags flag, char* mas_for_number){
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

char* numbertostring(long number, flags flag){//не очень хорошо с unsighned long пересмотреть работа
    long tmp_number = number;
    if(tmp_number < 0) tmp_number = tmp_number * -1;
    char mas_for_number[20];
    int index = 0;
    
    int dlina = dec_to_string(mas_for_number, &index, tmp_number);

    if(flag.istochnost){
        dlina = rabota_tochnost(flag, number, dlina, mas_for_number, index);
    }

    char* result = zapolnenie_mas_result(dlina, number, flag, mas_for_number);

    return result;
}

int main(){
    char buf[100];
    //s21_sprintf(buf, "%+5.5d asdjhlfjkvh %.0i Hjhs^sh %%", 42, 30);
    s21_sprintf(buf, "%#6.4X", 255);
    printf("%s\n", buf);

    char buf1[100];
    //sprintf(buf1, "%+5.5d asdjhlfjkvh %.0i Hjhs^sh %%", 42, 30);
    sprintf(buf1, "%#6.4X", 255);
    printf("%s\n", buf1);

    printf("%d", strcmp(buf, buf1));//тест

    return 0;
}