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

void ioopm_make_merch(merch_t *merch, char *name, char *desc, int price) {
    merch->name = name;
    merch->information = desc;
    merch->price = price;
}

void ioopm_merch_destroy(merch_t *merch) {
    free(merch);
}

void free_void_ptr_value(elem_t key_ignored, elem_t *value, void *ignored) {
    merch_t *merch = value->void_pointer;
    if (merch->locations == NULL) {
        free(merch->information);
        free(value->void_pointer);
    } else {
        ioopm_linked_list_destroy(merch->locations);
        merch->locations = NULL;
        free(merch->information);
        free(value->void_pointer);
    }
}

void free_void_ptr_key(elem_t key, elem_t *value_ignored, void *ignored) {
    free(key.void_pointer);
}

void ioopm_add_merchandise(ioopm_hash_table_t *wh) {
    elem_t *result = calloc(1, sizeof(elem_t));
    char *name = ask_question_string("What is the name of the item? ");
    elem_t name_elem_t = {.string = name};

    while (ioopm_hash_table_lookup(wh, name_elem_t, result)) {  //if item name is already in warehouse, ask again.
        puts("Merchandise is already in warehouse! ");
        name = ask_question_string("What is the name of the item? ");
        if (strcmp(name, name_elem_t.string) != 0) {
            break;
        }
    }

    char *desc = ask_question_string("What is the description of the item? ");
    int price = ask_question_int("What is the price of the item? ");
    merch_t *merchandise = ioopm_merch_create();

    ioopm_make_merch(merchandise, (name), (desc), price);
    ioopm_hash_table_insert(wh, (elem_t) {.string = name}, (elem_t) {.void_pointer = merchandise}); //inserta merch i warehouse (wh) hashtable
    // Vet inte om det ska vara void_pointer eller om vi får lägga till merch_t pointer i elem_t?
    
    free(result);
    //ioopm_merch_destroy(merchandise);
    //free(name);
    //free(desc);
    //ioopm_linked_list_destroy(items);
}

void list_merchandise(ioopm_hash_table_t *wh) {
    ioopm_list_t *keys = ioopm_hash_table_keys(wh);
    link_t *current = keys->head;
    for (int i = 0; current != NULL || i < 20; i++) {
        char *merch = ioopm_linked_list_get(keys, i).string;
        printf("%d: %s\n", i, merch);
        current = current->next;
    }

    char *keep_listing = ask_question_string("Would you like to keep listing? (n)o or (N)o if not");
}

void remove_merchandise(ioopm_hash_table_t *ht) {

}

void edit_merchandise(ioopm_hash_table_t *ht) {

}

void show_stock(ioopm_hash_table_t *ht) {

}



void ioopm_replenish(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {

    ioopm_list_t *items = ioopm_hash_table_keys(wh);
    char *name = ask_question_string("What is the name of the item? ");
    elem_t name_elem_t = {.string = name};
    items->ioopm_eq_function = ioopm_str_eq;
    elem_t *result = calloc(1, sizeof(elem_t));
    bool j = ioopm_hash_table_lookup(wh, name_elem_t, result);
    merch_t *merch = result->void_pointer;
    merch_t a = *merch;

    printf("stock: %d", a.stock);
    
    char *shelf = ask_question_shelf("To which storage location would you like to add the item? ");
    int quantity = ask_question_int("How many items would you like to add to the stock? ");
    elem_t shelf_elem_t = {.string = shelf};
    merch->stock += quantity;

    ioopm_hash_table_insert(locations, name_elem_t, shelf_elem_t);  //Inserta merch i locations ht.
    if (merch->locations == NULL) {
        ioopm_list_t *locations_list = ioopm_linked_list_create(ioopm_str_eq);
        ioopm_linked_list_append(locations_list, shelf_elem_t);
        merch->locations = locations_list;
    } else {
        ioopm_linked_list_append(merch->locations, shelf_elem_t);
    }


    printf("stock: %d \n", merch->stock);
    printf("shelf: %s \n", merch->locations->head->value.string);
    
}

void create_cart(ioopm_hash_table_t *ht) {
    ioopm_hash_table_create(ioopm_str_eq);

}

void remove_cart(ioopm_hash_table_t *ht) {

}

int main(int argc, char *argv[]) {
    ioopm_hash_table_t *wh = ioopm_hash_table_create(ioopm_str_eq);
    ioopm_hash_table_t *locations = ioopm_hash_table_create(ioopm_str_eq);
    wh->hash_func = string_sum_hash;
    locations->hash_func = string_sum_hash;

    ioopm_add_merchandise(wh);
    ioopm_list_t *merch_list = ioopm_hash_table_values(wh);
    elem_t merch1 = ioopm_linked_list_get(merch_list, 0);
    merch_t *meerch = merch1.void_pointer;
    printf("%d \n", meerch->price);

    // ioopm_add_merchandise(wh);
    // merch_list = ioopm_hash_table_values(wh);
    // elem_t merch2 = ioopm_linked_list_get(merch_list, 1);
    // merch_t *meerch2 = merch2.void_pointer;
    // printf("%d \n", meerch2->price);

    // ioopm_replenish(wh, locations);
    // list_merchandise(wh);

    return 0;
}