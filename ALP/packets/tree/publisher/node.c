#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils/pathDecomposition.c"
#include "./utils/ipConverter.c"

#define TRUE 1
#define FALSE 0

typedef struct node
{
    int numberNode;
    int ip;
    
    char driver;
    
    char temperature;
    char pressure;
    char humitidy;
    char lightness;
    char command;
    char info;
    
    char connected;
}node;

typedef struct lnode
{
    node* myNode;
    char* path;
    int ipSubscriber;
    
    struct lnode* next_item;
}lnode;

static struct lnode* listofNodes;

static node* newNode(int number,char driver){
    node* newNode = malloc(sizeof(node));
    
    newNode -> numberNode = number;
    
    newNode -> humitidy = FALSE;
    newNode -> lightness = FALSE;
    newNode -> temperature = FALSE;
    newNode -> pressure = FALSE;
    
    newNode -> driver = driver;
    
    newNode -> command = FALSE;
    newNode -> info = FALSE;
    newNode -> ip = 0;
    
    newNode -> connected = FALSE;

    if(driver){
        newNode -> command = TRUE;  
    }
    
    return newNode;
}

static lnode* newNodeItem(node* myNode){
    lnode* itemNode = malloc(sizeof(lnode));

    itemNode -> myNode = myNode;
    itemNode -> next_item = NULL;

    char* npath = newShortPath();
    itemNode -> ipSubscriber = 0;

    itemNode -> path = npath;
    
    return itemNode;
}

void createListNodes(){
    listofNodes = NULL;
}

static int addNewNode(int number,char driver){
    node* myNewNode = newNode(number,driver);
    
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

node* getNodeByIp(int ip){
    lnode* analysedItem = listofNodes;

    while (analysedItem != NULL)
    {
        if(analysedItem->myNode != NULL){
            if(analysedItem -> myNode -> ip == ip){
                return analysedItem -> myNode;
            }
        }
        analysedItem = analysedItem -> next_item;
    }

    return NULL;        
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
            printf("NODE: %d ",analysedSensorNode ->myNode->numberNode);
            
            int ip = analysedSensorNode -> myNode -> ip;
            
            if(ip == 0 && !analysedSensorNode -> myNode -> driver) printf(" is not registered by broker");
            else if(analysedSensorNode -> myNode -> driver) printf(" is registered by broker but not exists (shortcut for publishing by subscribers)");
            else printf("is sensor node registered in broker and known as %s ", decodeIPtoCharacters(ip));
            
            printf("\n");
            
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
            
             if(analysedSensorNode -> myNode ->command){
                printf("  + command \n");
            }
            
            
             if(analysedSensorNode -> myNode ->info){
                printf("  + info \n");
            }
            
            if(analysedSensorNode -> myNode ->driver) printf("is driver \n");
            else if(analysedSensorNode -> myNode ->connected) printf("is connected \n");
            else printf("is not connected \n");
        
        }
        analysedSensorNode = analysedSensorNode -> next_item;
    }
    
}
void killNodesList(){
    lnode* analysedItem = listofNodes;

    while (analysedItem != NULL)
    {
        free(analysedItem -> path);
        
        free(analysedItem -> myNode);
        lnode* savedItem = analysedItem;
        analysedItem = analysedItem -> next_item;

        free(savedItem);
    }

}

void updateServices(node* myNode,char* context){
            
            if((context[2] >> 7) & 0x1) {
                printf(" + temperature \n");
                myNode -> temperature = TRUE;
            }else{
                myNode -> temperature = FALSE;
            }
            if((context[2] >> 6) & 0x1){
                printf(" + pressure \n");
                 myNode -> pressure = TRUE;
            }else{
                 myNode -> pressure = FALSE;
            }
            
            if((context[2] >> 5) & 0x1){
                printf(" + humitidy \n");
                 myNode -> humitidy = TRUE;
            }else{
                 myNode -> humitidy = FALSE;
            }
            
            if((context[2] >> 4) & 0x1){
                printf(" + lightness \n");
                myNode -> lightness = TRUE;
            }else{
                myNode -> lightness = FALSE;
            }
            
            if((context[2] >> 3) & 0x1){
                printf(" + command \n");
                myNode -> command = TRUE;
            }else{
                myNode -> command = FALSE;
            }
            
            if((context[2] >> 2) & 0x1){
                printf(" + info \n");
                myNode -> info = TRUE;
            }else{
                myNode -> info = FALSE;
            }
}