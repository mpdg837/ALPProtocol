
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./shortcuts.c"

typedef struct tree{
    char* name_of_branch;
    struct tree* next_branch;
    struct tree* subbranch;
    shortcut* nodes;
} tree;

static tree* mainTree;
static tree* actBranch;

tree* emptyBranch(char* newName){
    tree* newBranch = malloc(sizeof(tree));

    newBranch -> name_of_branch = newName;
    newBranch -> next_branch = NULL;
    newBranch -> nodes = NULL;
    newBranch ->  subbranch = NULL;

    return newBranch;
}


void addNode(int number) {
    shortcut *analyseNode = actBranch->nodes;

    if(analyseNode == NULL){
        shortcut *newNode = emptyShortcut(number);
        actBranch->nodes = newNode;
    }else{

        if(!nodeExists(analyseNode,number)){
            shortcut *newNode = emptyShortcut(number);
            analyseNode->next_short = newNode;
        }
    }


}

char branchExists(tree* analyseBranch,char* newName){

    char detected = FALSE;

    while(1){
        if(strcmp(analyseBranch -> name_of_branch,newName) == 0){
            actBranch = analyseBranch;
            detected = TRUE;
            break;
        }
        if(analyseBranch -> next_branch == NULL){
            break;
        }
        analyseBranch = analyseBranch -> next_branch;
    }

    return detected;
}
void goNextBranch(char* newName){
    // Add new branch

    if(actBranch -> subbranch == NULL){
        tree* newBranch = emptyBranch(newName);
        actBranch -> subbranch = newBranch;
        actBranch = newBranch;
    }else{
        tree* analyseBranch = actBranch -> subbranch;

        if(!branchExists(analyseBranch,newName)){

            tree* newBranch = emptyBranch(newName);
            analyseBranch -> next_branch = newBranch;

            actBranch = newBranch;
        }
    }

}

void spacing(int level){
    int k=0;
    for(k=0;k<level;k++){
        printf("   ");
    }
}

void printBranch(tree* branch,int level){

    tree* analyseTree = branch;

    while(analyseTree != NULL){
        spacing(level);
        printf("+");
        printf(" %s \n",analyseTree -> name_of_branch);


        printNodes(analyseTree -> nodes,level+1);

        tree* subbranch = analyseTree -> subbranch;
        if(subbranch != NULL){
            printBranch(subbranch,level +1);
        }

        if(analyseTree -> next_branch == NULL){
            break;
        }
        analyseTree = analyseTree -> next_branch;
    }

}
