#include <stdio.h>
#include <stdlib.h>
#include "MyMalloc.h"

#define SIZE 25000

//------------------------------------------------------------------------------------------------------------------------------

char myHeap[SIZE];

struct Segment{

    char isFree;
    int size;
    struct Segment *next;

};

static struct Segment * origin=NULL;
static struct Segment * ptr;
static struct Segment * preptr;
static struct Segment * temp;

//------------------------------------------------------------------------------------------------------------------------------

static void initializer(){

    origin=(struct Segment *)myHeap;                                                     //creating a structure in array and pass the first byte address of the array to pointer "origin"

    origin->isFree='Y';
    origin->size=sizeof(myHeap)-sizeof(struct Segment);
    origin->next=NULL;

    return;
}

//------------------------------------------------------------------------------------------------------------------------------

static void split_segment(struct Segment* ptr,int size){

    if(    ((temp=(struct Segment*)((char *)(ptr+1)+size)))  >=  (struct Segment *)(&myHeap[SIZE])  ) //to check whether the temp address is between the address range of the array
        return;

    temp->isFree='Y';                                                                   //inserting the data for the new Segment
    temp->size=ptr->size-sizeof(struct Segment)-size;

        temp->next=ptr->next;                                                           //linking the Meta Blocks
        ptr->next=temp;

        ptr->size=size;                                                                 //updating the size of the previous Segment

    return;
}

//------------------------------------------------------------------------------------------------------------------------------

void *MyMalloc(int size){

    if(origin==NULL)
        initializer();

    if(size<=0 || size > SIZE-sizeof(struct Segment))
        return NULL;

    ptr=origin;

    while(ptr){

        if(ptr->isFree=='Y' && ptr->size >= size){                                      //traversing threw Meta Blocks until finding a suitable Segment data

            if(ptr->size > size+sizeof(struct Segment))                                 //split the Segment if it is possible to , create a new Meta Block and atleast to hold one char
                split_segment(ptr,size);

            ptr->isFree='N';                                                            //change the Token as Allocated

            return (void *)(ptr+1);                                                     //returning the address which is, next to the end of Meta block
        }

    ptr=ptr->next;

    }

    return NULL;
}

//------------------------------------------------------------------------------------------------------------------------------

static void merge_segment(struct Segment* ptr){

    ptr->size=ptr->size+sizeof(struct Segment)+ptr->next->size;
    ptr->next=ptr->next->next;

    return;
}

//------------------------------------------------------------------------------------------------------------------------------

void MyFree(void* del){

    if(origin==NULL)
        return;

    ptr=origin;
    preptr=NULL;

    while(ptr){

        if((void *)(ptr+1)==del){                                                      //traverse through the loop until finding the specific address

            ptr->isFree='Y';

            if((ptr->next!=NULL) && (ptr->next->isFree=='Y'))                         //if next segment is free then merge it
                merge_segment(ptr);


            if((preptr!=NULL && preptr->isFree=='Y'))                                 //if previous segment is free then merge it *
                merge_segment(preptr);

            return;
        }

        preptr=ptr;
        ptr=ptr->next;
    }

    return;
}

//------------------------------------------------------------------------------------------------------------------------------

void print_Segment(){                                                                   //Additional Function to print the Segment

    printf("\n------------------------------------------------------------------");
    printf("\n| Free  |");
    printf("   strt addres    |");
    printf("\tEnding address  |\t  Size   |\n------------------------------------------------------------------");

    ptr=origin;

    while(ptr){

        printf("\n| %3c \t|",ptr->isFree);
        printf(" %p |",(void *)(ptr+1));

        if(ptr->next)
            printf("  %p  |",(void *)ptr->next-1);
        else
            printf("  %p  |",NULL);

        printf("\t%5d    |  MetaData Block RANGE { %p  -  %p }",ptr->size,ptr,(void *)(ptr+1)-1);
        printf("\n------------------------------------------------------------------");

        ptr=ptr->next;

        }

    printf("  MetaData Block size-------%ld Bytes",sizeof(struct Segment));

    return;
}



//Index No :-17001498
