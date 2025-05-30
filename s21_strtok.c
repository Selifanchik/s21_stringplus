#include "s21_string.h"

char *s21_strtok(char *str, const char *delim){

    static char *token_for_next;

    if(str!=S21_NULL){
        token_for_next=str;
    }
    
    char *token_start = S21_NULL;

//поставили указатель на первый символ, не совпадающий с delim, это будет начало токена
if(token_for_next!=S21_NULL){
while(*token_for_next!='\0' && s21_strchr(delim, *token_for_next)!=S21_NULL){
    token_for_next++;
}
}
//теперь это начало нашего токена
if (token_for_next!=S21_NULL && *token_for_next != '\0') {
   token_start = token_for_next;
    }
//можно заменить разделитель(delim) на \0 и переставлять указатель на начало следующего токена или NULL если строка закончена
if(token_start!=S21_NULL){
token_for_next = s21_strpbrk(token_for_next, delim);
if(token_for_next!=S21_NULL){
    *token_for_next='\0';
    token_for_next++;
}
}
return token_start;
}