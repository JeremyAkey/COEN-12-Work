#include <stdio.h>
#include <stdlib.h>
#define MAX_WORD_LENGTH 30

//Made by Jeremy Akey for Lab 1 of COEN 12 on 3/31/2022, This is code to count the number of words in a supplied txt file.
//The only function used is the main function, which takes a command line input, checks it's validity and the ability to open the file provided, then traverses through the file's words with a max length of 30, counting each word it traverses. The counted number of words is outputted at the end of the main function. 

int main(int argc, char *argv[]){
    char *fname, holder[MAX_WORD_LENGTH];
    FILE *infp;
    int count;
    if(argc == 1){
        printf("One argument expected\n");
        return 1;
    } else if(argc == 2){
        fname = argv[1];
	if((infp = fopen(fname, "r")) == NULL){
	    printf("Error opening file!\n");
	    return 1;
		}
    } else {
        printf("Too many arguments\n");
        return 1;
    }
    while(fscanf(infp, "%s", holder) == 1){
        count++;
    }
    printf("There are %d words in this file!\n",count);
}
