#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>//потом убрать и использовать только наши функ.(в коде много оригинальных ЗАМЕНИТЬ!!!)

//для lc, ls
#include <wchar.h>
#include <locale.h>

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
char* number_di_to_string(long number, flags flag);
int long_to_string(char* mas_for_number, int *index, long number);
int rabota_tochnost(flags flag, int number, int dlina, char* mas_for_number, int index);
char* zapolnenie_mas_result(int dlina, long number, flags flag, char* mas_for_number);
char* rabota_width(flags flag, char* string, int dlina);


void specificator_uxXo(flags flag, va_list *arg, char *buf, char chr);
char* number_uxXo_to_string(unsigned long number, flags flag, int base, char chr);
int rabota_reshetka(int dlina, int base, char* mas_for_number, char chr);

void specificator_c(flags flag, va_list *arg, char* buf);
void specificator_s(flags flag, va_list *arg, char* buf);
int tochnost_ls(wchar_t* value, flags flag);

void specificator_n(va_list *arg, char* buf);
void specificator_p(va_list *arg, char* buf, flags flag);

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
            specificator_s(flag, arg, buf);
            break;
        case 'c': 
            specificator_c(flag, arg, buf);
            break;
        case 'f': 
            //printf("%f", va_arg(arg, double));
            break;
        case 'e': 
            //printf("%f", va_arg(arg, double));
            break;
        case 'E': 
            //printf("%f", va_arg(arg, double));
            break;
        case 'g': 
            //printf("%f", va_arg(arg, double));
            break;
        case 'G': 
            //printf("%f", va_arg(arg, double));
            break;
        case 'n': 
            specificator_n(arg, buf);
            break;
        case 'p': 
            specificator_p(arg, buf, flag);
            break;

        //default:
            //printf("%c", format[i]);
    }
}

void specificator_n(va_list *arg, char* buf){
    int dlina = strlen(buf);
    int *ptr = va_arg(*arg, int*);
    *ptr = dlina;
}

void specificator_p(va_list *arg, char* buf, flags flag){
    void *ptr = va_arg(*arg, void*);
    unsigned long number = (unsigned long)ptr;
    flag.reshetka = 1;
    char* string = number_uxXo_to_string(number, flag, 16, 'x');
    int dlina = (int)strlen(string); //поменять на size_t нашу
    if(dlina < flag.width){
        string = rabota_width(flag, string, dlina);
    }
    strcat(buf, string);
    free(string);
}

void specificator_c(flags flag, va_list *arg, char* buf){
    char* string = malloc(MB_CUR_MAX + 1);
    if(flag.l){
        wchar_t value = va_arg(*arg, wchar_t);
        int len = wcrtomb(string, value, NULL);
        string[len] = '\0';
    }
    else{
        int value_int = va_arg(*arg, int);
        string[0] = value_int;
        string[1] = '\0';
    }

    if(flag.width >= 2){
        int dlina = strlen(string);
        int raznica = flag.width - dlina;
        string = realloc(string, dlina + raznica + 1);
        if(flag.minus){
            memset(string + dlina, ' ', raznica);
        }
        else{
            char* tmp_mas = malloc(dlina);
            strcpy(tmp_mas, string);
            memset(string, ' ', raznica);
            string[raznica] = '\0';
            strcat(string, tmp_mas);
            free(tmp_mas);
        }
        string[dlina + raznica + 1] = '\0';
    }
    strcat(buf, string);
    free(string);
}

void specificator_s(flags flag, va_list *arg, char* buf){
    char* string = NULL;
    if(flag.l){
        wchar_t* value = va_arg(*arg, wchar_t*);
        int len = wcstombs(NULL, value, 0);
        if(flag.istochnost && flag.tochnost < len){
            len = tochnost_ls(value, flag);
        }
        string = malloc(len + 1);
        wcstombs(string, value, len);
        string[len] = '\0';
    }
    else{
        const char* tmp = va_arg(*arg, const char*);
        string = malloc(strlen(tmp) + 1);
        strcpy(string, tmp);
        string[strlen(tmp)] = '\0';
    }

    int dlina = strlen(string);
    if(flag.istochnost && !flag.l){
        if(flag.tochnost < dlina){
            string[flag.tochnost] = '\0';
        }
    }

    dlina = strlen(string);
    if(flag.width && dlina < flag.width){
        int raznica = flag.width - dlina;
        string = realloc(string, dlina + raznica + 1);
        if(flag.minus){
            memset(string + dlina, ' ', raznica);//
        }
        else{
            char* tmp_mas = malloc(dlina);
            strcpy(tmp_mas, string);
            memset(string, ' ', raznica);
            string[raznica] = '\0';
            strcat(string, tmp_mas);
            free(tmp_mas);
        }
        string[dlina + raznica] = '\0';
    }
    strcat(buf, string);
    free(string);
}

int tochnost_ls(wchar_t* value, flags flag){
    char* string = malloc(MB_CUR_MAX);
    int dlina = 0;
    int i = 0;
    while(value[i] != '\0'){
        int len = wcrtomb(string, value[i], NULL);
        if(dlina + len > flag.tochnost){
            break;
        }
        dlina += len;
        i++;
    }
    return dlina;
}

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
        string = number_uxXo_to_string(value, flag, 8, chr);
    }
    else if(chr == 'x' || chr == 'X'){
        string = number_uxXo_to_string(value, flag, 16, chr);
    }
    else{//u
        string = number_uxXo_to_string(value, flag, 10, chr);
    }

    int dlina = (int)strlen(string); //поменять на size_t нашу
    if(dlina < flag.width){
        string = rabota_width(flag, string, dlina);
    }
    strcat(buf, string);
    free(string);
}

char* number_uxXo_to_string(unsigned long number, flags flag, int base, char chr){
    char mas_for_number[64];
    int index = 0;
    unsigned long number_copy = number;
    do{
        int number_tmp = number % base;
        if(base == 16){
            mas_for_number[index] = (char)((number_tmp < 10) ? number_tmp + '0' : ((chr == 'x') ? number_tmp - 10 + 'a' : number_tmp - 10 + 'A'));
        }
        else{
            mas_for_number[index] = (char)(number_tmp + '0');
        }
        number /= base;
        index++;
    } while(number != 0);

    mas_for_number[index] = '\0';

    int dlina = strlen(mas_for_number);

    for(int i = 0; i < index / 2; i++){
        int tmp1 = mas_for_number[i];
        mas_for_number[i] = mas_for_number[dlina - i - 1];
        mas_for_number[dlina - i - 1] = tmp1;
    }

    if(flag.istochnost){
        dlina = rabota_tochnost(flag, number_copy == 0, dlina, mas_for_number, index);//подумать может быть нужно сделать точность отдельно
    }

    if(flag.reshetka && chr != 'u'){
        dlina = rabota_reshetka(dlina, base, mas_for_number, chr);
    }

    char* result = malloc(dlina + 1);
    for(int i = 0; i < dlina; i++){
        result[i] = mas_for_number[i];
    }
    result[dlina] = '\0';

    return result;
}

int rabota_reshetka(int dlina, int base, char* mas_for_number, char chr){
    int flag = 1;
    if(base == 8 && mas_for_number[0] == '0'){
        flag = 0;
    }
    if(flag){
        int count = dlina - 1;
        int raznica = base == 8 ? 1 : 2;
        while(count >= 0){
            mas_for_number[count + raznica] = mas_for_number[count];
            count--;
        }
        if(base == 16){
            mas_for_number[1] = chr == 'x' ? 'x' : 'X';
            mas_for_number[0] = '0';
            dlina += 2;
        }
        else{
            mas_for_number[0] = '0';
            dlina += 1;
        }
    }

    return dlina;
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
    char* string = number_di_to_string(value, flag);
    int dlina = (int)strlen(string); //поменять на size_t нашу
    if(dlina < flag.width){
        string = rabota_width(flag, string, dlina);
    }
    strcat(buf, string);
    free(string);
}

char* number_di_to_string(long number, flags flag){
    long tmp_number = number;
    if(tmp_number < 0) tmp_number = tmp_number * -1;
    char mas_for_number[20];
    int index = 0;
    
    int dlina = long_to_string(mas_for_number, &index, tmp_number);

    if(flag.istochnost){
        dlina = rabota_tochnost(flag, number == 0, dlina, mas_for_number, index);
    }

    char* result = zapolnenie_mas_result(dlina, number, flag, mas_for_number);

    return result;
}

int long_to_string(char* mas_for_number, int *index, long number){
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

int rabota_tochnost(flags flag, int zero, int dlina, char* mas_for_number, int index){
    if(flag.istochnost && !flag.tochnost && zero){
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

char* rabota_width(flags flag, char* string, int dlina){
    int raznica = flag.width - dlina;
    string = realloc(string, dlina + raznica + 1);//подумать если вдруг будет NULL
    if(flag.minus){
        memset(string + dlina, ' ', raznica);//
    }
    else{
        int znak = 0;
        if(strchr("+-", string[0]) && flag.zero && !flag.istochnost){
            znak++;
        }
        int count = dlina - 1;
        while(count >= znak){
            string[count + raznica] = string[count];
            count--;
        }
        memset(string + znak, (flag.zero && !flag.istochnost) ? '0' : ' ', raznica);
    }
    return string;
}

int main(){
    setlocale(LC_ALL, "C.UTF-8"); //для lc, ls
    char buf1[1024], buf2[1024];
    //char ch[5] = "asdf";
    //wchar_t ch = L'г';
    wchar_t *str = L"開при";
    
    //int a;

    sprintf(buf1, "!%-6.5ls!", str);
    s21_sprintf(buf2, "!%-6.5ls!", str);

    printf("%s\n", buf1);
    printf("%s\n", buf2);
    //printf("%d,%d\n", a, b);
    printf("%d", strcmp(buf1, buf2));
    return 0;
}
