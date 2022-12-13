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
//This is a function to create a SET ADT for generic inputs, rather than just strings like we had been working with before
static int search(SET*, void*, bool*);

typedef struct set{
    void** data;
    int* flags;
    int length;
    int count;
    int (*compare)();
    unsigned (*hash)();
}SET;

//Function to create the set, and to set the Flags array to empty with the size being determined by the maxElts in the other programs.
//Efficiency: O(n)
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
  
	int i;
    	SET *sp;
    	sp = (SET*)malloc(sizeof(SET));
    	assert(sp != NULL);
	sp->count = 0;
	sp->length = maxElts;
	sp->data = (void **)malloc(sizeof(void*)*maxElts);
	assert(sp->data != NULL);
	sp->flags = malloc(sizeof(int*)*maxElts);
	assert(sp->flags != NULL);
	for(i = 0; i < sp->length; i++)
	{
		sp->flags[i] = EMPTY;
	}
    sp->compare = compare;
    sp->hash = hash;

	return sp;
}

//Finds the number of elements by returning the current count
//Efficiency: O(1)
int numElements(SET* sp){
    assert(sp!=NULL);
    return sp->count;
}

//Destroys the set by destroying the strings, then the string array, then the structure
//Efficiency: O(1)
void destroySet(SET *sp) {
    assert(sp != NULL);
    free(sp->data);
    free(sp->flags);
    free(sp);
    return;
}

//Adds an element by searching to see if the element is repeated, then adding it to the correct hash location.
//Efficiency: O(n) (This comes from the search)
void addElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);
    bool found = false;
    int loc; 
    loc = search(sp, elt, &found);
    if(found == true || loc == -1) {
        return;
    }
    if(sp->count == sp->length){
        return;
    }
    sp->data[loc] = elt;
    assert(sp->data[loc] != NULL);
    sp->flags[loc] = FILLED;
    sp->count++;
    return;
}
//Removes an element by searching to see where it is in the list
//Efficiency: O(n) (This comes from the search)
void removeElement(SET *sp, void *elt) {
     assert(sp != NULL && elt != NULL);
    bool found = false;
    int loc;
    loc = search(sp, elt, &found);
    if(found == true) {
        assert(sp->data[loc] != NULL);
        sp->flags[loc] = DELETED;
        sp->count--;
        return;
    }
}
//Finds an element using the search function, then returns the element.
//Efficiency: O(n)
void *findElement(SET *sp, void *elt){
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
//Allocates an array the size of count, then adds all of the flagged elements, then returns the filled array.
//Efficiency: O(n)
void *getElements(SET *sp){
    
	assert(sp != NULL);
    void **elms;
    elms = malloc(sizeof(void*)*(sp->count));
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

//Searches the array using the hash function and returns the index
//Efficiency: O(n)
static int search(SET *sp, void *elt, bool *found){
assert(sp != NULL);
    	assert(elt != NULL);
    	int i, loc;
    	int hold = -1;
    	*found = false;
    	unsigned key = sp->hash(elt);

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
