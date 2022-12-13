#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"

//Term Project by Jeremy Akey, Due June 4th 2022
//For this project, I will be using a doubly linked circular linked list where each node contains a circular queue in order to create a queue ADT with a better runtime. 
//This works because you can just make more nodes when the list is full, rather than have to reallocate the queue array.


typedef struct node{
    void **data;
    int count;
    int length;
    int first;
    struct node *prev;
    struct node *next;      

} NODE;

typedef struct list{
    int length;
    int count;
    int nextLength;
    struct node *head;
} LIST;

//My idea for how to implement the loony lists, is through a doubly linked list circular list with a head pointer and a dummy node, so that you can access the front and back of the list in an efficient way


/*

• Should the linked list be singly-linked or doubly-linked?

Doubly Linked.

• Do we need only a head pointer or both a head and a tail pointer?

I am going to just use a head pointer, because I will make the list circular.

• Should the arrays within the nodes all be the same size, or should nodes increase in size as we add more and
more elements?

They nodes will be double the length of the previous node.

• Should we start searching for an item at a given index from the first node or the last node in the list? Does it
matter?

Search from the beginning to make the implementation simpler.

*/

//Efficiency: O(1) Creates the list ADT and a dummy node.
LIST *createList(void){
    LIST *lp = malloc(sizeof(LIST));
    assert(lp != NULL);
    lp->length = 0;
    lp->count = 0;
    lp->nextLength = 20;

    NODE *new = malloc(sizeof(NODE));
    assert(new != NULL);
    new->count = 0;
    new->length = 0;
    new->first = 0;
    new->data = NULL;
    new->prev = new;
    new->next = new;
    lp->head = new;
    return lp;
}
//Efficiency: O(n) Iterates through the list deleting the data.
void destroyList(LIST *lp){
    assert(lp != NULL);
    NODE *pDel;
    while(lp->length != 0){
        pDel = lp->head->next;
        lp->head->next = pDel->next;
        pDel->next->prev = lp->head;
        free(pDel->data);
        free(pDel);
        lp->length--;
     }
    free(lp->head);
    free(lp);
}
//Efficiency: O(1) returns the number of items
int numItems(LIST *lp){
    assert(lp != NULL);
    return lp->count;
}
//Efficiency: O(1) Adds an element to the beginning of the array of the first node, If the node is full, make another and make it the new first node.
void addFirst(LIST *lp, void *item){
    assert(lp != NULL);
    assert(item != NULL);
    if(lp->head->next->count == lp->head->next->length){
        NODE *pNew = malloc(sizeof(NODE));
        assert(pNew != NULL);
        pNew->count = 0;
        pNew->length = lp->nextLength;
        lp->length++;
        lp->nextLength *= 2;
        pNew->first = 0;
        pNew->data = malloc(sizeof(void*)*pNew->length);
        assert(pNew->data != NULL);
        pNew->prev = lp->head;
        pNew->next = lp->head->next;
        lp->head->next->prev = pNew;
        lp->head->next = pNew;
    }
    NODE *p = lp->head->next;
    if(p->count > 0){
       p->first += p->length -1;
       p->first %= p->length;
    } 
    p->data[p->first] = item;
    p->count++;
    lp->count++;

}
//Efficiency: O(1) Add an element to the end of the array in the last node, if the last node is full, add another node at the end.
void addLast(LIST *lp, void *item){
    assert(lp != NULL);
    assert(item != NULL);
    if(lp->head->prev->count == lp->head->prev->length){
        NODE *pNew = malloc(sizeof(NODE));
        assert(pNew != NULL);
        pNew->count = 0;
        pNew->length = lp->nextLength;
        lp->length++;
        lp->nextLength *= 2;
        pNew->first = 0;
        pNew->data = malloc(sizeof(void*)*pNew->length);
        assert(pNew->data != NULL);
        pNew->prev = lp->head->prev;
        pNew->next = lp->head;
        lp->head->prev->next = pNew;
        lp->head->prev = pNew;
    }
    int loc = (lp->head->prev->first + lp->head->prev->count++) % lp->head->prev->length;
    lp->head->prev->data[loc] = item;
    lp->count++;
}
//Efficiency: O(1) Remove the first element in the first node, if the first node is empty delete it.
void *removeFirst(LIST *lp){
    assert(lp != NULL);
    assert(lp->count > 0);
    NODE *p;
    if(lp->head->next->count == 0){
        p = lp->head->next;
        lp->head->next = p->next;
        p->next->prev = lp->head;
        free(p->data);
        free(p);
        lp->length--;
        lp->nextLength /= 2;
    }

    void *hold = lp->head->next->data[lp->head->next->first++];
    lp->head->next->first %= lp->head->next->length;
    lp->head->next->count--;
    lp->count--;
    return hold;
}
//Efficiency: O(1) Remove the last element in the last node, if the last node is empty delete it.
void *removeLast(LIST *lp){
    assert(lp != NULL);
    assert(lp->count > 0);
    NODE *p = lp->head->prev;
    if(lp->head->prev->count == 0){
        lp->head->prev = p->prev;
        p->prev->next = lp->head;
        free(p->data);
        free(p);
        lp->length--;
        lp->nextLength /= 2;
    }
    p = lp->head->prev;
    void *hold = p->data[(p->first + p->count -1) % p->length];
    p->count--;
    lp->count--;
    return hold;
}
//Efficiency: O(1) Return the first element in the first node.
void *getFirst(LIST *lp){
    assert(lp != NULL);
    return (lp->head->next->data[lp->head->next->first]);
}
//Efficiency: O(1) Return the last element in the last node.
void *getLast(LIST *lp){
    assert(lp!= NULL);
    return(lp->head->prev->data[(lp->head->prev->first + lp->head->prev->count) % lp->head->prev->length]);
}
//Efficiency: O(n) Traverse the list, subtracting the counts of each passed node until you have an array with the index available, then return that item.
void *getItem(LIST *lp, int index){
    assert(lp != NULL);
    NODE *p = lp->head->next;
    while(index >= p->count){
        index -= p->count;
        p = p->next;
    }
    void *hold = p->data[((p->first + index)% p->length)];
    return hold;
}
//Efficiency: O(n) Traverse the list, subtracting the counts of each passed node until you have an array with the index available then set the index to the variable.
void setItem(LIST *lp, int index, void *item){
    assert(lp != NULL);
     NODE *p = lp->head->next;
     while(index >= p->count){
        index -= p->count;
        p = p->next;
    }
     int loc = ((p->first + index) % p->length);
     p->data[loc] = item;

}
