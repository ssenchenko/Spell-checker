#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <string.h>

#define DEFAULT_HTSIZE (1 << 16) - 15 // 2^18 - 5 - prime number which is more enough for our dictionary

// node for a hashtable with separate chaining 
typedef struct __node {
	char* p_text;
	struct __node* prior;
	struct __node* next;
} node_t;

// hashtable which is an array
typedef struct __hashtable {
	node_t** array; // array of node_t*
	size_t alloc_size; // allocated size
	size_t logical_size; // number of elements in hashtable
} hashtable_t;

typedef struct {
	node_t* p_element; // pointer to the inserted element or the element that prevented insertion
	bool succeeded; // true if element was inserted
} result_t;

// universal hash function for strings
int hash_u(const char *p_text, size_t M);

// create a new hashtable and allocate space for it
hashtable_t new_htable(size_t sz);

// default hashtable
hashtable_t new_htable_default(void);

// insert a new string to a hashtable
// it creates unordered MULTImap which allows duplicates
void insert_ht(const char* p_text, hashtable_t* p_htable);

// check if the element is in the hashtable
// returns a pointer to the element if found or NULL otherwise
node_t* find_ht(const char* p_text, hashtable_t htable);

// dispose hashtable memory and all memory for its content
void delete_htable(hashtable_t* p_htable);

// ============================= functions to work with lists ===================================
/* search the list
 * input: 
 * 		pointer to the string to look up
 *		pointer to the first element of the list
 * output:
 *		pointer to the found element or to the las element in the list if the element has not be found
 *		true if element has been found; false means that the pointer points to the last element of the list
 */
result_t find_l(const char* p_text, node_t* p_first);

/* inserts element AFTER the specified element
 * if element is null than NO insertion happens
 * input:
 * 		pointer to the text to insert
 * 		pointer to the element of the list
 * output:
 *		pointer to the inserted element
 */
 node_t* insert_l(const char* p_text, node_t* p_element);
 
 /* inserts element in the beginning of the list
 * input:
 * 		pointer to the text to insert
 * 		pointer to the first element
 * output:
 *		pointer to the new first element of the least which is freshly inserted element
 */
 node_t* push_front(const char* p_text, node_t* p_first);

 // allocates memory for a new node and 
 // stores there a a pointer to a string
 node_t* new_node(const char* p_text);

 /* delete the list and it's content
  * input:
  * 	pointer to the first element of the list
  */
 void delete_nodes(node_t* p_first);

#endif //HASHTABLE_H