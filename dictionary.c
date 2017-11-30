/**
 * Implements a dictionary's functionality.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"
#include "hashtable.h"

#define EOL '\n'
#define MASK 32

static hashtable_t htable; //hash tabe that is visible only in this file (?)

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    assert(word != NULL);
    char* p_text = strdup(word);
    for(int i = 0; p_text[i] != '\0'; i++)
        p_text[i] = tolower(p_text[i]);
    node_t* found = find_ht(p_text, htable);
    free(p_text);
    if (found == NULL) return false;
    else return true;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE* pfile = fopen(dictionary, "r");
    if (pfile == NULL)
        return false;
    char c; // current character
    char line[LENGTH + 1]; // current line which is one word and cannot be longer than 45 letters
    htable = new_htable_default();
    int i = 0; // character in a current word
    while ((c = fgetc(pfile)) != EOF) {
        if (c != EOL) {
            line[i++] = c; // better to do tolower(c) here
        }
        else add_word(line, &i);
    }
    fclose(pfile);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return htable.logical_size;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    delete_htable(&htable);
    return true;
}

void add_word(char line[], int* size_ptr) {
    char* str; // pointer to the string in heap
    // finish the string
    line[*size_ptr] = '\0';
    // now copy the string to some heap location
    str = strdup(line);
    assert(str != NULL);
    insert_ht(str, &htable);
    (*size_ptr) = 0; //start a new word
}