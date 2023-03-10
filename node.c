#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./pathDecomposition.c"

#define TRUE 1
#define FALSE 0

typedef struct node
{
    int numberNode;

    char temperature;
    char pressure;
    char humitidy;
    char lightness;
    char turnOn;
    char turnOff;
    
    char connected;
}node;

typedef struct lnode
{
    node* myNode;
    char* path;
    struct lnode* next_item;
}lnode;

static struct lnode* listofNodes;

static node* newNode(int number){
    node* newNode = malloc(sizeof(node));
    
    newNode -> numberNode = number;
    
    newNode -> humitidy = FALSE;
    newNode -> lightness = FALSE;
    newNode -> temperature = TRUE;
    newNode -> pressure = TRUE;
    
    newNode -> turnOff = FALSE;
    newNode -> turnOn = FALSE;
    
    newNode -> connected = FALSE;

    return newNode;
}

static lnode* newNodeItem(node* myNode){
    lnode* itemNode = malloc(sizeof(lnode));

    itemNode -> myNode = myNode;
    itemNode -> next_item = NULL;

    char* npath = newShortPath();

    itemNode -> path = npath;

    return itemNode;
}

void createListNodes(){
    listofNodes = NULL;
}

static int addNewNode(int number){
    node* myNewNode = newNode(number);

    if(listofNodes == NULL){
        listofNodes = newNodeItem(myNewNode);
    }else{
        lnode* analysedNode = listofNodes;
        char detected = FALSE;

        while (1)
        {
            if(analysedNode -> myNode != NULL){
                if(analysedNode -> myNode ->numberNode == number){
                    detected = TRUE;
                    break;
                }
            }     
            if(analysedNode -> next_item == NULL)break;
       
            analysedNode = analysedNode -> next_item;
        }

        if(detected) {
            free(myNewNode);
            return 0;
        }
        else analysedNode -> next_item = newNodeItem(myNewNode);
        
    }
    return 1;
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

void showLNodeList(){
    lnode* analysedSensorNode = listofNodes;

    while (analysedSensorNode != NULL)
    {
        if(analysedSensorNode -> myNode != NULL){
            printf("SENSOR NODE: %d \n",analysedSensorNode ->myNode->numberNode);

            if(analysedSensorNode -> myNode ->temperature){
                printf(" + temperature \n");
            }
        
            if(analysedSensorNode -> myNode ->pressure){
                printf("  + pressure \n");
            }
        
            if(analysedSensorNode -> myNode ->humitidy){
                printf("  + humitidy \n");
            }
        
            if(analysedSensorNode -> myNode ->lightness){
                printf("  + lightness \n");
            }
        
        }
        analysedSensorNode = analysedSensorNode -> next_item;
    }
    
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