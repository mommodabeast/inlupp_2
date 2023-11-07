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
#include "ui.h"


int main() {
    ioopm_hash_table_t *wh = ioopm_hash_table_create(ioopm_str_eq);
    ioopm_hash_table_t *locations = ioopm_hash_table_create(ioopm_str_eq);
    wh->hash_func = string_sum_hash;
    locations->hash_func = string_sum_hash;

    event_loop(wh, locations);

    free_and_destroy_hts(wh, locations);

    return 0;
}