#pragma once
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "common.h"

/**
 * @file linked_list.h
 * @author David Bagstevold, Hans-Eskil Ytterfelt-Martinsson
 * @date 10 Oct 2022
 * @brief Simple single-linked list, that stores a elem_t
 *
 * The linked list is allocated on the heap using the ioopm_linked_list_create function.
 * This step needs to be done first with a comparison function argument, in order to be able to use 
 * different functions to operate on the created linked list, such as inserting and removing values
 * from the list. Constructor functions like insert and append will allocate memory on the heap,
 * while destructors like destroy and remove will free memory from the heap, to avoid memory leakage.
 * 
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */

typedef struct link link_t; 

typedef struct list ioopm_list_t; /// Meta: struct definition goes in C file

typedef bool(*ioopm_int_predicate)(elem_t key, elem_t value, void *extra);
typedef void(*ioopm_apply_int_function)(elem_t key, elem_t *value, void *extra);

struct link {
    elem_t value; 
    link_t *next;
};

struct list {
    bool(*ioopm_eq_function)(elem_t a, elem_t b);
    link_t *head;
    link_t *last;
    size_t size; //Jag tänker att den pekar ju på ett element först och det är vår som håller koll på first/last
};

/// @brief Creates a new empty list
/// @param eq_func is the comparison function between two elem_t
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_func);

/// @brief Tear down the linked list and the elements and return all its memory
/// @param list the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time. If list is empty, a new link_t 
///        will be added to the start of the list, with both head and last pointers pointing at
///        the newly inserted link_t.
/// @param list the linked list that will be appended
/// @param value the value to be appended
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value);

/// @brief Insert at the front of a linked list in O(1) time. if the list is empty, a new 
///        link_t is created and the list->head becomes equal to list->last
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value);

/// @brief Insert an element into a linked list in O(n) time.
///        The valid values of index are [0,n] for a list of n elements,
///        where 0 means before the first element and n means after
///        the last element. If an index is greater than the size of list
///        it will be appended. 
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted 
void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value);

/// @brief Remove an element from a linked list in O(n) time.
///        The valid values of index are [0,n-1] for a list of n elements,
///        where 0 means the first element and n-1 means the last element.
///        the function uses an assert that raises an error if index is 
///        negative or greater than size of the linked list.
/// @param list the linked list
/// @param index the position in the list
/// @return the value removed
elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index);

/// @brief Retrieve an element from a linked list in O(n) time.
///        The valid values of index are [0,n-1] for a list of n elements,
///        where 0 means the first element and n-1 means the last element.
///        if the index is out of scope an assert will be raised.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index);

/// @brief Test if an element is in the list
/// @param list the linked list
/// @param element the element sought
/// @return true if element is in the list, else false
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t ioopm_linked_list_size(ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements in the list is 0, else false
bool ioopm_linked_list_is_empty(ioopm_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list the linked list
void ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
///        The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, void *extra);

/// @brief Test if a supplied property holds for any element in a list.
///        The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, void *extra);

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of fun
void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function fun, void *extra);

/// @brief checks if value is an even number
/// @param key_ignored key is not used in this function 
/// @param value the value to check if it is an even number
/// @param extra_ignored this param is not used
bool is_even(elem_t key_ignored, elem_t value, void *extra_ignored);

/// @brief replaces a value with given int
/// @param key_ignored key is not used
/// @param value the value to be replaced
/// @param extra the number to replace value with
void replace(elem_t key_ignored, elem_t *value, void *extra);