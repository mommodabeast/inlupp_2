#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"
#include "hash_table.h"
#include "common.h"
#include "utils.h"
#include "logic.h"

merch_t *ioopm_merch_create() {
    return calloc(1, sizeof(merch_t));
}

shelf_t *ioopm_shelf_create() {
    return calloc(1, sizeof(shelf_t));
}

void ioopm_make_merch(merch_t *merch, char *name, char *desc, int price) {
    merch->name = name;
    merch->information = desc;
    merch->price = price;
}

void ioopm_merch_destroy(merch_t *merch) {
    free(merch);
}

void ioopm_shelf_destroy(shelf_t *shelf) {
    free(shelf->shelf);
    free(shelf);
}

void free_shelf(elem_t key_ignored, elem_t *value, void *extra) {
    ioopm_shelf_destroy(value->void_pointer);
}

void free_void_ptr_value(elem_t key_ignored, elem_t *value, void *ignored) {
    merch_t *merch = value->void_pointer;
    if (merch->locations == NULL) {
        free(merch->information);
        free(value->void_pointer);
    } else {
        ioopm_linked_list_apply_to_all(merch->locations, free_shelf, NULL);
        ioopm_linked_list_destroy(merch->locations);
        merch->locations = NULL;
        free(merch->information);
        free(value->void_pointer);
    }
}

// void free_void_ptr_value_locations(elem_t key_ignored, elem_t *value, void *ignored) {
//     free(value->void_pointer);
// }

void free_void_ptr_key(elem_t key, elem_t *value_ignored, void *ignored) {
    free(key.void_pointer);
}

void free_and_remove(char *name, elem_t *result, ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    elem_t name_elem_t = {.string = name};

    merch_t *merch = result->void_pointer;
    char *name_to_remove = merch->name;

    if (merch->locations == NULL) {  //om locations lista är 0 behöver vi bara freea och remova i wh ht
        free(merch->information);
        ioopm_hash_table_remove(wh, name_elem_t);
    } else {
        free(merch->information);
        ioopm_hash_table_remove(wh, name_elem_t);
        ioopm_list_t *keys = ioopm_hash_table_keys(locations);
        int size = ioopm_linked_list_size(keys);
        for (int i = 0; i < size; i++) {
            elem_t key = ioopm_linked_list_get(keys, i);
            if (strcmp(name, key.string) == 0) {
                char *key_string = key.string;
                elem_t key_elem_t = {.string = key_string};
                ioopm_hash_table_remove(locations, key_elem_t);
                free(key_string);
            }
        }
        ioopm_linked_list_destroy(keys);
    }
    free(name_to_remove);
    free(merch);
  
}

void wh_insert(ioopm_hash_table_t *wh, char *name, char *desc, int price, elem_t *result) {
    elem_t name_elem_t = {.string = name};

    if (ioopm_hash_table_lookup(wh, name_elem_t, result)) {
        printf("%s is already in the warehouse! Going back to main menu \n", name);
        free(name);
        free(desc);

    } else {
        merch_t *merchandise = ioopm_merch_create();
        ioopm_make_merch(merchandise, name, desc, price);
        ioopm_hash_table_insert(wh, (elem_t) {.string = name}, (elem_t) {.void_pointer = merchandise}); //inserta merch i warehouse (wh) hashtable
        
    }
}

void free_and_destroy_hts(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    if (!ioopm_hash_table_is_empty(wh)) { //frigöra alla pekare

        if (!ioopm_hash_table_is_empty(locations)) {
            ioopm_hash_table_apply_to_all(wh, free_void_ptr_value, NULL);
            ioopm_hash_table_apply_to_all(wh, free_void_ptr_key, NULL);
            ioopm_hash_table_apply_to_all(locations, free_void_ptr_key, NULL);

        } else {
            ioopm_hash_table_apply_to_all(wh, free_void_ptr_value, NULL);
            ioopm_hash_table_apply_to_all(wh, free_void_ptr_key, NULL);
            ioopm_hash_table_apply_to_all(locations, free_void_ptr_key, NULL);
            ioopm_hash_table_apply_to_all(locations, free_void_ptr_value, NULL);

        }  

    } else {

    }
    ioopm_hash_table_destroy(wh);
    ioopm_hash_table_destroy(locations);
}