#include <stdio.h>
#include <stdlib.h>
#include "MyMalloc.h"

#define SIZE 25000

#define location(ptr) (void *)((char *)ptr+sizeof(char *))

//--------------------------------------------------------------------------------------------------------------

char myHeap[SIZE], freed ='f', allocated ='a';

static char ** root;
static char **traverser;
static char **temp;
static char **pretraverser;

//--------------------------------------------------------------------------------------------------------------

static void initializer(){

    myHeap[0]=freed;                                                                    //initializing the array as free
    root=(char **)(myHeap+1);                                                           //creating a char * in array and pass the second byte's address to root(which is a pointer to a pointer)
    *root=NULL;                                                                         //dereferencing the pointer gives the next+1 address of metablock

    return;
}

//--------------------------------------------------------------------------------------------------------------


static int findsize(char **traverser){

    if(*traverser)                                                                      //if the traverser does not contain a Null values then
        return (char *)(*traverser)-(char *)traverser-sizeof(char *)-1;                 //return the size (next metablock start address - current metablock start address)
        return &myHeap[SIZE]-(char *)traverser-sizeof(char *);                          //else return size (last byte address of the array - current metablock start address)
}

//--------------------------------------------------------------------------------------------------------------

static void split(char **traverser,int size)
{
    temp=(char **)((char *)traverser+size+sizeof(char)+sizeof(char *));

    if(temp>=(char **)(&myHeap[25000]))                                                 //checking the address of temp is greater than or equal to address of last byte of the arry
        return;                                                                         //if true can't split return from the function

    *temp=*traverser;                                                                   //linking the blocks
    *traverser=(char *)temp;

    *((char *)temp-1)=freed;                                                            //inseting free token data to the new metablock


    return;
}

//--------------------------------------------------------------------------------------------------------------

void * MyMalloc(int size){
    if(root==NULL)
        initializer();

    if(size<=0)
        return NULL;

    traverser=root;
    pretraverser=NULL;


    while(traverser){

        if(*((char*)traverser-1)==freed && findsize(traverser)>=size){

            if(findsize(traverser) > size+(sizeof(char)+sizeof(char *)) )
                split(traverser,size);

            *((char *)traverser-1)=allocated ;                                         //updating the data of the metablock as allocated

            return location(traverser);                                                //returning the address
        }

        traverser=(char **)(*traverser);                                               //assinig the address of next metablock to traverser pointer

    }

    return NULL;
}

//--------------------------------------------------------------------------------------------------------------

static void merge(char **traverser){

    temp=(char **)(*traverser);                                                         //get the address of next metablock and storing it into temp
    *traverser=*temp;                                                                   //set the traverser's next metablock as temp's next metablokc

    return;
}

//--------------------------------------------------------------------------------------------------------------

void MyFree(void *dele){

    if(!root)
        return;

    traverser=root;
    pretraverser=NULL;

    while(traverser){

        if((void *)location(traverser)==dele){                                          //looping unitil accessing the correct location

            *((char *)traverser-1)=freed;                                               //change the metablock token as free

            if(*traverser && *((char *)*traverser-1)==freed)                            //if the nextblock is free then merge
                merge(traverser);

            if(pretraverser && *((char *)pretraverser-1)==freed)                        //if the previousblock is free then merge
                 merge(pretraverser);

           return;
        }

       pretraverser=traverser;
       traverser=(char **)(*traverser);

    }

    return;
}


//--------------------------------------------------------------------------------------------------------------

void printSeg(){

    printf("\n------------------------------------------------------------------");
    printf("\n| Free  |");
    printf("   strt addres    |");
    printf("\tEnding address  |\t  Size   |\n------------------------------------------------------------------");

    traverser=root;


      while(traverser){


        printf("\n| %3c \t|",*((char *)traverser-1));
        printf(" %p |",traverser+1);

        if(*traverser)
            printf("  %p  |",(char *)*traverser-1);
        else
            printf("  %p  |",NULL);

        printf("\t%5d    |  MetaData Block RANGE { %p  -  %p }",findsize(traverser),(char *)traverser-1,(char *)(traverser+1)-1);
        printf("\n------------------------------------------------------------------");


        traverser=(char **)(*traverser);
    }
    printf("  MetaData Block size-------%ld Bytes",sizeof(char *)+sizeof(char));

    return;

}

//17001498

