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

void free_void_ptr_value_locations(elem_t key_ignored, elem_t *value, void *ignored) {
    free(value->void_pointer);
}

void free_void_ptr_key(elem_t key, elem_t *value_ignored, void *ignored) {
    free(key.void_pointer);
}

char *merch_name_helper(ioopm_hash_table_t *wh) {
    elem_t *result = calloc(1, sizeof(elem_t));
    puts("Merchandise is already in warehouse! ");
    char *name = ask_question_string("What is the name of the item? ");
    elem_t name_elem_t = {.string = name};

    if (ioopm_hash_table_lookup(wh, name_elem_t, result)) {
        free(result);
        free(name);
        return merch_name_helper(wh);
    } else {
        free(result);
        char *merch_name = strdup(name);
        free(name);
        return merch_name;
    }
}

void ioopm_add_merchandise(ioopm_hash_table_t *wh) {
    elem_t *result = calloc(1, sizeof(elem_t));
    char *name = ask_question_string("What is the name of the item? ");
    elem_t name_elem_t = {.string = name};

    if (ioopm_hash_table_lookup(wh, name_elem_t, result)) {
        free(name);
        char *merch_name = merch_name_helper(wh);
        char *desc = ask_question_string("What is the description of the item? ");
        int price = ask_question_int("What is the price of the item? ");
        merch_t *merchandise = ioopm_merch_create();

        ioopm_make_merch(merchandise, merch_name, desc, price);
        ioopm_hash_table_insert(wh, (elem_t) {.string = merch_name}, (elem_t) {.void_pointer = merchandise});
    } else {
        char *desc = ask_question_string("What is the description of the item? ");
        int price = ask_question_int("What is the price of the item? ");
        merch_t *merchandise = ioopm_merch_create();

        ioopm_make_merch(merchandise, name, desc, price);
        ioopm_hash_table_insert(wh, (elem_t) {.string = name}, (elem_t) {.void_pointer = merchandise}); //inserta merch i warehouse (wh) hashtable
    }
    free(result);
}

void list_merchandise(ioopm_hash_table_t *wh) {
    ioopm_list_t *keys = ioopm_hash_table_keys(wh);
    link_t *current = keys->head;
    int i = 0;
    int limit = 1;  //ändra till 20

    while (current != NULL && i < limit) {
        char *merch = ioopm_linked_list_get(keys, i).string;
        printf("%d: %s\n", i, merch);
        current = current->next;
        i++;
    }
    
    while (current != NULL) {
        char *keep_listing = ask_question_any_input_string("Would you like to keep listing? "
        "input any string starting with 'n' or 'N' if not. Any other string, including an empty string if yes. ");
        if (keep_listing[0] == 'n' || keep_listing[0] == 'N') {
            free(keep_listing);
            break;
        } else {
            limit += 1; //ändra till 20
            while (current != NULL && i < limit) {
                char *merch = ioopm_linked_list_get(keys, i).string;
                printf("%d: %s\n", i, merch);
                current = current->next;
                i++;
            }
        }
        free(keep_listing);
    }
    ioopm_linked_list_destroy(keys);
}

void remove_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    char *name = ask_question_string("Which item would you like to remove? ");
    elem_t name_elem_t = {.string = name};
    elem_t *result = calloc(1, sizeof(elem_t));

    if (ioopm_hash_table_lookup(wh, name_elem_t, result)) {
        char *confirm = ask_question_string("Are you sure you want to remove item? "
        "Any string starting with 'y' or 'Y' if yes. Any other string if no" );
        if (confirm[0] == 'y' || confirm[0] == 'Y') {
            merch_t *merch = result->void_pointer;
            char *name_to_remove = merch->name;

            if (merch->locations == NULL) {
                free(merch->information);
                free(result->void_pointer);
            } else {
                ioopm_linked_list_apply_to_all(merch->locations, free_shelf, NULL);
                ioopm_linked_list_destroy(merch->locations);
                merch->locations = NULL;
                free(merch->information);
            }
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
            free(name_to_remove);
            free(merch);
            ioopm_linked_list_destroy(keys);
            free(confirm);
        } else {
            free(confirm);
        }

    } else {
        printf("%s does not exist in the warehouse!\n", name);
        remove_merchandise(wh, locations);
    }
    free(name);
    free(result);
}

void free_and_remove(char *name, elem_t *result, ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    elem_t name_elem_t = {.string = name};

    merch_t *merch = result->void_pointer;
    char *name_to_remove = merch->name;

    if (merch->locations == NULL) {
        free(merch->information);
        free(result->void_pointer);
    } else {
        free(merch->information);
    }

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
    free(name_to_remove);
    free(merch);
    ioopm_linked_list_destroy(keys);
}

void edit_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    char *name = ask_question_string("What item would you like to edit? ");
    elem_t name_elem_t = {.string = name};
    elem_t *result = calloc(1, sizeof(elem_t));


    if (ioopm_hash_table_lookup(wh, name_elem_t, result)) {     //kolla om name av en merch finns, annars rekursivt kall
        char *new_name = ask_question_string("Insert new name: ");
        elem_t new_name_elem_t = {.string = new_name};
        elem_t *new_result = calloc(1, sizeof(elem_t));

        if (ioopm_hash_table_lookup(wh, new_name_elem_t, new_result)) {
            free(new_name);
            char *new_name2 = merch_name_helper(wh);      //fråga igen om new_name finns redan
            char *new_desc = ask_question_string("Insert new description: ");
            int new_price = ask_question_int("Insert new price: ");
            elem_t new_name2_elem_t = {.string = new_name2};
            merch_t *merchandise = ioopm_merch_create(); 

            char *confirm = ask_question_string("Are you sure you want to edit item? "
            "Any string starting with 'y' or 'Y' if yes. Any other string if no" );

            if (confirm[0] == 'y' || confirm[0] == 'Y') {
            merch_t *merch = result->void_pointer;          //spara merchens locations lista
            ioopm_list_t *locations_list = merch->locations;
            


            free_and_remove(name, result, wh, locations);

            int size = ioopm_linked_list_size(locations_list); // inserta nya ändrade merch och shelfs till locations ht
            for (int i = 0; i < size; i++) {
                elem_t key = ioopm_linked_list_get(locations_list, i);
                ioopm_hash_table_insert(locations, new_name2_elem_t, key);
            }

            char *new_name_cpy = strdup(new_name2);
            ioopm_make_merch(merchandise, new_name_cpy, new_desc, new_price);
            ioopm_hash_table_insert(wh, (elem_t) {.string = new_name_cpy}, (elem_t) {.void_pointer = merchandise});
             merchandise->locations = locations_list;  //sätta nya ändrade merch's locations list till den gamla sparade

            free(confirm);
            free(name);
            } else {
                free(confirm);
                free(name);
                free(new_name2);
                free(new_desc);
                ioopm_merch_destroy(merchandise);
                //inte confirmed, gör ingenting förutom freea
            }
        } else {
            char *new_desc = ask_question_string("Insert new description: ");
            int new_price = ask_question_int("Insert new price: ");
            merch_t *merchandise = ioopm_merch_create(); 

            char *confirm = ask_question_string("Are you sure you want to edit item? "
            "Any string starting with 'y' or 'Y' if yes. Any other string if no" );

            if (confirm[0] == 'y' || confirm[0] == 'Y') {
                merch_t *merch = result->void_pointer;          //spara merchens locations lista
                ioopm_list_t *locations_list = merch->locations;

                free_and_remove(name, result, wh, locations);

                int size = ioopm_linked_list_size(locations_list); // inserta nya ändrade merch och shelfs till locations ht
                for (int i = 0; i < size; i++) {
                elem_t key = ioopm_linked_list_get(locations_list, i);
                ioopm_hash_table_insert(locations, new_name_elem_t, key);
                }
                char *new_name_cpy = strdup(new_name);
                ioopm_make_merch(merchandise, new_name_cpy, new_desc, new_price);
                ioopm_hash_table_insert(wh, (elem_t) {.string = new_name_cpy}, (elem_t) {.void_pointer = merchandise});
                merchandise->locations = locations_list;  //sätta nya ändrade merch's locations list till den gamla sparade

                free(confirm);
                free(name);

            } else {
                free(confirm);
                free(name);
                free(new_name);
                free(new_desc);
                ioopm_merch_destroy(merchandise);
                //inte confirmed, gör ingenting förutom freea
            }
        }
        free(new_result);
        free(result);
    } else {
        printf("%s does not exist in the warehouse!\n", name);
        edit_merchandise(wh, locations);
    }
    
}

void show_stock(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    char *name = ask_question_string("Which item's stock would you like to see? ");
    elem_t name_elem_t = {.string = name};
    elem_t *result = calloc(1, sizeof(elem_t));

    if (ioopm_hash_table_lookup(wh, name_elem_t, result)) {
        merch_t *merch = result->void_pointer;
        ioopm_list_t *shelf_list = merch->locations;
        int size = ioopm_linked_list_size(shelf_list);

        for (int i = 0; i < size; i++) {
            elem_t shelf = ioopm_linked_list_get(shelf_list, i);
            shelf_t *location = shelf.void_pointer;
            printf("location %d: %s\n", i+1, location->shelf);
            printf("quantity: %d\n", location->quantity);
        }
        printf("Total stock: %d\n", merch->stock);
    } else {
        printf("%s does not exist in the warehouse!\n", name);
        show_stock(wh, locations);
    }
    free(name);
    free(result);
}

int quant_helper() {
    puts("quantity cannot be less than 1! ");
    int quantity = ask_question_int("How many items would you like to add to the stock? ");

    if (quantity > 0) {
        return quantity;
    } else {
        return quant_helper();
    }
}

void ioopm_replenish(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {

    char *name = ask_question_string("What is the name of the item? ");
    elem_t name_elem_t = {.string = name};
    elem_t *result = calloc(1, sizeof(elem_t));
    bool exist_wh = ioopm_hash_table_lookup(wh, name_elem_t, result);

    if (exist_wh) {
        merch_t *merch = result->void_pointer;
        char *location = ask_question_shelf("To which storage location would you like to add the item? ");
        int quantity = ask_question_int("How many items would you like to add to the stock? ");
        elem_t location_elem_t = {.string = location};
        
        if (quantity <= 0) {
            quantity = quant_helper();
        }

       
        shelf_t *shelf = ioopm_shelf_create();
        shelf->shelf = location;
        shelf->quantity = quantity;
        merch->stock += quantity;
        elem_t shelf_elem_t = {.void_pointer = shelf};
        elem_t *result2 = calloc(1, sizeof(elem_t));
       
        if (merch->locations == NULL) {
            ioopm_hash_table_insert(locations, name_elem_t, location_elem_t);  //Inserta merch i locations ht.
            ioopm_list_t *locations_list = ioopm_linked_list_create(ioopm_str_eq);
            ioopm_linked_list_append(locations_list, shelf_elem_t);
            merch->locations = locations_list;
        } else {
            ioopm_hash_table_insert(locations, name_elem_t, location_elem_t);  //Inserta merch i locations ht.
            ioopm_linked_list_append(merch->locations, shelf_elem_t);
            free(name);

        }
    free(result2);
    } else {
        printf("%s does not exist in the warehouse!\n", name);
        free(name);
        ioopm_replenish(wh, locations);
    }

    free(result);
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

    // ioopm_add_merchandise(wh);
    ioopm_replenish(wh, locations);
    ioopm_replenish(wh, locations);
    // list_merchandise(wh);
    // show_stock(wh, locations);
    // remove_merchandise(wh, locations);
    // list_merchandise(wh);
    // edit_merchandise(wh, locations);
    // list_merchandise(wh);
    show_stock(wh, locations);

    if (!ioopm_hash_table_is_empty(wh) && !ioopm_hash_table_is_empty(locations)) { //frigöra alla pekare
        
        ioopm_hash_table_apply_to_all(wh, free_void_ptr_value, NULL);
        ioopm_hash_table_apply_to_all(wh, free_void_ptr_key, NULL);
        ioopm_hash_table_apply_to_all(locations, free_void_ptr_key, NULL);
    } else {

    }

    ioopm_linked_list_destroy(merch_list);
    ioopm_hash_table_destroy(wh);
    ioopm_hash_table_destroy(locations);

    return 0;
}