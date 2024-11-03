/*
 * clist_test.c
 * 
 * Automated test code for CLists
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "clist.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "clist.h"


// Define the INVALID_RETURN for the tests that use it
#define INVALID_RETURN NULL

// Helper function to log errors and continue execution
void log_error(const char *message, const char *file, int line) {
  fprintf(stderr, "Error: %s in %s at line %d\n", message, file, line);
}

// Wrapper function to handle test execution and error logging
int run_test(int (*test_func)(), const char *name) {
  int result = 0;
  printf("Running %s...\n", name);
  result = test_func();
  if (!result) {
    log_error("Test failed", __FILE__, __LINE__);
  } else {
    printf("%s passed.\n", name);
  }
  return result;
}

// Some known testdata, for testing
const char *testdata[] = {"Zero", "One", "Two", "Three", "Four", "Five",
  "Six", "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen",
  "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen",
  "Twenty"};

const char *testdata_sorted[] = {"Eight", "Eighteen", "Eleven", "Fifteen",
  "Five", "Four", "Fourteen", "Nine", "Nineteen", "One", "Seven",
  "Seventeen", "Six", "Sixteen", "Ten", "Thirteen", "Three", "Twelve",
  "Twenty", "Two", "Zero"};

static const int num_testdata = sizeof(testdata) / sizeof(testdata[0]);


// Checks that value is true; if not, prints a failure message and
// returns 0 from this function
#define test_assert(value) {                                            \
    if (!(value)) {                                                     \
      printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value);      \
      goto test_error;                                                  \
    }                                                                   \
  }

// Checks that value == INVALID_RETURN; if not, prints a failure
// message and returns 0 from this function
#define test_invalid(value) {                                           \
    if (value != INVALID_RETURN) {                                      \
      printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value);      \
      goto test_error;                                                  \
    }                                                                   \
  }

// Checks that value == expected; if not, prints a failure message and
// returns 0 from this function
#define test_compare(value, expected) {                           \
    const char *v = (value);                                      \
    const char *e = (expected);                                   \
    if (strcmp(v, e) != 0) {                                      \
      printf("FAIL %s[%d] %s: expected '%s', got '%s'\n",         \
          __FUNCTION__, __LINE__, #value, e, v);                  \
      goto test_error;                                            \
    }                                                             \
  }


/*
 * Tests the CL_new, CL_push, CL_pop, and CL_free functions
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_push_pop()
{
  int ret = 0;
  CList list = CL_new();

  test_assert( CL_length(list) == 0 );
  test_invalid( CL_pop(list) );

  CL_push(list, testdata[0]);

  test_assert( CL_length(list) == 1 );
  test_compare( CL_pop(list), testdata[0] );
  test_assert( CL_length(list) == 0 );
  test_invalid( CL_pop(list) );

  for (int i=0; i < num_testdata; i++) {
    CL_push(list, testdata[i]);
    test_assert( CL_length(list) == i+1 );
  }

  for (int i=num_testdata-1; i >= 0; i--) {
    test_compare( CL_pop(list), testdata[i] );
    test_assert( CL_length(list) == i );
  }

  ret = 1;

 test_error:
  CL_free(list);
  return ret;
}

// Mine
int test_CL_append() {
  printf("Testing CL_append...\n");

  CList list = CL_new();  // Create a new list
  assert(CL_length(list) == 0);  // Initially, the list should be empty

  // Append elements to the list
  CL_append(list, "alpha");
  CL_append(list, "bravo");
  CL_append(list, "charlie");

  // Check if the list length is correct
  assert(CL_length(list) == 3);

  // Verify the order of elements
  assert(strcmp(CL_nth(list, 0), "alpha") == 0);
  assert(strcmp(CL_nth(list, 1), "bravo") == 0);
  assert(strcmp(CL_nth(list, 2), "charlie") == 0);

  // Print results to verify visually in DEBUG mode
  printf("List after appending: ");
  CL_print(list);

  // Free the list at the end of the test
  CL_free(list);

  printf("CL_append test passed.\n");
  return 1;
}


int test_cl_append()
{
  int ret = 0;
  CList list = CL_new();
  
  // Append all the items
  for (int i=0; i < num_testdata; i++) {
    CL_append(list, testdata[i]);
    test_assert( CL_length(list) == i+1 );
  }

  // the list is in the right order
  for (int i=0; i < num_testdata; i++)
    test_compare( CL_nth(list, i), testdata[i] );

  ret = 1;

 test_error:
  CL_free(list);
  return ret;
}


/*
 * Tests the CL_nth function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_nth()
{
  int ret = 0;
  CList list = CL_new();
  
  // Empty list -- ensure that CL_nth returns NULL for -2, -1, 0, 1, and 2
  test_invalid( CL_nth(list, -2) );
  test_invalid( CL_nth(list, -1) );
  test_invalid( CL_nth(list, 0) );
  test_invalid( CL_nth(list, 1) );
  test_invalid( CL_nth(list, 2) );

  // Add one item, then perform same tests
  CL_push(list, testdata[2]);
  test_assert( CL_length(list) == 1 );
  test_invalid( CL_nth(list, -3) );
  test_invalid( CL_nth(list, -2) );
  test_compare( CL_nth(list, -1), testdata[2]);
  test_compare( CL_nth(list, 0), testdata[2]);
  test_invalid( CL_nth(list, 1) );
  test_invalid( CL_nth(list, 2) );

  // Push two more items, perform similar tests 
  CL_push(list, testdata[1]);
  CL_push(list, testdata[0]);
  test_assert( CL_length(list) == 3 );
  test_invalid( CL_nth(list, -5) );
  test_invalid( CL_nth(list, -4) );
  test_compare( CL_nth(list, -3), testdata[0]);
  test_compare( CL_nth(list, -2), testdata[1]);
  test_compare( CL_nth(list, -1), testdata[2]);
  test_compare( CL_nth(list, 0), testdata[0]);
  test_compare( CL_nth(list, 1), testdata[1]);
  test_compare( CL_nth(list, 2), testdata[2]);
  test_invalid( CL_nth(list, 3) );
  test_invalid( CL_nth(list, 4) );

  ret = 1;

 test_error:
  CL_free(list);
  return ret;
}


/*
 * A demonstration of how to use a CList, which also doubles as a
 * test case.
 *
 * Returns: 1 if all tests pass, 0 otherwise.
 */
int sample_clist_usage()
{
  int ret = 0;
  CList list = CL_new();

  // new lists have length 0
  test_assert( CL_length(list) == 0 );

  CL_push(list, "alpha");       // push 'alpha' onto front of list
  CL_push(list, "bravo");       // push 'bravo' onto front of list
  CL_push(list, "charlie");     // push 'charlie' onto front of list

  // list is now charlie, bravo, alpha

  CL_print(list);               // print out the list: charlie, bravo, alpha

  test_assert( CL_length(list) == 3 );

  // pop the element off the front of the list, which should be 'charlie'
  test_compare( CL_pop(list), "charlie" );
  test_assert( CL_length(list) == 2 );

  CL_insert(list, "delta", 2);    // insert 'delta' at position 2
  CL_append(list, "echo");        // append 'echo' at end of list
  CL_insert(list, "foxtrot", -2); // insert 'foxtrot' one before end

  // list is now: bravo, alpha, delta, foxtrot, echo
  test_assert( CL_length(list) == 5 );

  // retrieve the 3rd element, numbering from 0, so it should be foxtrot
  test_compare( CL_nth(list, 3), "foxtrot" );
  
  // list hasn't changed
  test_assert( CL_length(list) == 5 );

  // now REMOVE the third element
  test_compare( CL_remove(list, 3), "foxtrot" );
  test_assert( CL_length(list) == 4 );

  // list is now: bravo, alpha, delta, echo

  // make a copy of the list
  CList list_copy = CL_copy(list);

  test_assert( CL_length(list_copy) == 4 );

  // reverse the copy, so it is now echo, delta, alpha, bravo
  CL_reverse(list_copy);

  // remove the first item from the copy
  test_compare( CL_remove(list_copy, 0), "echo" );
  test_assert( CL_length(list_copy) == 3 );
  
  // original list should be unchanged
  test_assert( CL_length(list) == 4 );
  
  // join the two lists; note this operation empties list_copy
  CL_join(list, list_copy);

  // list is now: bravo, alpha, delta, echo, delta, alpha, bravo
  // list_copy should be empty
  test_assert( CL_length(list) == 7 );
  test_compare( CL_nth(list, 3), "echo" );
  test_assert( CL_length(list_copy) == 0);

  ret = 1;

 test_error:
  // Lists must be destroyed
  CL_free(list);
  CL_free(list_copy);
  return ret;
}

//Mine
int test_CL_insert() {
  printf("Testing CL_insert...\n");
  int result = 1;  
  CList list = CL_new();

  // Test inserting at the beginning
  if (!CL_insert(list, "charlie", 0)) {
    printf("Failed to insert 'charlie' at position 0.\n");
    result = 0;  // failure
  }

  if (!CL_insert(list, "bravo", 0)) {
    printf("Failed to insert 'bravo' at position 0.\n");
    result = 0;  // failure
  }

  if (!CL_insert(list, "alpha", 0)) {
    printf("Failed to insert 'alpha' at position 0.\n");
    result = 0;  //  failure
  }

  // Verify insertion results
  const char* item = CL_nth(list, 0);
  if (strcmp(item, "alpha") != 0) {
    printf("Incorrect item at position 0: expected 'alpha', got '%s'.\n", item);
    result = 0;  // failure
  }

  // Test inserting at the end (index equal to length of list)
  if (!CL_insert(list, "delta", CL_length(list))) {
    printf("Failed to insert 'delta' at the end.\n");
    result = 0;  //    failure
  }

  // Test inserting in the middle
  if (!CL_insert(list, "beta", 2)) {
    printf("Failed to insert 'beta' at position 2.\n");
    result = 0;  // failure
  }

  // Test inserting out of bounds, should fail
  if (CL_insert(list, "echo", -10)) {
    printf("Insertion out of bounds (-10) should not succeed.\n");
    result = 0;  // failure
  }

  if (CL_insert(list, "foxtrot", 100)) {
    printf("Insertion out of bounds (100) should not succeed.\n");
    result = 0;  // failure
  }

  CL_print(list); 
  CL_free(list);
  printf("CL_insert test %s.\n", result ? "passed" : "failed");
  return result;
}



//Mine
int test_CL_remove() {
  printf("Testing CL_remove...\n");
  int result = 1;   
  CList list = CL_new();

  // Populate the list
  CL_append(list, "alpha");
  CL_append(list, "beta");
  CL_append(list, "gamma");
  CL_append(list, "delta");

  // Test removing the head
  const char* removed = CL_remove(list, 0);
  if (!removed || strcmp(removed, "alpha") != 0) {
    printf("Failed to remove the head: expected 'alpha', got '%s'.\n", removed ? removed : "NULL");
    result = 0;  //    failure
  }

  // Test removing from middle
  removed = CL_remove(list, 1);  // After removal of 'alpha', 'gamma' is now at index 1
  if (!removed || strcmp(removed, "gamma") != 0) {
    printf("Failed to remove from middle: expected 'gamma', got '%s'.\n", removed ? removed : "NULL");
    result = 0;  //    failure
  }

  // Test removing the last element
  removed = CL_remove(list, 1);  // Now 'delta' should be at index 1 after removing 'gamma'
  if (!removed || strcmp(removed, "delta") != 0) {
    printf("Failed to remove the last element: expected 'delta', got '%s'.\n", removed ? removed : "NULL");
    result = 0;  //    failure
  }

  // Verify final list structure and length
  if (CL_length(list) != 1 || strcmp(CL_nth(list, 0), "beta") != 0) {
    printf("List structure incorrect after removals: expected length 1 with 'beta', got length %d.\n", CL_length(list));
    result = 0;  //    failure
  }

  CL_print(list);
  CL_free(list);
  printf("CL_remove test %s.\n", result ? "passed" : "failed");
  return result;
}



//Mine
int test_CL_copy() {
  printf("Testing CL_copy...\n");
  int result = 1;   
  CList original = CL_new();
  CL_append(original, "alpha");
  CL_append(original, "beta");

  // Copy the list
  CList copied = CL_copy(original);

  // Check initial identical content
  if (CL_length(copied) != 2 || strcmp(CL_nth(copied, 0), "alpha") != 0 || strcmp(CL_nth(copied, 1), "beta") != 0) {
    printf("Copied list content mismatch or incorrect length.\n");
    result = 0;  //    failure
  }

  // Modify original
  CL_append(original, "gamma");

  // Ensure copied list is unaffected by changes to original
  if (CL_length(copied) != 2) {
    printf("Copied list was affected by modifications to the original list.\n");
    result = 0;  //    failure
  }

  // Cleanup
  CL_free(original);
  CL_free(copied);

  printf("CL_copy test %s.\n", result ? "passed" : "failed");
  return result;
}



//Mine
int test_CL_reverse() {
  printf("Testing CL_reverse...\n");
  int result = 1;   
  CList list = CL_new();
  CL_append(list, "alpha");
  CL_append(list, "beta");
  CL_append(list, "gamma");

  // Reverse the list
  CL_reverse(list);

  // Verify the order is reversed
  if (strcmp(CL_nth(list, 0), "gamma") != 0 ||
      strcmp(CL_nth(list, 1), "beta") != 0 ||
      strcmp(CL_nth(list, 2), "alpha") != 0) {
    printf("List order after reverse is incorrect.\n");
    printf("Expected order: 'gamma', 'beta', 'alpha'. Actual order: '%s', '%s', '%s'.\n",
           CL_nth(list, 0), CL_nth(list, 1), CL_nth(list, 2));
    result = 0;  //    failure
      }

  printf("List after reversal: ");
  CL_print(list);

  // Clean up
  CL_free(list);
  printf("CL_reverse test %s.\n", result ? "passed" : "failed");
  return result;
}


//Mine for testing
void print_element(int pos, const char *element, void *cb_data) {
  printf("%d: %s %s\n", pos, element, (const char *) cb_data);
}

// same
void callback(int pos, const char *element, void *cb_data) {
  printf("%d: %s %s\n", pos, element, (const char *) cb_data);
  int *result = (int *) cb_data;
  const char *expected_elements[] = {"alpha", "beta", "gamma"};

  // Check if the position and element match expected values
  if (strcmp(element, expected_elements[pos]) != 0) {
    printf("Element mismatch at position %d: expected '%s', got '%s'.\n", pos, expected_elements[pos], element);
    *result = 0;  //    failure
  }
}


//Mine
int test_CL_foreach() {
  printf("Testing CL_foreach...\n");
  int result = 1;   
  CList list = CL_new();
  CL_append(list, "alpha");
  CL_append(list, "beta");
  CL_append(list, "gamma");

  // Apply CL_foreach with the callback and pass result as part of callback data
  CL_foreach(list, callback, &result);

  if (result) {
    printf("CL_foreach test passed.\n");
  } else {
    printf("CL_foreach test failed.\n");
  }

  CL_free(list);
  return result;
}





int main() {
  int passed = 0;
  int num_tests = 0;

  passed += run_test(test_cl_push_pop, "test_cl_push_pop");
  passed += run_test(test_cl_append, "test_cl_append");
  passed += run_test(test_cl_nth, "test_cl_nth");
  passed += run_test(sample_clist_usage, "sample_clist_usage");
  passed += run_test(test_CL_insert, "test_CL_insert");
  passed += run_test(test_CL_remove, "test_CL_remove");
  passed += run_test(test_CL_copy, "test_CL_copy");
  passed += run_test(test_CL_reverse, "test_CL_reverse");
  passed += run_test(test_CL_foreach, "test_CL_foreach");

  num_tests = 9;

  printf("Passed %d/%d test cases\n", passed, num_tests);
  fflush(stdout);
  return (passed == num_tests) ? 0 : 1;
}
