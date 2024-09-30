/*
 * clist.c
 * 
 * Linked list implementation for ISSE Assignment 5
 *
 * Author: AHMED MOHAMED (amohame2)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "clist.h"

#define MAX_PTRS 1024
#define DEBUG

void* valid_ptrs[MAX_PTRS];
int num_valid_ptrs = 0;

/*
 * Create (malloc) a new _cl_node and populate it with the supplied
 * values
 *
 * Parameters:
 *   element, next  the values for the node to be created
 * 
 * Returns: The newly-malloc'd node, or NULL in case of error
 */
// static struct _cl_node* _CL_new_node(CListElementType element, struct _cl_node *next)
// {
//   struct _cl_node* new = (struct _cl_node*) malloc(sizeof(struct _cl_node));
//
//   assert(new);
//
//   new->element = element;
//   new->next = next;
//
//   return new;
// }

static struct _cl_node* _CL_new_node(CListElementType element, struct _cl_node *next) {
  struct _cl_node* new = (struct _cl_node*) malloc(sizeof(struct _cl_node));
  assert(new);

  new->element = strdup(element);
  new->next = next;

  return new;
}



// Documented in .h file
CList CL_new() {
  CList list = malloc(sizeof(*list));
  if (list == NULL) {
    return NULL;
  }
  list->head = NULL;
  list->length = 0;
  return list;
}

void register_ptr(void* ptr) {
  if (num_valid_ptrs < MAX_PTRS) {
    valid_ptrs[num_valid_ptrs++] = ptr;
  }
}

void unregister_ptr(void* ptr) {
  for (int i = 0; i < num_valid_ptrs; i++) {
    if (valid_ptrs[i] == ptr) {
      valid_ptrs[i] = valid_ptrs[--num_valid_ptrs];
      break;
    }
  }
}


void* debug_malloc(size_t size) {
  void* ptr = malloc(size);
  if (ptr) {
    register_ptr(ptr);
  }
  return ptr;
}

void debug_free(void* ptr) {
  unregister_ptr(ptr);
  free(ptr);
}


bool IsValidMemory(void* ptr) {
  for (int i = 0; i < num_valid_ptrs; i++) {
    if (valid_ptrs[i] == ptr) {
      return true;
    }
  }
  return false;
}

// Documented in .h file
void CL_free(CList list) {

  assert(list);
  struct _cl_node* current = list->head;
  if (current == NULL) {
    return;
  }

  struct _cl_node *next_node;

  while (current != NULL) {
    if (!IsValidMemory(current)) {
      break; // memory is corrupted
    }
    next_node = current->next;
    free(current->element);
    free(current);
    current = next_node;
  }
  if (!IsValidMemory(list)) {
    return;  // Exit & avoid crashing
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

  // unlink previous head node, then free it
  list->head = popped_node->next;
  free(popped_node);
  // we cannot refer to popped node any longer

  list->length--;

  return ret;
}



// Documented in .h file
void CL_append(CList list, CListElementType element)
{
  assert(list);

  struct _cl_node *new_node = _CL_new_node(element, NULL);
  assert(new_node);  // new node created successfully

  if (list->head == NULL) {

    list->head = new_node;
  } else {
    // other --> find last node
    struct _cl_node *current = list->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
  }

  list->length++;  // Increment list len
}





// Documented in .h file
CListElementType CL_nth(CList list, int pos) {
  if (pos < 0) {
    pos = list->length + pos; // Convert - index to +
  }
  if (pos < 0 || pos >= list->length) {
    fprintf(stderr, "Index out of bounds\n");
    return NULL; //
  }

  struct _cl_node *current = list->head;
  for (int i = 0; i < pos; i++) {
    if (current == NULL) {
      fprintf(stderr, "Unexpected NULL node in list\n");
      return NULL;
    }
    current = current->next;
  }
  return current->element;
}




// Documented in .h file
bool CL_insert(CList list, CListElementType element, int pos) {
  if (pos < 0 || pos > list->length) {
    fprintf(stderr, "Index out of range\n");
    return false;
  }

  if (pos == 0) {  // Insert at head
    CL_push(list, element);
    return true;
  }

  struct _cl_node* current = list->head;
  for (int i = 0; i < pos - 1; i++) {
    current = current->next;
  }

  struct _cl_node* new_node = _CL_new_node(element, current->next);


  current->next = new_node;
  list->length++;
  return true;
}



    
// Documented in .h file
CListElementType CL_remove(CList list, int pos) {
  if (pos < 0 || pos >= list->length) {
    fprintf(stderr, "Index out of bounds\n"); // reused
    return INVALID_RETURN;
  }

  if (pos == 0) {
    return CL_pop(list);
  }

  struct _cl_node* current = list->head;
  for (int i = 0; i < pos - 1; i++) {
    current = current->next;
  }

  struct _cl_node* target_node = current->next;
  CListElementType removed_element = target_node->element;
  current->next = target_node->next;
  free(target_node->element);
  free(target_node);
  list->length--;

  return removed_element;
}




// Documented in .h file
int CL_insert_sorted(CList list, CListElementType element) {
  assert(list);

  if (list->head == NULL || element < list->head->element) {
    CL_push(list, element);
    return 0;
  }

  struct _cl_node* current = list->head;
  struct _cl_node* previous = NULL;
  int index = 0;

  while (current != NULL && current->element < element) {
    previous = current;
    current = current->next;
    index++;
  }

  struct _cl_node* new_node = _CL_new_node(element, current);
  previous->next = new_node;
  list->length++;
  return index;
}



// Documented in .h file
void CL_join(CList list1, CList list2) {
  if (list2->head == NULL) {  //if list2 empty, do none
    return;
  }

  if (list1->head == NULL) {  //if list1 empty, take all list2
    list1->head = list2->head;
  } else {
    struct _cl_node* current = list1->head;
    while (current->next != NULL) {
      current = current->next;  // find last node of list1
    }
    current->next = list2->head;  //connect to list2 head
  }

  list1->length += list2->length;  // Update length

  // clear list2
  list2->head = NULL;
  list2->length = 0;
}



// Documented in .h file
// void CL_reverse(CList list)
void CL_reverse(CList list) {
  assert(list);

  struct _cl_node *prev = NULL;
  struct _cl_node *current = list->head;
  struct _cl_node *next = NULL;

  while (current != NULL) {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }

  list->head = prev;
}



// Documented in .h file
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data) {
  assert(list);

  struct _cl_node *current = list->head;
  int index = 0;

  while (current != NULL) {
    callback(index, current->element, cb_data);
    current = current->next;
    index++;
  }
}




CList CL_copy(CList src_list) {
  assert(src_list);

  CList new_list = CL_new();
  if (src_list->head == NULL) {
    return new_list;
  }

  struct _cl_node* src_current = src_list->head;
  struct _cl_node* new_tail = _CL_new_node(src_current->element, NULL);

  new_list->head = new_tail;
  src_current = src_current->next;

  while (src_current != NULL) {
    struct _cl_node* new_node = _CL_new_node(src_current->element, NULL);

    new_tail->next = new_node;
    new_tail = new_node;
    src_current = src_current->next;
  }

  new_list->length = src_list->length;
  return new_list;
}


// helpers that I was debugging with

// Check if a list is empty
int CL_is_empty(CList list) {
  return (list == NULL || list->head == NULL);
}

// get the element of the head node
const char* CL_head_element(CList list) {
  if (list != NULL && list->head != NULL) {
    return list->head->element;
  }
  return NULL;
}
