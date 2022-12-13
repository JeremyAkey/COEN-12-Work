#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h"


//Lab 5 by Jeremy Akey, Due May 29th 2022
//huffman.c of lab5, this code uses that huffman coding technique to compress the data in a given file. It does this through the implementation of the priority queue ADT made earlier.

typedef struct node NODE;

//Efficiency O(h): coutns the distance from the leaf to the root of the heap.
int huffNum(NODE *p){
    int num = 0;

    while(p->parent != NULL){
        p = p->parent;
        num++;
    }
    return num;
}

//Efficiency O(1): Function to compare the data in two nodes
int compare(NODE *p, NODE *q){
    if(p->count < q->count){
        return -1;
    } else {
        return p->count > q->count;
    }
}

//Main function to take a command line input (file location), then applying the Huffman coding method to compress it and store the data.
int main(int argc, char *argv[]){

    //Opens the given file and reads the characters in it, assigning them to an array.
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL){
        return 0;
    }
    int nums[257] = {0};
   
    do {
        int hold;
        hold = fgetc(fp);
        if(feof(fp)){
            break;
        }
        nums[hold]++;
    } while(1);

    //Applies the ADT to create the priority queue
    PQ *pq = createQueue(compare);
    NODE *nodes[257] = {0};
    int i;
    for(i = 0; i < 257; i++){
        nodes[i] = NULL;
    }
    for(i = 0; i < 257; i++){
        if(nums[i] > 0){
            NODE *p = malloc(sizeof(NODE));
            p->count = nums[i];
            p->parent = NULL;
            addEntry(pq, p);
            nodes[i] = p;
        }
    }
    //Adds the end node to the end
    NODE *end = malloc(sizeof(NODE));
    end->count = 0;
    end->parent = NULL;
    addEntry(pq, end);
    nodes[256] = end;

    //Combines the trees until youre left with a huffman tree
    while(numEntries(pq) > 1){
        NODE *first = removeEntry(pq);
        NODE *second = removeEntry(pq);
        NODE *third = malloc(sizeof(NODE));
        third->count = (first->count + second->count);
        third->parent = NULL;
        first->parent = third;
        second->parent = third;
        addEntry(pq, third);
    }

    //Prints out the valid nodes
    for(i = 0; i < 257; i++){
        if(nodes[i] != NULL){
            int num = huffNum(nodes[i]);
            if(isprint(i)){
                printf("%c: %d x %d bits = %d bits\n", i, nums[i], num, num*nums[i]);
            } else {
                printf("%03o: %d x %d bits = %d bits\n", i, nums[i], num, num*nums[i]);
            }
        }
    }
    //Packs the tree using the pack.c file
    pack(argv[1], argv[2], nodes);
    return 0;
}

