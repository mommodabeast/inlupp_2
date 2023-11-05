#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

bool is_number(char *str)
{
int i = 0;

if (str[0] == '-')
{
if (str[1] == '\0')
return false;
//i = 1;
}

for (; str[i] != '\0'; i++)
{
if (!isdigit(str[i]))
{
return false;
}
}

return true;
}

char print(char *string) {

  char ord;

    for (int i = 0; i <= strlen(string); i++)
    {
      ord = putchar(string[i]); 
    }

return ord;
}

char printin(char *string) {

    char ord = print(string);

    return printf("%c\n", ord);

}

void clear_input_buffer()
{
int c;
do
{
  c = getchar();
}
while (c != '\n' && c != EOF);
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_function) atoi);
  return answer.int_value; // svaret som ett heltal
}

// int read_string(char *buf, int bufsize) {
    
//   scanf("%[^\n]", buf);

//   int counter = 0;
//   int length = strlen(buf);

//   if (length < bufsize - 1)
//   {
//     for(int i = 0; i < length; i++)
//       {
//         counter = counter + 1;
//       }
//   } else {
//     for (int j = 0; j < bufsize; j++)
//     {
//       counter = counter + 1;
//     }
//   }

//   clear_input_buffer();
//   buf[counter] = '\0';
//   return counter;
// }

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
  return ask_question(question, not_empty, (convert_function) strdup).string_value;
}

answer_t ask_question(char *question, check_function check, convert_function convert) {

char answer[256];
//char question[256];

while (1) {

  printf("%s ", question);
  read_string(answer, sizeof(answer));

  /*int input_length = read_string(answer, sizeof(answer));*/

  if (check(answer)) {
    answer_t convertedAnswer = convert(answer);
    return convertedAnswer;
  
  } else {
    printf("Invalid answer format. Try again\n");
  }

}


}

bool is_shelf(char *str) {
    int len = strlen(str);
    bool is_shlf = true;
    
    // ASCII value check if first letter is a capital letter and stringlength is more than one
    if (str[0] >= 65 && str[0] <= 90 && len > 1) {
        
        for (int i = 1; i < len; i = i + 1) {
            
            if (!isdigit(str[i])) {
                is_shlf = false;
                break;
            
            } else { }
        }
        
    } else {
        is_shlf = false;
    }
    
    return is_shlf;
}

char *ask_question_shelf(char *question)
{
  answer_t answer = ask_question(question, is_shelf, (convert_function) strdup);
  return answer.string_value; // svaret som ett heltal
}

bool empty_or_more(char *str)
{
  return true;
}

char *ask_question_any_input_string(char *question)
{
  return ask_question(question, empty_or_more, (convert_function) strdup).string_value;
}