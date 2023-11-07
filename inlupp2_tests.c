#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "logic.h"
#include "ui.h"

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// These are example test functions. You should replace them with
// functions of your own.

void test_basic_functions() {
  merch_t *merch = ioopm_merch_create();
  ioopm_make_merch(merch, "Mjölk", "1 liter mellanmjölk", 14);
  CU_ASSERT_EQUAL(merch->name, "Mjölk");
  ioopm_hash_table_t *wh = ioopm_hash_table_create(ioopm_str_eq);
  ioopm_hash_table_t *locations = ioopm_hash_table_create(ioopm_str_eq);
  ioopm_hash_table_insert(wh, (elem_t) {.string = "Mjölk"}, (elem_t) {.void_pointer = merch});
  ioopm_list_t *merch_list = ioopm_hash_table_values(wh);
  elem_t milk = ioopm_linked_list_get(merch_list, 0);
  merch_t *merchandise = milk.void_pointer;
  CU_ASSERT_EQUAL(merchandise, merch);
  CU_ASSERT_EQUAL(merchandise->name, "Mjölk");
  CU_ASSERT_EQUAL(merchandise->information, "1 liter mellanmjölk");
  CU_ASSERT_EQUAL(merchandise->price, 14);

  ioopm_merch_destroy(merch);
  ioopm_linked_list_destroy(merch_list);
  ioopm_hash_table_destroy(wh);
  ioopm_hash_table_destroy(locations);
}

void test_add_merchandise() {
  ioopm_hash_table_t *wh = ioopm_hash_table_create(ioopm_str_eq);
  ioopm_hash_table_t *locations = ioopm_hash_table_create(ioopm_str_eq);
  wh->hash_func = string_sum_hash;
  locations->hash_func = string_sum_hash;
  elem_t *result = calloc(1, sizeof(elem_t));
  elem_t milk = {.string = "milk"};

  ioopm_add_merchandise(wh);
  bool merch_exist = ioopm_hash_table_lookup(wh, milk, result);
  CU_ASSERT_TRUE(merch_exist);
  merch_t *merch = result->void_pointer;
  CU_ASSERT_STRING_EQUAL(merch->name, "milk");
  CU_ASSERT_EQUAL(merch->price, 12);
  CU_ASSERT_STRING_EQUAL(merch->information, "1l milk");
  CU_ASSERT_EQUAL(merch->locations, NULL);
  CU_ASSERT_EQUAL(merch->stock, 0);

  free(result);
  ioopm_hash_table_apply_to_all(wh, free_void_ptr_value, NULL);
  ioopm_hash_table_apply_to_all(wh, free_void_ptr_key, NULL);
  ioopm_hash_table_destroy(wh);
  ioopm_hash_table_destroy(locations);
  
}

int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
    (CU_add_test(my_test_suite, "Test basic functions", test_basic_functions) == NULL) ||
    (CU_add_test(my_test_suite, "Test add merchandise", test_add_merchandise) == NULL) ||
    
    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_NORMAL);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}