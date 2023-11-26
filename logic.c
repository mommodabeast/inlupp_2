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

static int cart_id_counter = 0;
cart_t *current_cart = NULL;

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

    //if (current_cart->cart_content != NULL) {
    if (cart_id_counter != 0) {
        ioopm_linked_list_apply_to_all(current_cart->cart_content, free_void_ptr_value, NULL);
        ioopm_linked_list_destroy(current_cart->cart_content);
        free(current_cart);
        current_cart = NULL;
    } else {

    }

    ioopm_hash_table_destroy(wh);
    ioopm_hash_table_destroy(locations);
}

int stock_availability(int stock){
    puts("Out of stock! ");
    printf(" \nStock available %d: ", stock);
    int quantity = ask_question_int("How many items would you like to add to the cart?" );

    if (quantity <= stock )
    {
        return quantity;
    }
    else
    {
        return stock_availability(stock);
    }
}

struct cart *ioopm_create_cart()
{
    cart_t *cart = malloc(sizeof(struct cart));
    cart->cart_id = ++cart_id_counter;
    cart->cart_content = ioopm_linked_list_create(ioopm_str_eq);
    return cart;
}

void create_cart()
{
    if (current_cart)
    {
        printf("A cart is already created. Please finish or clear the current cart before creating a new one.\n");
        return;
    }

    current_cart = ioopm_create_cart();
    printf("Created cart with ID: %d\n", current_cart->cart_id);
}

void remove_cart()
{
    if (!current_cart)
        printf("No cart to remove.\n");

    char confirmation[2];

    printf("Are you sure you want to remove the current cart #%d? Enter 'Y' or 'y' to confirm: ", current_cart->cart_id);
    scanf("%1s", confirmation);

    if (confirmation[0] == 'Y' || confirmation[0] == 'y')
    {
        current_cart->cart_id--;                                                                                    //lade till detta
        cart_id_counter -= 1;
        ioopm_linked_list_apply_to_all(current_cart->cart_content, free_void_ptr_value, NULL);
        ioopm_linked_list_destroy(current_cart->cart_content);
        free(current_cart);
        current_cart = NULL;
        printf("Current cart removed.\n");
    }
    else
    {
        printf("Removal cancelled.\n");
    }
}

void add_to_cart(ioopm_hash_table_t *wh)
{
    if (!current_cart)
    {
        printf("No cart available. Create a cart first.\n");
        return;
    }

    char *name = ask_question_string("What is the name of the item? ");
    elem_t name_elem_t = {.string = name};
    elem_t *result = calloc(1, sizeof(elem_t));
    bool exist_wh = ioopm_hash_table_lookup(wh, name_elem_t, result);

    if (exist_wh)
    {
        merch_t *merch = result->void_pointer;
        printf(" \nStock available %d: ", merch->stock);
        int quantity = ask_question_int("How many items would you like to add to the cart?" );
        if (quantity <= 0)
        {
            quantity = quant_helper();
        }
        if (quantity > merch->stock)
        {
            quantity = stock_availability(merch->stock);
        }

        link_t *current = current_cart->cart_content->head;
        while (current)
        {
            merch_t *cart_item = current->value.void_pointer;
            if (strcmp(cart_item->name, merch->name) == 0)
            {
                cart_item->stock += quantity;
                // merch->stock -= quantity;
                printf("%d items added of merch %s to cart# %d\n", quantity, merch->name, current_cart->cart_id);
                free(result);
                return;
            }
            current = current->next;
        }

        merch_t *cart_item = ioopm_merch_create();
        ioopm_make_merch(cart_item, (merch->name), strdup(merch->information), (merch->price));
        cart_item->stock = quantity;
        ioopm_linked_list_append(current_cart->cart_content, (elem_t){.void_pointer = cart_item});
        // merch->stock -= quantity;
        printf("%d items added of merch %s to cart# %d\n", quantity, merch->name, current_cart->cart_id);
        free(result);
        free(name);
        return;
    }
    else
    {
        printf("Merchandise with name %s not found!\n", name);
        free(name);
        add_to_cart(wh);
    }

    free(result);
}

bool merchandise_exists_in_cart(cart_t *cart, char *name) {
    ioopm_list_t *cart_content = cart->cart_content;

    for (size_t i = 0; i < ioopm_linked_list_size(cart_content); ++i) {
        merch_t *cart_item = ioopm_linked_list_get(cart_content, i).void_pointer;

        if (strcmp(cart_item->name, name) == 0) {
            return true;
        }
    }

    return false;
}

void remove_from_cart()
{
    if (!current_cart)
    {
        printf("No cart available. Create a cart first.\n");
        return;
    }

    link_t *current = current_cart->cart_content->head;

    if (!current)
    {
        printf("Current cart is Empty\n");
        return;
    }
    else
    {
        printf("Current items in the cart:\n");
    }

    while (current)
    {
        merch_t *cart_item = current->value.void_pointer;
        printf("%-20s | Price: %d | Stock: %-4d\n", cart_item->name, cart_item->price, cart_item->stock);
        current = current->next;
    }

    char *name = ask_question_string("What is the name of the item? ");
    bool exist_wh = merchandise_exists_in_cart(current_cart, name);

    if (exist_wh)
    {
        int quantity = ask_question_int("How many items would you like to remove from the cart? ");

        if (quantity <= 0)
        {
            quantity = quant_helper();
        }

        link_t *current2 = current_cart->cart_content->head;
        size_t index = 0;


        while (current2)
        {
            merch_t *cart_item = current2->value.void_pointer;
            if (strcmp(cart_item->name, name) == 0)
            {
                if (quantity >= cart_item->stock)
                {
                    ioopm_linked_list_remove(current_cart->cart_content, index);
                    ioopm_merch_destroy(cart_item);
                    printf("Removed all items of merch %s from cart# %d\n", name, current_cart->cart_id);
                }
                else
                {
                    cart_item->stock -= quantity;
                    printf("%d items removed of merch %s from cart# %d\n", quantity, name, current_cart->cart_id);
                }
                break;
            }
            current2 = current2->next;
            ++index;
        }
    }
    else
    {
        printf("Merchandise with name %s not found!\n", name);
        remove_from_cart();
    }
    free(name);
}

void calculate_cost(){
    int calculated_price = 0 ;
    if (!current_cart)
    {
        printf("No cart available. Create a cart first.\n");
        return;
    }
    link_t *current = current_cart->cart_content->head;

    while (current)
    {
        merch_t *cart_item = current->value.void_pointer;
        printf("%-20s | Price: %d | Stock: %-4d\n", cart_item->name, cart_item->price, cart_item->stock);
        calculated_price += cart_item->price*cart_item->stock;
        current = current->next;
    }

    printf("Total Amount of current Cart %d: %d.\n", current_cart->cart_id,  calculated_price);
}

void checkout(ioopm_hash_table_t *wh)
{
    if (!current_cart)
    {
        printf("No cart available. Create a cart first.\n");
        return;
    }

    printf("Checking out cart# %d\n", current_cart->cart_id);

    link_t *current = current_cart->cart_content->head;

    while (current)
        {
        merch_t *cart_item = current->value.void_pointer;
        int quantity = cart_item->stock;
        char *name = cart_item->name;
        elem_t name_elem_t = {.string = name};
        elem_t *result = calloc(1, sizeof(elem_t));
        bool exist_wh = ioopm_hash_table_lookup(wh, name_elem_t, result);


        if (exist_wh)
        {
            merch_t *wh_merch = result->void_pointer;

            if (wh_merch->stock >= quantity)
            {
                wh_merch->stock -= quantity;
                printf("%d items of %s successfully purchased. Stock left: %d\n", quantity, name, wh_merch->stock);
            }
            else
            {
                printf("Purchase error.\n");
            }
        }
        else
        {
            printf("Merchandise with name %s not found in the warehouse.\n", name);
        }
        current = current->next;
        free(result);
    }

    remove_cart();
    
    printf("Checkout completed.\n");
}