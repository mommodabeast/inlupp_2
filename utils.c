#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func check, convert_func convert) {
bool a = false;
int buff_size = 255;
char buff[buff_size];
while(a == false) {
    printf("%s", question);
    read_string(buff, buff_size);
    if(check(buff)) {
        a = true;
    } else {

    }
}
return convert(buff);
}

bool is_number(char *str) {
int length = strlen(str);
for(int i = 0; i < length; i++) {
    if(isdigit(str[i])) {
        if(i == length-1) {
            return true;
        } else {
          
        }
    } 
    else if(str[i] == '-' && i == 0){
        
    } else {
        return false;
    }
}
return false;
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.int_value; 
}

int read_string(char *buf, int buf_siz) {
    char character = getchar();
    int counter = 0;
    while (character != '\n' && counter <= buf_siz-1) {
        buf[counter] = character;
        counter++;
        character = getchar();
    }
    buf[counter] = '\0';
    return counter;
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

void print(char *str) {
  int length = strlen(str);
  for(int i = 0; i < length; i++) {
    putchar(str[i]);
  }
}

void println(char *str) {
  print(str);
  putchar('\n');
}

bool is_shelf(char *str) {
int length = strlen(str);
for(int i = 0; i < length; i++) {
    if(isdigit(str[i]) && i != 0) {
        if(i == length-1) {
            return true;
        } else {
          
        }
    } 
    else if(isalpha(str[i]) && !isdigit(str[i]) && i == 0){
        
    } else {
        return false;
    }
}
return false;
}

char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}