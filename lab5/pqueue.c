#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pqueue.h"



//Lab 5 by Jeremy Akey, Due May 29th 2022.
//This code creates a priority queue ADT using an array and a min heap.

//Creating the local functions to make finding the parent and children of nodes easier.
static int parent(int);

static int leftch(int);

static int rightch(int);

//Making the queue structure using an array of void pointers, in order to keep it general.
struct pqueue{
    int count;
    int length;
    void **data;
    int (*compare)();
};

//Efficiency: O(1), Create the queue.
PQ *createQueue(int (*compare)()){
    PQ *pq = malloc(sizeof(PQ));
    assert(pq != NULL);
    pq->count = 0;
    pq->length = 10;
    pq->compare = compare;
    pq->data = malloc(sizeof(void*)*10);
    assert(pq->data != NULL);
    return pq;
}
//Efficiency: O(1), returns the number of elements in the queue.
int numEntries(PQ *pq){
    assert(pq != NULL);
    return pq->count;
}

//Efficiency: O(n), frees all of the data in the priority queue in order.
void destroyQueue(PQ *pq){
    assert(pq != NULL);
    int i;
    for(i=0; i<pq->count; i++){
        free(pq->data[i]);
    }
    free(pq->data);
    free(pq);
}

//Efficiency: O(log(n)) //Inserts an element into the priority queue.
void addEntry(PQ *pq, void *elm){
    assert(pq != NULL);
    int loc;
    if(pq->count == pq->length){
        pq->length *= 2;
        pq->data = realloc(pq->data, sizeof(void*)*pq->length);
    }
    pq->data[pq->count] = elm;
    loc = pq->count;
    while(pq->count != 0 && (*pq->compare)(pq->data[parent(loc)], pq->data[loc]) > 0) {
        void *hold = pq->data[loc];
        pq->data[loc] = pq->data[parent(loc)];
        pq->data[parent(loc)] = hold;
        loc = parent(loc);
    }
    pq->count++;
}

//Efficiency: O(log(n)), removes an element from the priority queue.
void *removeEntry(PQ *pq){
    assert(pq != NULL);
    int loc, min;
    void *root = pq->data[0];
    pq->data[0] = pq->data[pq->count-1];
    loc = 0;
    min = 0;
    while(leftch(loc) <= pq->count -1){
        min = leftch(loc);
        if(rightch(loc) <= pq->count -1){
            if((*pq->compare)(pq->data[rightch(loc)], pq->data[leftch(loc)]) < 0){
                min = rightch(loc);
            }
        }
        if((*pq->compare)(pq->data[loc], pq->data[min]) < 0){
            break;
        } else {
            void* hold = pq->data[loc];
            pq->data[loc] = pq->data[min];
            pq->data[min] = hold;
        }
        loc = min;
    }
    pq->count--;
    return root;
}

//Efficiency O(1): Functions that run the logic to find the parent and children in an array.
static int parent(int loc){
    return (loc-1)/2;
}

static int leftch(int loc){
    return (loc*2)+1;
}

static int rightch(int loc){
    return (loc*2)+2;
}
