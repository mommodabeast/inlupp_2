#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"
#include "hash_table.h"
#include "common.h"

#define _XOPEN_SOURCE 700

int hashing_func (elem_t key) {
  return key.number;
}


ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function eq_func) {
  /// Allocate space for a ioopm_hash_table_t = No_bucket pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->hash_func = hashing_func;
  result->ioopm_eq_function = eq_func;
  return result;
}


static size_t count_numbers_of_items(entry_t *entry) {
  size_t number_of_elements = 0;

  while (entry->next != NULL) {
    number_of_elements++;
    entry = entry->next;
  }

  return number_of_elements;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht) {
  size_t ht_size = 0;

  for (size_t i = 0; i < No_bucket; i++) {
    if (ht->buckets[i].next != NULL) {
      ht_size += count_numbers_of_items(&ht->buckets[i]);
    }
  }
  return ht_size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht) {
  
  for (size_t i = 0; i < No_bucket; i++)
    if (ht->buckets[i].next != NULL) {
      return false;
    }
  return true;
}

static void entry_destroy(entry_t *entry) {
  if(entry->next == NULL) {
    free(entry);
  } else {
    entry_destroy((entry->next));
    free(entry);
  }
}
static void clean_up_ht(ioopm_hash_table_t *ht) {
    size_t index = 0;
  while(index < No_bucket) {
    if(ht->buckets[index].next == NULL) {
      index++;
    } else {
      entry_destroy(ht->buckets[index].next);
      ht->buckets[index].next = NULL;
      index++;
    }
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  clean_up_ht(ht);
  free(ht);
}

static entry_t *find_previous_entry_for_key(entry_t *entry, elem_t key, ioopm_hash_table_t *ht) {
  entry_t *current_entry = entry;
  while(current_entry->next != NULL) {
    if(ht->ioopm_eq_function(current_entry->next->key, key)) {
      return current_entry;
    } else {
      current_entry = current_entry->next;
    }
  }
  return current_entry;
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result) { 
    /// Find the previous entry for key
    int hashed_key = ht->hash_func(key);
    size_t index = abs(hashed_key % No_bucket); 
    entry_t *tmp = find_previous_entry_for_key(&ht->buckets[index], key, ht);
    entry_t *next = tmp->next;
    if ((next != NULL)) {
      *result = next->value;
      return true;
    } else {
      return false;
    }
  }



static entry_t *entry_create(elem_t key, elem_t value, entry_t *next) {
  entry_t *new_entry = calloc(1, sizeof(entry_t));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next;
  return new_entry;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  /// Calculate the bucket for this entryF
  int hashed_key = ht->hash_func(key);
  size_t bucket = abs(hashed_key % No_bucket);
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(&ht->buckets[bucket], key, ht);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && (ht->ioopm_eq_function(next->key, key)))
    {
      next->value = value;
    }
  else
    {
      entry->next = entry_create(key, value, next);
    }
}

// elem_t *ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) {
//   int hashed_key = ht->hash_func(key);
//   size_t bucket = abs(hashed_key % No_bucket);
//   elem_t *result = NULL;
//   bool check_if_in_table = ioopm_hash_table_lookup(ht, key, result);

//   if (check_if_in_table == false) {
//     return result; 
//   } else {
//     entry_t *prev_bucket = find_previous_entry_for_key(&ht->buckets[bucket], key, ht);
//     entry_t *tmp = prev_bucket->next;
//     if (prev_bucket->next->next == NULL) {
//       prev_bucket->next = NULL;
//       free(tmp);
//       return result;
//     } else  {
//       prev_bucket->next = prev_bucket->next->next;
//       free(tmp);
//       return result;
//     }
//   }
// }

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) {    //Fick ändra remove till att bl.a. returna elem_t inte elem_t *
  int hashed_key = ht->hash_func(key);
  size_t bucket = abs(hashed_key % No_bucket);
  elem_t elem_not_in_ht = {.void_pointer = NULL};
  entry_t *prev_bucket = find_previous_entry_for_key(&ht->buckets[bucket], key, ht);
  entry_t *tmp = prev_bucket->next;

  if (prev_bucket->next == NULL) {    //om value inte finns i ht;
    return elem_not_in_ht; 
  } else {
    elem_t result = tmp->value;
    if (prev_bucket->next->next == NULL) {
      prev_bucket->next = NULL;
      free(tmp);
      return result;
    } else  {
      prev_bucket->next = prev_bucket->next->next;
      free(tmp);
      return result;
    }
  }
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht) {
  clean_up_ht(ht);
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) {
  ioopm_list_t *all_keys = ioopm_linked_list_create(ht->ioopm_eq_function);
  
  for (size_t i = 0; i < No_bucket; i++) {
    if (ht->buckets[i].next != NULL) {
      entry_t *entry = ht->buckets[i].next;
        while (entry != NULL) {
          ioopm_linked_list_append(all_keys, entry->key);
          entry = entry->next;
        }
      }
    }
  return all_keys;
}

// static size_t iterate_thru_ht_values(entry_t *entry, char **arr, size_t index) { 
//   size_t i = index;

//   while (entry->next != NULL) {
//     arr[i] = entry->next->value;
//     i++;
//     index++;
//     entry = entry->next;
//   }
//   return index;
// }

// char **ioopm_hash_table_values(ioopm_hash_table_t *ht) {
//   char **all_values = calloc(ioopm_hash_table_size(ht), sizeof(all_values));
//   size_t current_index = 0;

//   for (int i = 0; i < No_bucket; i++) {
//     if (ht->buckets[i].next != NULL) {
//       current_index = iterate_thru_ht_values(&ht->buckets[i], all_values, current_index);
//       } else {
//     }
//   }
//   return all_values;
// }

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht) {
  ioopm_list_t *list_of_values = ioopm_linked_list_create(ht->ioopm_eq_function);

    for (size_t i = 0; i < No_bucket; i++) {
    if (ht->buckets[i].next != NULL) {
      entry_t *entry = ht->buckets[i].next;
        while (entry != NULL) {
          ioopm_linked_list_append(list_of_values, entry->value);
          entry = entry->next;
        }
      }
    }
    return list_of_values;
}

static bool key_equiv(elem_t key, elem_t value_ignored, void *x, ioopm_eq_function func) {
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  
  return func(key, other_key);
}

static bool value_equiv(elem_t key, elem_t value, void *x, ioopm_eq_function func) {
  elem_t *other_value_ptr = x;
  elem_t other_value = *other_value_ptr;
  
  return func(value, other_value);
}

bool positive_key(elem_t key, elem_t value_ignored, void *not_used, ioopm_eq_function func) {   //pred for has_all
  if (key.number >= 0) {
    return true;
  } else {
    return false;
  }
}


static bool apply_to_entries_any(entry_t *entry, ioopm_predicate pred, void *arg, ioopm_hash_table_t *ht) {
  while (entry != NULL) {
    if (pred(entry->key, entry->value, arg, ht->ioopm_eq_function)) {
      return true;
    } else {
      entry = entry->next;
    }
  }
  return false;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) {
  bool pred_any = false;
  for (size_t i = 0; i < No_bucket; i++) {
    if (pred_any == true) {
      return pred_any;
    }
    else if (ht->buckets[i].next != NULL) {
      pred_any = apply_to_entries_any(ht->buckets[i].next, pred, arg, ht);
    }
  }
  return pred_any;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key) { //int key. 
  
  return ioopm_hash_table_any(ht, key_equiv, key.void_pointer); //&key
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value) { //char *value
  return ioopm_hash_table_any(ht, value_equiv, value.void_pointer);
}

static bool apply_entries_all(entry_t *entry, ioopm_predicate pred, void *arg, ioopm_hash_table_t *ht) {
  bool bool_of_current_entry = true;
  while (bool_of_current_entry && entry != NULL) {
    bool_of_current_entry = pred(entry->key, entry->value, arg, ht->ioopm_eq_function);
    entry = entry->next;
  }
  return bool_of_current_entry;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) {
  bool pred_all = true;
  size_t index = 0;

  while (pred_all == true && index < No_bucket) {

      if (ht->buckets[index].next != NULL) {
        pred_all = apply_entries_all(ht->buckets[index].next, pred, arg, ht);
      }
      index++;
  }
  return pred_all;
}

static void apply_to_all (entry_t *entry, ioopm_apply_function apply_fun, void *arg) {
  
  while (entry != NULL) {
    apply_fun(entry->key, &entry->value, arg);
    entry = entry->next;
  }
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg) {
  size_t index = 0;

  while (index < No_bucket) {

      if (ht->buckets[index].next != NULL) {
        apply_to_all(ht->buckets[index].next, apply_fun, arg);
      }
      index++;
  }
}

void replace_string(elem_t key, elem_t *value, void *arg) {
  char *other_value_ptr = arg;
  value->string = other_value_ptr;
} 

