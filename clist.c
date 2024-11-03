/*
 * clist.c
 * 
 * Linked list implementation for ISSE Assignment 5
 *
 * Author: <your name here>
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "clist.h"

#define DEBUG

struct _cl_node {
  CListElementType element;
  struct _cl_node *next;
};

struct _clist {
  struct _cl_node *head;
  int length;
};



/*
 * Create (malloc) a new _cl_node and populate it with the supplied
 * values
 *
 * Parameters:
 *   element, next  the values for the node to be created
 * 
 * Returns: The newly-malloc'd node, or NULL in case of error
 */
static struct _cl_node*
_CL_new_node(CListElementType element, struct _cl_node *next)
{
  struct _cl_node* new = (struct _cl_node*) malloc(sizeof(struct _cl_node));

  assert(new);

  new->element = element;
  new->next = next;

  return new;
}



// Documented in .h file
CList CL_new()
{
  CList list = (CList) malloc(sizeof(struct _clist));
  assert(list);

  list->head = NULL;
  list->length = 0;

  return list;
}



// Documented in .h file
void CL_free(CList list) {
  struct _cl_node *current = list->head;
  struct _cl_node *next;

  while (current != NULL) {
    next = current->next;
    free((void *)current->element);
    free(current);
    current = next;
  }

  free(list);
}



// Documented in .h file
int CL_length(CList list)
{
  assert(list);
#ifdef DEBUG
  // In production code, we simply return the stored value for
  // length. However, as a defensive programming method to prevent
  // bugs in our code, in DEBUG mode we walk the list and ensure the
  // number of elements on the list is equal to the stored length.

  int len = 0;
  for (struct _cl_node *node = list->head; node != NULL; node = node->next)
    len++;

  assert(len == list->length);
#endif // DEBUG

  return list->length;
}



// Documented in .h file
void CL_print(CList list)
{
  assert(list);

  int num = 0;
  for (struct _cl_node *node = list->head; node != NULL; node = node->next)
    printf("  [%d]: %s\n", num++, node->element);
}



// Documented in .h file
void CL_push(CList list, CListElementType element)
{
  assert(list);
  list->head = _CL_new_node(element, list->head);
  list->length++;
}



// Documented in .h file
CListElementType CL_pop(CList list)
{
  assert(list);

  struct _cl_node *popped_node = list->head;

  if (popped_node == NULL)
    return INVALID_RETURN;

  CListElementType ret = popped_node->element;

  list->head = popped_node->next;
  free(popped_node);

  list->length--;

  return ret;
}



// Documented in .h file
void CL_append(CList list, CListElementType element)
{
    assert(list);  // Ensure the list is valid

    struct _cl_node *new_node = _CL_new_node(element, NULL);  // Create new node with no next node
    assert(new_node);  // Ensure the node was created successfully

    if (list->head == NULL) {
        // If the list is empty, the new node becomes the head
        list->head = new_node;
    } else {
        // Otherwise, traverse to the end of the list and append the new node
        struct _cl_node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->length++;  // Increment the length of the list
}





// Documented in .h file
CListElementType CL_nth(CList list, int pos) {
  assert(list);
  if (pos < 0) {
    pos += list->length;  // Handle negative indices
  }
  struct _cl_node *current = list->head;
  for (int i = 0; current != NULL && i < pos; i++) {
    current = current->next;
  }
  if (current == NULL) {
    return INVALID_RETURN;  // Out of bounds
  }
  return current->element;
}




// Documented in .h file
bool CL_insert(CList list, CListElementType element, int pos) {
  assert(list);  // Ensure the list is valid

  if (pos < 0) {
    pos = list->length + pos + 1;  // Convert negative index to positive
    if (pos < 0) return false;  // Out of range
  }

  if (pos == 0) {  // Insert at the head
    list->head = _CL_new_node(element, list->head);
    list->length++;
    return true;
  }

  struct _cl_node *current = list->head;
  for (int i = 0; current != NULL && i < pos - 1; i++) {
    current = current->next;
  }

  if (current == NULL) return false;  // Out of range

  // Normal insertion
  struct _cl_node *new_node = _CL_new_node(element, current->next);
  if (new_node == NULL) return false;  // Memory allocation failed

  current->next = new_node;
  list->length++;
  return true;
}



    
CListElementType CL_remove(CList list, int pos) {
  assert(list);  // Ensure the list is valid

  if (pos < 0) {
    pos = list->length + pos;  // Convert negative index to positive
    if (pos < 0) return INVALID_RETURN;  // Out of range
  }

  struct _cl_node *current = list->head, *prev = NULL;
  for (int i = 0; current != NULL && i < pos; i++) {
    prev = current;
    current = current->next;
  }

  if (current == NULL) return INVALID_RETURN;  // Out of range if current is NULL

  CListElementType ret = current->element;  // Store the data to be returned

  // If removing the first element
  if (prev == NULL) {
    list->head = current->next;
  } else {
    prev->next = current->next;  // Bypass the current node
  }

  free(current);  // Free the node
  list->length--;  // Decrement the length of the list

  return ret;  // Return the removed element
}




// Documented in .h file
CList CL_copy(CList src_list) {
  assert(src_list);  // Ensure the source list is valid

  CList new_list = CL_new();  // Create a new list
  if (src_list->head == NULL) return new_list;  // If source is empty, return empty list

  struct _cl_node *src_node = src_list->head;
  struct _cl_node *new_node = _CL_new_node(src_node->element, NULL);  // Copy the first node
  assert(new_node);  // Ensure the node was created successfully
  new_list->head = new_node;
  new_list->length = 1;

  src_node = src_node->next;  // Move to next node in source list

  struct _cl_node *last_node = new_node;  // Keep track of the last node in new list

  while (src_node != NULL) {
    new_node = _CL_new_node(src_node->element, NULL);  // Copy each node
    assert(new_node);  // Ensure the node was created successfully
    last_node->next = new_node;  // Link the new node to the list
    last_node = new_node;  // Update the last node pointer
    src_node = src_node->next;  // Move to next node in source list
    new_list->length++;  // Increment the length of the new list
  }

  return new_list;
}



int CL_insert_sorted(CList list, CListElementType element) {
  assert(list);

  struct _cl_node **tracer = &list->head;
  while (*tracer && strcmp((*tracer)->element, element) < 0) {
    tracer = &((*tracer)->next);
  }
  struct _cl_node *new_node = _CL_new_node(element, *tracer);
  if (!new_node) return -1;  // Failed to allocate memory
  *tracer = new_node;
  list->length++;
  return 0;  // Success
}



// Documented in .h file
void CL_join(CList list1, CList list2) {
  assert(list1);
  assert(list2);
  if (list2->head == NULL) return;  // Nothing to join
  struct _cl_node *current = list1->head;
  if (current == NULL) {
    list1->head = list2->head;  // Directly point head to list2's head if list1 is empty
  } else {
    while (current->next != NULL) {  // Find the end of list1
      current = current->next;
    }
    current->next = list2->head;  // Link the end of list1 to the start of list2
  }
  list1->length += list2->length;  // Update the length
  list2->head = NULL;  // Clear list2
  list2->length = 0;
}



// Documented in .h file
void CL_reverse(CList list) {
  assert(list);  // Ensure the list is valid

  struct _cl_node *prev = NULL;
  struct _cl_node *current = list->head;
  struct _cl_node *next = NULL;

  while (current != NULL) {
    next = current->next;  // Store next node
    current->next = prev;  // Reverse current node's pointer
    prev = current;  // Move prev and current one step forward
    current = next;
  }
  list->head = prev;  // Reset head to the new first element
}




// Documented in .h file
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data) {
  int pos = 0;
  struct _cl_node *current = list->head;
  while (current != NULL) {
    callback(pos, current->element, cb_data);
    current = current->next;
    pos++;
  }
}





