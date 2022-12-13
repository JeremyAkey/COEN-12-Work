#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include <stdbool.h>
#include "list.h"

//Set.c for Lab4 by Jeremy Akey, Due May 15th 2022
//This is code to define a SET ADT, using lists to store the data, which we defined in list.c. This SET uses a combination of hashing and linked lists in order to resolve collisions, and remove the need for probing.
//Because linked lists have infinite capacity, collisions are no longer an issue, because we can store multiple items in one array index in the SET.

static int search(SET*, void*, bool*);

// SET structure
struct set{
    int count;
    int length;
    LIST **data;
    int (*compare)();
    unsigned (*hash)();
};

// Function to create the set, using the functions we made for the LIST ADT, O(n) efficiency because it must create n lists for the set.
SET *createSet(int maxElts,int (*compare)(),unsigned (*hash)()){
    SET *sp = malloc(sizeof(SET));
    int i;
    assert(sp!=NULL);
    sp->count = 0;
    sp->length = maxElts/20;
    sp->data = malloc(sizeof(LIST*) * sp->length);
    sp->compare = compare;
    sp->hash = hash;
    for(i = 0; i<sp->length; i++){
        sp->data[i]=createList(sp->compare);
    }
    assert(sp->data != NULL);
    return sp;
}

// Function to destroy the set, O(n) efficiency because it must delete n lists in the SET.
void destroySet(SET *sp) {
    assert(sp != NULL);
    int i;
    for(i = 0; i<sp->length;i++) {
        destroyList(sp->data[i]);
    }
    free(sp->data);
    free(sp);
}

// Function to return the number of elements, O(1) runtime.
int numElements(SET *sp) {
    assert(sp!=NULL);
    return sp->count;
}

//Function to add an element to the SET, O(n) efficiency because it uses the search function
void addElement(SET *sp, void *elt) {
    assert(sp != NULL && elt != NULL);
    bool found = false;
    int loc = search(sp, elt, &found);           
    if(found == false) {                           
        addLast(sp->data[loc], elt);              
        sp->count++;                                
    }
    return;
}

//Function to remove an element from the SET, O(n^2) efficiency because it uses the search function, then uses the removeItem function, which both have an O(n) efficiency
void removeElement(SET *sp, void *elt) {
    assert(sp != NULL && elt != NULL);
    bool found = false;
    int loc = search(sp, elt, &found);           
    if(found == false) {                               
        return;                                    
    }
    else {
        removeItem(sp->data[loc], elt);            
        sp->count--;                               
        return;
}
}

//Function to find an element in the SET, O(n^2) efficiency because it has to search the set for the correct list, then use the findItem function
void *findElement(SET *sp, void *elt) {
 assert(sp != NULL && elt != NULL);
    bool found = false;
    int loc = search(sp, elt, &found);            
    if(found == false) {                          
        return NULL;                               
    }
    else {                                        
        return findItem(sp->data[loc], elt);      
    }
}

//Function to return all of the elements in the SET, O(n^3) effiency because it uses the getItems function in a nested loop
void *getElements(SET *sp) {
    
    assert(sp != NULL || sp->count != 0);
    void **elts = malloc(sizeof(void *) * sp->count);
    assert(elts != NULL);
    int i, j, counter = 0;
    for(i = 0; i<sp->length; i++){
        if(sp->data[i] != NULL){
            void **hold = getItems(sp->data[i]);
            for(j = 0; j<numItems(sp->data[i]); j++){
                elts[counter] = hold[j];
                counter++;
            }
        }
    }
    return elts;
}

//Function to search for a specific item in the SET, O(n^2) runtime because it uses the hash function and the findItem function
static int search(SET* sp, void *elt, bool *found){
    assert(sp != NULL && elt != NULL);
    int loc = (*sp->hash)(elt)%sp->length;                    
    if(findItem(sp->data[loc], elt) != NULL) {               
        *found = true;                                          
        return loc;
    }
    else {
        *found = false;                                         
        return loc;
    }
}

