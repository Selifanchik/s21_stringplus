#include <stdarg.h>
#include <stdio.h>
#include <string.h>//потом убрать и использовать только наши функ.

typedef struct{
    int minus;
    int plus;
    int space;
    int reshetka;
    int zero;
    int width;
    int tochnost;
    int h;
    int l;
    int L;
} flags;

void parsing(const char *format, int *i, flags *flag, va_list *arg);
void pars_flags_dlina(char chr, flags *flag);
int pars_width_tochnost(const char *format, int *i);

int s21_sprintf(char *str, const char *format, ...){
    printf("!%s!\n", format);//тест, можно убрать
    str[0] = 1;//пока чтобы не ругался при компиляции
    
    flags flag = {0};
    va_list arg;             // указатель на параметр
    va_start(arg, format);   // получение адреса первого вариадического параметра (то есть ссылка на последний не варидлический)
   
    //%[флаги][ширина][.точность][длина]
    //идем по форматной строке
    for(int i = 0; i < (int)strlen(format); i++){//заменить потом strlen на нашу //также использовать наш size_t
        if(format[i] != '%'){//пока не спецификатор просто печатаем
            printf("%c", format[i]);
            continue;
        }
        i++;
        if(format[i] == '%'){//если у нас "%%"" то просто печатаем и продолжаем
            printf("%c", format[i]);
            continue;
        }

        memset(&flag, 0, sizeof(flags));//обнуление структуры //заменить потом на наш memset

        parsing(format, &i, &flag, &arg);

        //печатаем для теста
        printf("flags:\n-:%d\n+:%d\n :%d\n#:%d\n0:%d\nwidth:%d\ntoch:%d\nh:%d\nl:%d\nL:%d\n", flag.minus, flag.plus, flag.space, flag.reshetka, flag.zero, flag.width, flag.tochnost, flag.h, flag.l, flag.L);
        printf("\nspec:%c\n", format[i]);

        //
        //тут у нас format[i] ссылается на спецификатор. Также готова структурка(содержит все вкл флаги, ширину, точность и тд).
        //
        

        /*i++;//пока не трогать
        switch(format[i]){
            case 'd': 
                printf("%d", va_arg(arg, int));
                break;
            case 'f': 
                printf("%f", va_arg(arg, double));
                break;
            case 's': 
                printf("%s", va_arg(arg, const char*));
                break;
            case 'c': 
                printf("%c", va_arg(arg, int));
                break;
            default:
                printf("%c", format[i]);
        }*/

    }
    va_end(arg); // завершение процедуры перебора вариадических параметров

    return 1;
}

void parsing(const char *format, int *i, flags *flag, va_list *arg){
    while(format[*i] && !strchr("cdieEfgGosuxXpn", format[*i])){//добавить остальные спецификаторы   //пока не спецификатор идем посимвольно
        
        if(strchr("-+ #0", format[*i])){
            pars_flags_dlina(format[*i], flag);
        }

        else if(strchr("123456789*", format[*i])){
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
            }
            else{
                flag->tochnost = pars_width_tochnost(format, i);
                continue;
            }
        }

        if(strchr("lhL", format[*i])){
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
    while(strchr("0123456789", format[*i])){
        sum = sum * 10 + (format[*i] - '0');
        (*i)++;
    }
    return sum;
}


int main(){
    char buf[100];
    s21_sprintf(buf, "%+- #056.65432Ld%*.*d", 8, 9, 4, 2);

    //sprintf(buf, "D:%d, sym:%c, float:%f, str:%s, d=%d", 1, 'c', 1.2, "asd", 10);
    //sprintf(buf, "%--+++d", 1);
    //printf("\n%s", buf);

    return 0;
}