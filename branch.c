
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

static tree* emptyBranch(char* newName){
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

static char branchExists(tree* analyseBranch,char* newName){

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
static void goNextBranch(char* newName){
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

static void spacing(int level){
    int k=0;
    for(k=0;k<level;k++){
        printf("   ");
    }
}

static void printBranch(tree* branch,int level){

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

static void addAllNodesDirectlyLocated(tree* analysedBranch){
    if(analysedBranch !=NULL){
        shortcut* analysedNode = analysedBranch -> nodes;    
        
        while (analysedNode != NULL)
        {
            if(analysedNode -> node != NULL)
                addSelectedItem(analysedNode -> node );

            analysedNode = analysedNode -> next_short;
        }
    }
}

void selectAllNodes(tree* branch){
    initNewSelectionItems();

    tree* analysedBranch = branch;

    addAllNodesDirectlyLocated(analysedBranch);
    while (analysedBranch != NULL)
    {
        selectAllNodes(analysedBranch -> subbranch);
        analysedBranch = analysedBranch -> next_branch;
    }
       
        
}

void showSelectedItems(){
    lnode* listElement = selectedItems;

    printf("Selected items: \n");
    while (listElement != NULL)
    {
        printf("* NODE ID: %d \n",listElement -> myNode ->numberNode);
        listElement = listElement -> next_item;
    }
    
}