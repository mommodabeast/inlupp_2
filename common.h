#pragma once
#include <stdbool.h>


typedef union elem elem_t;
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);
typedef bool(*ioopm_predicate)(elem_t key, elem_t value, void *extra, ioopm_eq_function func);
typedef void(*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);


union elem {
  int number;
  unsigned int positive_number;
  bool boolean;
  float decimal;
  char *string;
  void *void_pointer;
  /// other choices certainly possible
};

/// @brief checks if two integers are equal
/// @param a first elem_t to compare
/// @param b second elem_t to compare
/// @return returns true if the integers in the elem_t are equal, false otherwise
bool ioopm_int_eq(elem_t a, elem_t b);

/// @brief checks if two strings are equal
/// @param a first elem_t to compare
/// @param b second elem_t to compare
/// @return returns true if the string elements in the elem_t are equal, false otherwise
bool ioopm_str_eq(elem_t a, elem_t b);

/// @brief checks if two floats are equal
/// @param a first elem_t to compare
/// @param b second elem_t to compare
/// @return returns true if the float values in the elem_t are equal, false otherwise
bool ioopm_float_eq(elem_t a, elem_t b);

/// @brief checks if two unsigned integers are equal
/// @param a first elem_t to compare
/// @param b second elem_t to compare
/// @return returns true if the unsigned integers in the elem_t are equal, false otherwise
bool ioopm_unsigned_eq(elem_t a, elem_t b);

/// @brief checks if two boolean values are equal
/// @param a first elem_t to compare
/// @param b second elem_t to compare
/// @return returns true if the boolean values in the elem_t are equal, false otherwise
bool ioopm_bool_eq(elem_t a, elem_t b);

/// @brief checks if two void pointers are equal
/// @param a first elem_t to compare
/// @param b second elem_t to compare
/// @return returns true if the void pointers in the elem_t are equal, false otherwise
bool ioopm_void_eq(elem_t a, elem_t b);

/// @brief hashing function from freq-count file (inlupp1)
/// @param e the key to hash
/// @return an integer that will be used to index in a hashtable
int string_sum_hash(elem_t e);
