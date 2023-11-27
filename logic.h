#pragma once
#include "linked_list.h"
#include "common.h"
#include "hash_table.h"


/**
 * @file logic.h
 * @author Muhammad Saad, Hans-Eskil Ytterfelt Martinsson
 * @date 11 Nov 2023
 * @brief Cointains all functions that do not ask the user for inputs.
 *
 * This file contains MOST of the functions that do not take user input to modify the warehouse and locations hashtables.
 * The Warehouse hashtable is a hashtable that maps the name of an item (key) to a 
 * pointer to a merch_t struct (value). The locations hashtable maps the name of an item (key)
 * to a storage location (value). 
 *
 * @see 
 */

typedef struct shelf shelf_t;
typedef struct merch merch_t;
typedef struct cart cart_t;
extern cart_t *current_cart;


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

struct cart {
    int cart_id;
    ioopm_list_t *cart_content;
};

/// @brief Allocates memory on the heap for a merch_t struct
/// @return the allocated merch_t struct
merch_t *ioopm_merch_create();

cart_t *ioopm_create_cart();

/// @brief Allocates memory on the heap for a shelf_t struct
/// @return the allocated shelf_t struct
shelf_t *ioopm_shelf_create();

/// @brief A function to be used with the hash_table_apply_to_all and linked_list_apply_to_all 
/// function from assignment 1 in the files hash_table.h and linked_list.h. It frees the allocated 
/// shelf_t struct in a hashtable or linked_list
/// @param key_ignored Key is not used
/// @param value the value whose void pointer points to a shelf_t struct
/// @param extra extra is not used
void free_shelf(elem_t key_ignored, elem_t *value, void *extra);

/// @brief A function to be used with the hash_table_apply_to_all and linked_list_apply_to_all 
/// function from assignment 1 in the files hash_table.h and linked_list.h. 
/// It frees all pointers to names and descriptions of items in a hashtable. 
/// It also frees any eventual locations as well as the locations linked_list in a merch_t struct.
/// Lastly, the merch_t itself is destroyed
/// @param key_ignored Key is not used
/// @param value the value whose void pointer points to a merch_t struct
/// @param ignored ignored is not used
void free_void_ptr_value(elem_t key_ignored, elem_t *value, void *ignored);

/// @brief A function to be used with the hash_table_apply_to_all and linked_list_apply_to_all 
/// function from assignment 1 in the files hash_table.h and linked_list.h. Simply frees the 
/// pointer to a string name of a merch
/// @param key The pointer to a string to be freed
/// @param value_ignored value is not used
/// @param ignored ignored is not used
void free_void_ptr_key(elem_t key, elem_t *value_ignored, void *ignored);

/// @brief simple funtion that assigns a merch_t struct a name, description and price
/// @param merch the merch_t struct
/// @param name the name of the merch
/// @param desc a description of the merch
/// @param price the price of the merch
void ioopm_make_merch(merch_t *merch, char *name, char *desc, int price);

/// @brief simply frees a shelf_t from the heap
/// @param shelf the shelf_t to be freed
void ioopm_shelf_destroy(shelf_t *shelf);

/// @brief simply frees a merch_t from the heap
/// @param merch the merch_t to be freed
void ioopm_merch_destroy(merch_t *merch);

/// @brief A function that frees and removes a merch_t from the warehouse and if possible, the locations ht.
/// @param name name of the merch to be removed
/// @param result the merch_t struct to be removed
/// @param wh the warehouse ht
/// @param locations the locations ht
void free_and_remove(char *name, elem_t *result, ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

/// @brief inserts a merch_t in the warehouse ht
/// @param wh the warehouse ht
/// @param name the name of the merch to be inserted
/// @param desc the description of the merch to be inserted
/// @param price the price of the merch to be inserted
/// @param result an empty elem_t struct from assignment 1, needed for 
/// the hash_table_lookup function. Please see hash_table.h for more details
void wh_insert(ioopm_hash_table_t *wh, char *name, char *desc, int price, elem_t *result);

/// @brief a function that frees everything in the warehouse and locations hts, as well as the hts
/// themselves
/// @param wh the warehouse ht
/// @param locations the locations ht
void free_and_destroy_hts(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

/// @brief Adds a selected merchandise to the current shopping cart.
/// @param wh the warehouse ht
void add_to_cart(ioopm_hash_table_t *wh);


/// @brief Creates a new shopping cart for the user.
void create_cart();


/// @brief Removes the current shopping cart, discarding its contents.
void remove_cart();


/// @brief Removes a selected merchandise from the current shopping cart.
void remove_from_cart();


/// @brief Calculates the total cost of items in the current shopping cart.
void calculate_cost();


/// @brief Completes the checkout process, updating warehouse quantities and removing items from locations.
/// @param wh the warehouse ht
void checkout(ioopm_hash_table_t *wh);

/// @brief Helper function that asks the user to input an integer larger than 0
/// @param s A question string to ask the user
/// @return a valid integer provided by user input
int quant_helper(char *s);