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
    char *desc = ask_question_string("What is the description of the item? ");
    int price = ask_question_int("What is the price of the item? ");
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
    free(result);
}

void list_merchandise(ioopm_hash_table_t *wh) {
    ioopm_list_t *keys = ioopm_hash_table_keys(wh);
    link_t *current = keys->head;
    int i = 0;
    int limit = 1;  //ändra till 20

    while (current != NULL && i < limit) {
        char *merch = ioopm_linked_list_get(keys, i).string;
        printf("%d: %s\n", i+1, merch);
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
                printf("%d: %s\n", i+1, merch);
                current = current->next;
                i++;
            }
        }
        free(keep_listing);
    }
    ioopm_linked_list_destroy(keys);
}

void free_and_remove(char *name, elem_t *result, ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    elem_t name_elem_t = {.string = name};

    merch_t *merch = result->void_pointer;
    char *name_to_remove = merch->name;

    if (merch->locations == NULL) {  //om locations lista är 0 behöver vi bara freea och remova wh ht
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

void remove_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    char *name = ask_question_string("Which item would you like to remove? ");
    elem_t name_elem_t = {.string = name};
    elem_t *result = calloc(1, sizeof(elem_t));

    if (ioopm_hash_table_lookup(wh, name_elem_t, result)) {
        char *confirm = ask_question_string("Are you sure you want to remove item? "
        "Any string starting with 'y' or 'Y' if yes. Any other string if no" );
        if (confirm[0] == 'y' || confirm[0] == 'Y') {
            merch_t *merch = result->void_pointer;

            if (merch->locations != NULL) {         //freea locations lista om den finns
                ioopm_linked_list_apply_to_all(merch->locations, free_shelf, NULL);
                ioopm_linked_list_destroy(merch->locations);

            }
            free_and_remove(name, result, wh, locations);
            free(confirm);

        } else {
            free(confirm);
        }

    } else {
        printf("%s does not exist in the warehouse!\n", name);

    }
    free(name);
    free(result);
}

void edit_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    char *name = ask_question_string("What item would you like to edit? ");
    elem_t name_elem_t = {.string = name};
    elem_t *result = calloc(1, sizeof(elem_t));

    if (ioopm_hash_table_lookup(wh, name_elem_t, result)) {     //kolla om name av en merch finns, annars rekursivt kall
        char *new_name = ask_question_string("Insert new name: ");
        char *new_desc = ask_question_string("Insert new description: ");
        int new_price = ask_question_int("Insert new price: ");
        elem_t new_name_elem_t = {.string = new_name};
        elem_t *new_result = calloc(1, sizeof(elem_t));

        if (ioopm_hash_table_lookup(wh, new_name_elem_t, new_result)) {
            printf("%s already exists in the warehouse! Going back to main menu \n", new_name);
            free(new_name);
            free(new_desc);
            free(name);
            
        } else {
            merch_t *merchandise = ioopm_merch_create(); 

            char *confirm = ask_question_string("Are you sure you want to edit item? "
            "Any string starting with 'y' or 'Y' if yes. Any other string if no" );

            if (confirm[0] == 'y' || confirm[0] == 'Y') {
                merch_t *merch = result->void_pointer;          
                int stock = merch->stock;                           //spara merchens locations lista och stock
                ioopm_list_t *locations_list = merch->locations;

                free_and_remove(name, result, wh, locations);

                if (locations_list != NULL) {                   // inserta nya ändrade merch och gamla shelfs till locations ht om de fanns
                    char *new_name_cpy1 = strdup(new_name);
                    int size = ioopm_linked_list_size(locations_list); 
                    for (int i = 0; i < size; i++) {
                    elem_t key = ioopm_linked_list_get(locations_list, i);
                    ioopm_hash_table_insert(locations, (elem_t) {.string = new_name_cpy1}, key);
                }
                }
                char *new_name_cpy2 = strdup(new_name);
                ioopm_make_merch(merchandise, new_name_cpy2, new_desc, new_price);
                ioopm_hash_table_insert(wh, (elem_t) {.string = new_name_cpy2}, (elem_t) {.void_pointer = merchandise});
                merchandise->locations = locations_list;  //sätta nya ändrade merch's locations list till den gamla sparade
                merchandise->stock = stock;

                free(confirm);
                free(name);
                free(new_name);

            } else {
                free(confirm);
                free(name);
                free(new_name);
                free(new_desc);
                ioopm_merch_destroy(merchandise);  //inte confirmed, gör ingenting förutom freea
                
            }
        }
        free(new_result);
        free(result);
    } else {
        printf("%s does not exist in the warehouse!\n", name);
        free(name);
        free(result);

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

    char *location = ask_question_shelf("To which storage location would you like to add the item? ");
    char *name = ask_question_string("What is the name of the item? ");
    int quantity = ask_question_int("How many items would you like to add to the stock? ");
    elem_t name_elem_t = {.string = name};
    elem_t *result = calloc(1, sizeof(elem_t));
    bool exist_wh = ioopm_hash_table_lookup(wh, name_elem_t, result);

    if (exist_wh) {
        merch_t *merch = result->void_pointer;
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
        free(location);

    }

    free(result);
}

bool quit() {
    char *confirm = ask_question_string("Are you sure you want to quit? "
    "Any string starting with 'y' or 'Y' if yes. Any other string if no");
    
    if (confirm[0] == 'y' || confirm[0] == 'Y') {
        free(confirm);
        return false;
    } else {
        free(confirm);
        return true;
    }
}

void print_menu() {
    char *menu = "\n[A]dd merchandise\n"
                  "[L]ist merchandise\n"
                  "[D] remove merchandise\n"
                  "[E]dit merchandise\n"
                  "[S]how stock\n"
                  "[P] replenish\n"
                  "[C]reate cart\n"
                  "[R]emove cart\n"
                  "[+] add to cart\n"
                  "[-] remove from cart\n"
                  "[=] calculate cost\n"
                  "[O] checkout\n"
                  "[Q]uit\n";
    puts(menu);
}

char *ask_question_menu() {
    print_menu();
    char *choice = ask_question_string("Välj ett alternativ: ");
    if(strlen(choice) > 2) {
        free(choice);
        return ask_question_menu();
    }
    else if(choice[0] == ('a') || choice[0] == ('l') || choice[0] == ('d') || choice[0] == ('e') ||
       choice[0] == ('s') || choice[0] == ('p') || choice[0] == ('c') || choice[0] == ('r') || 
       choice[0] == ('+') || choice[0] == ('-') || choice[0] == ('=') || choice[0] == ('o') || 
       choice[0] == ('q')) {

       choice[0] = toupper(choice[0]);
       return choice;
    }
    else if(choice[0] == ('A') || choice[0] == ('L') || choice[0] == ('D') || choice[0] == ('E') ||
       choice[0] == ('S') || choice[0] == ('P') || choice[0] == ('C') || choice[0] == ('R') || 
       choice[0] == ('+') || choice[0] == ('-') || choice[0] == ('=') || choice[0] == ('O') || 
       choice[0] == ('Q')) {

       return choice;
    } else {
        free(choice);
        return ask_question_menu();
    }
}

void event_loop(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations) {
    
    bool loop = true;
    while(loop) {
        char *choice = ask_question_menu();
        if(choice[0] == 'A') {
            free(choice);
            ioopm_add_merchandise(wh);
        }
        else if(choice[0] == 'L') {
            free(choice);
            list_merchandise(wh);
        }
        else if(choice[0] == 'D') {
            free(choice);
            remove_merchandise(wh, locations);
        }
        else if(choice[0] == 'E') {
            free(choice);
            edit_merchandise(wh, locations);
        }
        else if(choice[0] == 'S') {
            free(choice);
            show_stock(wh, locations);
        }
        else if(choice[0] == 'P') {
            free(choice);
            ioopm_replenish(wh, locations);
        }
        else if(choice[0] == 'C') {
            free(choice);
            //create cart
        } 
        else if(choice[0] == 'R') {
            free(choice);
            //remove cart
        } 
        else if(choice[0] == '+') {
            free(choice);
            //add to cart
        } 
        else if(choice[0] == '-') {
            free(choice);
            //remove from cart
        } 
        else if(choice[0] == '=') {
            free(choice);
            //calculate cost
        } 
        else if(choice[0] == 'O') {
            free(choice);
            //checkout
        } 
        else if(choice[0] == 'Q') {
            //quit
            free(choice);
            loop = quit();
        } else {
            
        }
    }
}