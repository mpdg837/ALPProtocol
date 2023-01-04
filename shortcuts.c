#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./node.c"

typedef struct shortcut{
    node* node;
    struct shortcut* next_short;

} shortcut;

shortcut* emptyShortcut(int number){
    shortcut* newShort = malloc(sizeof(newNode));

    newShort -> node = getNode(number);
    newShort -> next_short = NULL;

    return newShort;

}


void spacingX(int level){
    int k = 0;
    for(k=0;k<level;k++){
        printf("   ");
    }
}

char nodeExists(shortcut* analyseNode, int number){
    char detected = FALSE;
    while (1){

        if(analyseNode -> node != NULL){
            if(analyseNode -> node -> numberNode == number){
                detected = TRUE;
                break;
            }
        }
        if(analyseNode -> next_short == NULL) break;

        analyseNode = analyseNode -> next_short;
    }

    return detected;
}

void printNodes(shortcut* analyseNode, int level) {

    shortcut* analysing = analyseNode;

    while (analysing != NULL) {
        spacingX(level);
        if(analysing -> node != NULL){
            printf("| Node ID: %d \n", analysing-> node -> numberNode);
        }

        analysing = analysing -> next_short;
    }

}
