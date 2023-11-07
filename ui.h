#pragma once
#include "linked_list.h"
#include "common.h"
#include "hash_table.h"
#include "logic.h"


void ioopm_add_merchandise(ioopm_hash_table_t *wh);

void list_merchandise(ioopm_hash_table_t *wh);

void remove_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

void edit_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

void show_stock(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

void ioopm_replenish(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);

bool quit();

void print_menu();

char *ask_question_menu();

void event_loop(ioopm_hash_table_t *wh, ioopm_hash_table_t *locations);