#include "hashtable.h"

#include <assert.h>
#include <stdlib.h>

hashtable_t new_htable(size_t sz) {
	hashtable_t htable;
	htable.array = calloc(sz, sizeof(node_t*));
	assert(htable.array != NULL);
	htable.alloc_size = sz;
	htable.logical_size = 0;
	return htable;
}

hashtable_t new_htable_default(void) {
	return new_htable(DEFAULT_HTSIZE);
}

int hash_u (const char *p_text, size_t M) {
    int h, a = 31415, b = 27183;
    for (h = 0; *p_text != '\0'; p_text++, a = a * b % (M - 1))
        h = (a * h + *p_text) % M;
    return h;
}

void insert_ht(const char* p_text, hashtable_t* p_htable) {
	int index = hash_u(p_text, p_htable->alloc_size);
	if (p_htable->array[index] != NULL) {
	    // after front_push the array has to point to a new first element
		p_htable->array[index] = push_front(p_text, p_htable->array[index]);
	}
	else {
		p_htable->array[index] = new_node(p_text);	
	}
	p_htable->logical_size++;
}

node_t* find_ht(const char* p_text, hashtable_t htable) {
	int index = hash_u(p_text, htable.alloc_size);
	if (htable.array[index]) {
		result_t found = find_l(p_text, htable.array[index]);
		if (found.succeeded) return found.p_element;
	}
	return NULL;
}

void delete_htable(hashtable_t *p_htable) {
	assert(p_htable->array != NULL);
	for (int i = 0; i < p_htable->alloc_size; i++) {
		if (p_htable->array[i]) delete_nodes(p_htable->array[i]);
	}
	free(p_htable->array);
	p_htable->alloc_size = 0;
	p_htable->logical_size = 0;
}

// ======================================== list functions ===============================================

result_t find_l(const char* p_text, node_t* p_first) {
	assert(p_first != NULL);
	result_t found = { .p_element = NULL, .succeeded = false };
	if (strcmp(p_text, p_first->p_text) == 0) {
		found.p_element = p_first;
		found.succeeded = true;
		return found;
	}
	else if (p_first->next == NULL) {
		found.p_element = p_first;
		// found.succeeded == false;
		return found;
	}
	else
		return find_l(p_text, p_first->next);
}

// insertion AFTER the specified element
node_t* insert_l(const char* p_text, node_t* p_element) {
	assert(p_element != NULL);
	// create a new node
	node_t* node_ptr = new_node(p_text);
	if (p_element->next == NULL) {
		node_ptr->prior = p_element;
		p_element->next = node_ptr;
	}
	else {
		node_t* tmp_ptr = p_element->next;
		p_element->next = node_ptr;
		node_ptr->prior = p_element;
		node_ptr->next = tmp_ptr;
		node_ptr->next->prior = node_ptr;
	}
	return node_ptr;	
}

node_t* push_front(const char* p_text, node_t* p_first) {
	assert(p_first != NULL);
	node_t* node_ptr = new_node(p_text);
	node_ptr->next = p_first;
	p_first->prior = node_ptr;
	return node_ptr;
}

node_t* new_node(const char* p_text) {
	node_t* p_element = calloc(1, sizeof(node_t));
	if (p_text) p_element->p_text = (char*)p_text;
	return p_element;
}

void delete_nodes(node_t* p_first) {
	assert(p_first != NULL);
	node_t* next = p_first->next;
	free(p_first->p_text);
	free(p_first);
	if (next != NULL) delete_nodes(next);
}