#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"
//Lab 2 by Jeremy Akey, Due April 17th 2022
static int search(SET*, char*, bool*);
typedef struct set{
    char** data;
    int length;
    int count;
}SET;

//Function to create the set, with the size being determined by the maxElts in the other programs.
//Efficiency: O(1)
SET* createSet(int maxElts){
    SET* sp;
    sp = malloc(sizeof(SET));
    assert(sp!=NULL);
    sp->count = 0;
    sp->length = maxElts;
    sp->data = malloc(sizeof(char*)*maxElts);
    assert(sp->data!=NULL);
    
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
        free(sp->data[i]);
    }
    free(sp->data);
    free(sp);
}

//Adds an element by searching to see if the element is repeated, then adding it to the end of the list
//Efficiency: O(n) (This comes from the search)
void addElement(SET *sp, char *elt) {
    assert(sp!=NULL);
    bool found = false;
    int loc = search(sp, elt, &found);
    if(found == false){
        elt = strdup(elt);
        assert(elt != NULL);
        sp->data[sp->count] = elt;
        sp->count++;
    }
    return;
}
//Removes an element by searching to see where it is in the list, then shifting the last element up to replace it.
//Efficiency: O(n) (This comes from the search)
void removeElement(SET *sp, char *elt) {
    assert(sp != NULL);
    int loc;
    bool found = false;
    loc = search(sp, elt, &found);
    if(found == true) {
        free(sp->data[loc]);
        sp->data[loc] = sp->data[sp->count-1];
        sp->count--;
    }
}
//Finds an element using the search function, then returns the element.
//Efficiency: O(n)
char *findElement(SET *sp, char *elt){
    assert(sp != NULL);
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
    int i;
    char **elms;
    elms = malloc(sizeof(char *)*(sp->count));
    assert(elms != NULL);

    return memcpy(elms,sp->data,(sizeof(char *)*sp->count));
}
//Searches the array using a linear search
//Efficiency: O(n)
static int search(SET *sp, char *elt, bool *found){
    assert(sp != NULL);
    *found = false; 
    int i;
    for(i=0;i<sp->count;i++){
        if(strcmp(sp->data[i], elt) == 0){
            *found = true;
            return i;
        }
    }
    *found = false;
    return sp->count;
}
