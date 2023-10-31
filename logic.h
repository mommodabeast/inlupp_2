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

void ioopm_merch_destroy(merch_t *merch);

void ioopm_make_merch(merch_t *merch, char *name, char *desc, int price);

void ioopm_add_merchandise(ioopm_hash_table_t *wh);

void list_merchandise(ioopm_hash_table_t *wh);

void ioopm_replenish(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

void free_void_ptr_value(elem_t key_ignored, elem_t *value, void *ignored);

void free_void_ptr_key(elem_t key, elem_t *value_ignored, void *ignored);