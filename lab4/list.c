#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"


//list.c for Lab 4 by Jeremy Akey, Due May 15th 2022.
//This is the code to define a Deque ADT using a doubly linked, circular linked list, so that we can input and remove from both ends of the list, as well as change its length dynamically as needed. We use this ADT to solve a maze, and put a list of numbers in order.
// The linked list also makes use of a dummy node, to reduce the number of input and removal cases, and make the code more streamlined.



// Node structure
typedef struct node{
    void* data;
    struct node *next;
    struct node *prev;
} NODE;

//List structure
typedef struct list{
    int count;
    struct node *head;
    int (*compare)();
} LIST;

// Function to create a list, O(1) efficiency

LIST *createList(int (*compare)())
{
    struct list *lp;    
    lp = malloc(sizeof(struct list));    
    assert(lp != NULL);    
    lp->count = 0;    
    lp->compare = compare;    
    lp->head = malloc(sizeof(struct node));    
    assert(lp->head != NULL);    
    lp->head->next = lp->head;    
    lp->head->prev = lp->head;    
    return lp;
}

// Destroy the list, O(n) efficiency because it has to traverse across n elements

void destroyList(LIST *lp)
{
    assert(lp != NULL);
    NODE *pDel = lp->head->prev;
    NODE *pPrev;
    while(pDel != lp->head){
          pPrev = pDel->prev;
          free(pDel);
          pDel = pPrev;
     }
    free(pDel);
    free(lp);
}

// Function to return the length of the list, O(1) efficiency

int numItems(LIST *lp)
{
    assert(lp != NULL);
    return lp->count;
}

// Function to add an element to the beginnning of the list, O(1) efficiency

void addFirst(LIST *lp, void *item)
{
    assert(lp != NULL);
    NODE *p;
    p = malloc(sizeof(struct node));
    assert(p != NULL);
    p->data = item;
    p->next = lp->head->next;
    lp->head->next->prev = p;
    lp->head->next = p;
    p->prev = lp->head;
    lp->count++;
    return;
}

// Function to add an element to the end of the list, O(1) efficiency

void addLast(LIST *lp, void *item)
{
    assert(lp != NULL);
    NODE *p = malloc(sizeof(struct node));
    assert(p != NULL);
    p->data = item;
    lp->head->prev->next = p;
    p->next = lp->head;
    p->prev = lp->head->prev;
    lp->head->prev = p;
    lp->count++;
    return;
}

// Function to remove an element from the beginnning of the list, O(1) efficiency

void *removeFirst(LIST *lp)
{
    assert(lp != NULL && lp->count != 0);
    NODE *p = lp->head->next;
    void *hold = p->data;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    lp->count--;
    free(p);
    return hold;
}

// Function to remove an element from the end of the list, O(1) efficiency

void *removeLast(LIST *lp)
{
    assert(lp != NULL && lp->count != 0);
    NODE *p = lp->head->prev;
    void *hold = p->data;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    lp->count--;
    free(p);
    return hold;
}

// Function to return the first item in the list, O(1) efficiency

void *getFirst(LIST *lp)
{
    assert(lp != NULL && lp->count != 0);
    return(lp->head->next->data);
}

// Function to return the last item in the list, O(1) efficiency

void *getLast(LIST *lp)
{
    assert(lp != NULL && lp->count != 0);
    return(lp->head->prev->data);
}

// Function to remove any item in the list, O(n) efficiency because it must search the list for the item

void removeItem(LIST *lp, void *item) {
    assert(lp != NULL);
    if(lp->count > 0) {
        NODE *p = lp->head->next;
        while(p != lp->head) {
            if((*lp->compare)(p->data, item) == 0) {
                p->prev->next = p->next;
                p->next->prev = p->prev;
                free(p);
                lp->count--;
                break;
            }
            else {
                p = p->next;
            }
        }
    }
    return;
}

// Function to find an item in the list, O(n) efficiency because it must search the list for the item

void *findItem(LIST *lp, void *item) {
    assert(lp != NULL);
    if(lp->count > 0) {
        NODE *p = lp->head->next;
        while(p != lp->head) {
            if((*lp->compare)(p->data, item) == 0) {
                return p->data;
            }
            else {
                p = p->next;
            }
        }
    }
    return NULL;
}

// Function to create an array of all the data in the list, then return the array, O(n) efficiency because it must traverse the list for the items 

void *getItems(LIST *lp)
{
    assert(lp != NULL);
    int i = 0;
    void **elts;
    NODE *p = lp->head->next;
    elts = malloc(sizeof(void *)*lp->count);
    assert(elts != NULL);
    while(i < lp->count && p != lp->head) {
        elts[i] = p->data;
        p = p->next;
        i++;
    }
    return elts;
}
