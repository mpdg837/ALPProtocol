#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1;
#define FALSE 0;

typedef struct sensorNode{
    int number_node;
    struct sensorNode* next_node;

} sensorNode;

sensorNode* emptyNode(int number){
    sensorNode* newNode = malloc(sizeof(newNode));

    newNode -> number_node = number;
    newNode -> next_node = NULL;

    return newNode;

}


void spacingX(int level){
    int k = 0;
    for(k=0;k<level;k++){
        printf("   ");
    }
}

char nodeExists(sensorNode* analyseNode, int number){
    char detected = FALSE;
    while (1){

        if(analyseNode -> number_node == number){
            detected = TRUE;
            break;
        }
        if(analyseNode -> next_node == NULL) break;

        analyseNode  = analyseNode -> next_node;
    }

    return detected;
}

void printNodes(sensorNode* analyseNode, int level) {

    sensorNode *analysing = analyseNode;

    while (analysing != NULL) {
        spacingX(level);
        printf("| Node ID: %d \n", analysing->number_node);
        analysing = analysing -> next_node;
    }

}
