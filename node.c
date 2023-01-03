#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct node
{
    int numberNode;

    char temperature;
    char pressure;
    char humitidy;
    char lightness;
}node;

typedef struct lnode
{
    node* myNode;
    struct lnode* next_item;
}lnode;

static struct lnode* listofNodes;

node* newNode(int number){
    node* newNode = malloc(sizeof(node));
    
    newNode -> numberNode = number;
    
    newNode -> humitidy = FALSE;
    newNode -> lightness = FALSE;
    newNode -> numberNode = FALSE;
    newNode -> temperature = FALSE;

    return newNode;
}
lnode* newNodeItem(node* myNode){
    lnode* itemNode = malloc(sizeof(lnode));
    itemNode -> myNode = myNode;
    itemNode -> next_item = NULL;
}

void createListNodes(){
    listofNodes = NULL;
}

void addNewNode(int number){
    node* myNewNode = newNode(number);
    if(listofNodes == NULL){
        listofNodes = newNodeItem(myNewNode);
    }else{
        lnode* analysedNode = listofNodes;

        while (analysedNode != NULL)
        {
            analysedNode = analysedNode -> next_item;
        }

        lnode* newItem = malloc(sizeof(lnode));
        newItem ->myNode = myNewNode;
        newItem ->next_item = NULL;

        analysedNode = newItem;
        
    }
}

node* getNode(int number){
    lnode* analysedItem = listofNodes;

    while (analysedItem != NULL)
    {
        if(analysedItem->myNode != NULL){
            if(analysedItem -> myNode -> numberNode == number){
                return analysedItem -> myNode;
            }
        }
        analysedItem = analysedItem -> next_item;
    }

    return NULL;        
}


void killNodesList(){
    lnode* analysedItem = listofNodes;

    while (analysedItem != NULL)
    {
        free(analysedItem -> myNode);
        lnode* savedItem = analysedItem;
        analysedItem = analysedItem -> next_item;

        free(savedItem);
    }

}