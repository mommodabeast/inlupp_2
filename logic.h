#pragma once
#include "linked_list.h"
#include "common.h"
#include "hash_table.h"

typedef struct shelf shelf_t;
typedef struct merch merch_t;


struct shelf {
    char *shelf;
    int quantity;
};

struct merch {
    char *name;
    char *information;
    int price;
    int stock;
    ioopm_list_t *locations;
};

merch_t *ioopm_merch_create();

shelf_t *ioopm_shelf_create();

void free_shelf(elem_t key_ignored, elem_t *value, void *extra);

void free_void_ptr_value(elem_t key_ignored, elem_t *value, void *ignored);

void free_void_ptr_value_locations(elem_t key_ignored, elem_t *value, void *ignored);

void free_void_ptr_key(elem_t key, elem_t *value_ignored, void *ignored);

void ioopm_make_merch(merch_t *merch, char *name, char *desc, int price);

void ioopm_shelf_destroy(shelf_t *shelf);

void ioopm_merch_destroy(merch_t *merch);

void ioopm_make_merch(merch_t *merch, char *name, char *desc, int price);

void free_and_remove(char *name, elem_t *result, ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

void wh_insert(ioopm_hash_table_t *wh, char *name, char *desc, int price, elem_t *result);

void free_and_destroy_hts(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);
