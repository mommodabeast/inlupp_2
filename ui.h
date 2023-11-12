#pragma once
#include "linked_list.h"
#include "common.h"
#include "hash_table.h"
#include "logic.h"

/**
 * @file ui.h
 * @author Muhammad Saad, Hans-Eskil Ytterfelt Martinsson
 * @date 11 Nov 2023
 * @brief Cointains all functions that ask the user for inputs.
 *
 * This file contains all functions that take user input to modify the warehouse and locations hashtables.
 * The Warehouse hashtable is a hashtable that maps the name of an item (key) to a 
 * pointer to a merch_t struct (value). The locations hashtable maps the name of an item (key)
 * to a storage location (value). 
 *
 * @see 
 */



/// @brief asks the user for a name, description and price of an item. The item is then added
/// to the warehouse hashtable. If item already exists in hashtable, the user is taken to the main menu
/// @param wh the warehouse hashtable
void ioopm_add_merchandise(ioopm_hash_table_t *wh);

/// @brief lists all the existing items in the warehouse hashtable
/// @param wh the warehouse hashtable
void list_merchandise(ioopm_hash_table_t *wh);

/// @brief asks the user for the name of the item to remove. The item is then removed
/// from the warehouse hashtable, as well as the locations hashtable if the item is in stock
/// If item does not exists in hashtable, the user is taken to the main menu
/// @param wh the warehouse hashtable
/// @param locations the locations hashtable
void remove_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

/// @brief asks the user for a name of an item. The user is then asked to input a new name,
/// description and price for the item, editing the specified item. Editing an item does
/// not effect it's stock or storage locations. If user tries to edit a non-existing item or if
/// the new name of the item is already in use, the user is taken to the main menu
/// @param wh the warehouse hashtable
/// @param locations the locations hashtable
void edit_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

/// @brief Shows the stock and storage locations, as well as the quantity of items in that location
/// of an item chosen by the user via name input. If a name is not in the warehouse, the user is taken to the main menu.
/// If an item is not in stock, a message will be printed and the user is taken to the main menu
/// @param wh the warehouse hashtable
/// @param locations the locations hashtable
void show_stock(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

/// @brief Asks the user for a storage location to insert an item, the name of the item to insert, as well as
/// the quantity of the specified item to insert in to the storage locations hashtable. An item that has been
/// inserted in the locations hashtable is considered to be in stock. If the name does not exist in the warehouse,
/// the user is taken to the main menu
/// @param wh the warehouse hashtable
/// @param locations the locations hashtable
void ioopm_replenish(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

/// @brief This function asks the user for a confirmation string.
/// @return if any string starting with 'y' or 'Y' is read, the function will return false, otherwise true
bool quit();

/// @brief A function that prints out all the input options for the event loop
void print_menu();

/// @brief A function that checks if option input is valid or not
/// @return string that the user inputs, if valid
char *ask_question_menu();

/// @brief An event loop that calls the different functions, depending on user inputs
/// @param wh the warehouse hashtable
/// @param locations the locations hashtable
void event_loop(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);