#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"
#define EMPTY 1
#define FILLED 2
#define DELETED 3

//Lab 3 by Jeremy Akey, Due May 1st 2022
//For this lab I used a hash function to create a SET ADT to handle strings.
static int search(SET*, char*, bool*);
unsigned strhash(char *);

typedef struct set{
    char** data;
    int* flags;
    int length;
    int count;
}SET;

//Function to create the set and set the flags array to empty, with the size being determined by the maxElts in the other programs.
//Efficiency: O(1)
SET *createSet(int maxElts)
  {
	int i;
    	SET *sp;
    	sp = (SET*)malloc(sizeof(SET));
    	assert(sp != NULL);
	sp->count = 0;
	sp->length = maxElts;
	sp->data = (char **)malloc(sizeof(char*)*maxElts);
	assert(sp->data != NULL);
	sp->flags = malloc(sizeof(int*)*maxElts);
	assert(sp->flags != NULL);
	for(i = 0; i < sp->length; i++)
	{
		sp->flags[i] = EMPTY;
	}
	return sp;
}

//Finds the number of elements by returning the current count
//Efficiency: O(1)
int numElements(SET* sp){
    assert(sp!=NULL);
    return sp->count;
}

//Destroys the set by destroying the strings, then the string array, then the structure
//Efficiency: O(n)
void destroySet(SET *sp) {
    assert(sp != NULL);
    int i;
    for (i = 0;i<sp->count;i++) {
        if(sp->flags[i] == FILLED){
            free(sp->data[i]);
        }
    }
    free(sp->data);
    free(sp->flags);
    free(sp);
    return;
}

//Adds an element by searching to see if the element is repeated, then adding it to its proper location using the hash function.
//Efficiency: O(n) (This comes from the search)
void addElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
    bool found = false;
    int loc; 
    loc = search(sp, elt, &found);
    if(loc == -1) {
        return;
    }
    if(found == false) {
        sp->data[loc] = strdup(elt);
        assert(sp->data[loc] != NULL);
        sp->flags[loc] = FILLED;
        sp->count++;
    }
    if(found == true) {
        return;
    }
}
//Removes an element by searching to see where it is in the list via the hash function, then removing it.
//Efficiency: O(n) (This comes from the search)
void removeElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    int loc;
    bool found = false;
    loc = search(sp, elt, &found);
    if(found == true) {
        assert(sp->data[loc] != NULL);
        sp->flags[loc] = DELETED;
        free(sp->data[loc]);
        sp->count--;
    }
}
//Finds an element using the hash function, then returns the element.
//Efficiency: O(n)
char *findElement(SET *sp, char *elt){
    assert(sp != NULL && elt != NULL);
    bool found = false;
    int loc;
    loc = search(sp, elt, &found);
    if(found == false){
        return NULL;
    } else{
        return(sp->data[loc]);
    }
}
//Puts the current elements in an array, then returns the array
//Efficiency: O(n)
char **getElements(SET *sp){
    
	assert(sp != NULL);
    char **elms;
    elms = malloc(sizeof(char*)*(sp->count));
    int i;
    int j = 0;
    for(i = 0; i < sp->length; i++) {
        if(sp->flags[i] == FILLED) {
            elms[j] = sp->data[i];
            j++;    
        }
    }
    return elms;
}

//Searches the array using a hash function, then returns the found index.
//Efficiency: O(n)
static int search(SET *sp, char *elt, bool *found){
assert(sp != NULL);
    	assert(elt != NULL);
    	int i, loc;
    	int hold = -1;
    	*found = false;
    	unsigned key = strhash(elt);

    	for(i = 0; i < sp->length; i++)
    	{
            loc = (key + i) % (sp->length);

       		if(sp->flags[loc] == FILLED)
        	{
            		if(strcmp(elt, sp->data[loc]) == 0)
            		{
                 		*found = true;
                 		return (loc);
            		}
        	}
        	if(sp->flags[loc] == DELETED)
        	{
            		hold = loc;
        	}
        	if(sp->flags[loc] == EMPTY)
        	{
            		if(hold > -1)
            		{
                		return hold;
            		}
            		return loc;
        	}
    	}
    	return -1;
}

//The given hash function
unsigned strhash(char *s) {
    unsigned hash = 0;

    while(*s != '\0'){
        hash = 31 * hash + *s ++;
    }
    return hash;
}
