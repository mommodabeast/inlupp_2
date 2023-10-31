#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"


// bool ioopm_check_eq(elem_t a, elem_t b) {
//     if (a.number == b.number) {
//         return true;
//     } else if (a.positive_number == b.positive_number) {
//         return true;    
//     } else if (a.boolean == b.boolean){
//         return true;
//     } else if (a.decimal == b.decimal) {
//         return true;
//     } else if (strcmp(a.string, b.string) == 0) {
//         return true;
//     } else if (a.void_pointer == b.void_pointer) {
//         return true;s
//     } else {
//         return false;
//     }    
// }

bool ioopm_int_eq(elem_t a, elem_t b) {
    return (a.number == b.number);
}

bool ioopm_str_eq(elem_t a, elem_t b) {
    if (strcmp(a.string, b.string) == 0) {
        return true;
    } else {
        return false;
    }
}

bool ioopm_float_eq(elem_t a, elem_t b) {
    return (a.decimal == b.decimal);
}

bool ioopm_unsigned_eq(elem_t a, elem_t b) {
    return (a.positive_number == b.positive_number);
}

bool ioopm_bool_eq(elem_t a, elem_t b) {
    return (a.boolean == b.boolean);
}

bool ioopm_void_eq(elem_t a, elem_t b) {
    return (a.void_pointer == b.void_pointer);
}

int string_sum_hash(elem_t e) {
    char *str = e.void_pointer;
    int result = 0;
    do
{
result += *str;
}
while (*++str != '\0');
    return result;
}

