#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logic.h"
#include "ui.h"

void header()
{
    printf("   _________________\n");
    printf("  /                /|\n");
    printf(" /   WAREHOUSE    / |\n");
    printf("/________________/  |\n");
    printf("| __ __ __ __ __ |  /\n");
    printf("| |__|__|__|__| || /\n");
    printf("|________________|/\n");
    printf("\n");
}
void display_menu() {
    printf("\n=== Warehouse Management System Menu ===\n");
    printf("A/a - Add Merchandise\n");
    printf("P/p - Replenish Merchandise\n");
    printf("L/l - List Merchandise\n");
    printf("S/s - Show Stock\n");
    printf("C/c - Create Cart\n");
    printf("R/r - Remove Cart\n");
    printf("+   - Add to Cart\n");
    printf("-   - Remove from Cart\n");
    printf("=   - Calculate Cost\n");
    printf("0   - Checkout\n");
    printf("Q/q - Exit\n");
    printf("========================================\n");
}

int main() {
        header();

    ioopm_hash_table_t *wh = ioopm_hash_table_create(ioopm_str_eq);
    ioopm_hash_table_t *locations = ioopm_hash_table_create(ioopm_str_eq);
    wh->hash_func = string_sum_hash;
    locations->hash_func = string_sum_hash;

    char action;
    do {
        display_menu();
        printf("Enter action code: ");
        scanf(" %c", &action);

        switch (action) {
            case 'A':
            case 'a':
                ioopm_add_merchandise(wh);
                break;
            case 'P':
            case 'p':
                ioopm_replenish(wh, locations);
                break;
            case 'L':
            case 'l':
                list_merchandise(wh);
                break;
            case 'S':
            case 's':
                show_stock(wh, locations);
                break;
            case 'C':
            case 'c':
                create_cart();
                break;
            case 'R':
            case 'r':
                remove_cart();
                break;
            case '+':
                add_to_cart(wh);
                break;
            case '-':
                remove_from_cart();
                break;
            case '=':
                calculate_cost();
                break;
            case 'O':
            case 'o':
                checkout(wh);
                break;
            case 'Q':
            case 'q':
                // Exit the program
                break;
            default:
                printf("Invalid action code. Please try again.\n");
        }
    } while (action != 'Q' && action != 'q');

    // Cleanup resources
    ioopm_hash_table_apply_to_all(wh, free_void_ptr_value, NULL);
    ioopm_hash_table_apply_to_all(wh, free_void_ptr_key, NULL);
    ioopm_hash_table_destroy(wh);
    ioopm_hash_table_destroy(locations);


    return 0;
}
