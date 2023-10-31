#pragma once
#include "linked_list.h"
#include "common.h"

/**
 * @file hash_table.h
 * @author David Bagstevold, Hans-Eskil Ytterfelt-Martinsson
 * @date 1 Sep 2022
 * @brief Simple hash table that maps integer keys to string values.
 *
 * The hashtable (ht) is allocated on the heap and has many different functions
 * associated with it. A ht always needs to be created first, if not e.g. the
 * function ioopm_hash_table_insert gets called first, the program will crash
 * since there are no ht created on the heap. When a ht is created the user
 * need to indicate which eq_function to use (found in common.c). The user cannot
 * compare different types e.g. string and number, this will result in a crash as well.
 * The user also needs to always use the ioopm_hash_table_destroy to deallocate ht's 
 * memory, and this is done lastly. Other than that the user are free to use the
 * other functions in anyway they may like. 
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */

typedef struct hash_table ioopm_hash_table_t;
typedef struct entry entry_t;
typedef int(*hash_func)(elem_t key);



#define No_bucket 17

struct entry {
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table {
  bool(*ioopm_eq_function)(elem_t a, elem_t b);
  entry_t buckets[No_bucket];
  int(*hash_func)(elem_t key);
};



/// @brief Create a new hash table
/// @param eq_func is the comparison function between two elem_t
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function eq_func);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key and assoc. value to the given hashtable
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result pointer to a string that gets set to the value that key maps to with pointer semantics
/// @return a boolean indicating if lookup is successful
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);


/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key); 

/// @brief returns the number of key => value entries in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true if the ht is empty, otherwise false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table and returns all the entries memory
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return a list of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param ht hash table operated upon
/// @return a list of values sfor hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
/// @return true if the key exists in the ht, otherwise false
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
/// @return true if the value exists in the ht, otherwise false.
///         It returns the first occurence of the value.
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if all elements in ht satisfies predicate, false if not.
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if there exists one entry that satisfies the predicate, otherwise false.
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg); 

/// @brief checks if key is a positive integer
/// @param key the key of given entry
/// @param value_ignored value is ignored
/// @param not_used this param is not used
/// @return true if the key is equal or larger then zero
bool positive_key(elem_t key, elem_t value_ignored, void *not_used, ioopm_eq_function func);

/// @brief replaces the value of an entry with any string arg
/// @param key key of given entry, but does not change key in any way
/// @param value the value to be replaced
/// @param arg the string to replace value
void replace_string(elem_t key, elem_t *value, void *arg); 

/// @brief hashes a value according to a algorithm
/// @param key the key of type elem_t 
/// @return a number
int hashing_func(elem_t key);