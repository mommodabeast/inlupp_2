#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

extern char *strdup(const char *);

typedef union { 
  int   int_value;
  float float_value;
  char *string_value;
} answer_t;
typedef bool(*check_function)(char *answer);
typedef answer_t(*convert_function)(char *answer);


bool is_number(char *str);
int read_string(char *buf, int buf_siz);
bool not_empty(char *str);
bool is_number(char *str);
int ask_question_int(char *question);
//int ask_question_int(char *question);
//char *ask_question_string(char *question, char *buf, int buf_siz);
void clear_input_buffer();
char print(char *string);
char printin(char *string);
answer_t ask_question(char *question, check_function check, convert_function convert);
char *ask_question_string(char *question);

bool is_shelf(char *string);
char *ask_question_shelf(char *question);
bool empty_or_more(char *str);
char *ask_question_any_input_string(char *question);

#endif 