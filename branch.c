
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./shortcuts.c"
#include "./dictionary.c"

typedef struct tree{
    short id;
    struct tree* next_branch;
    struct tree* subbranch;
    shortcut* nodes;
} tree;

static tree* mainTree;
static tree* actBranch;

static tree* emptyBranch(short id){
    tree* newBranch = malloc(sizeof(tree));

    newBranch -> id = id;
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

static char branchExists(tree* analyseBranch,short id){

    char detected = FALSE;
    
    while(1){
        if(analyseBranch -> id == id){
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

    short nId = encode(newName);

    if(actBranch -> subbranch == NULL){
        tree* newBranch = emptyBranch(nId);
        actBranch -> subbranch = newBranch;
        actBranch = newBranch;
    }else{
        tree* analyseBranch = actBranch -> subbranch;

        if(!branchExists(analyseBranch,nId)){

            tree* newBranch = emptyBranch(nId);
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
        printf(" %d \n",analyseTree -> id);


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
            if(analysedNode -> node != NULL){
                addSelectedItem(analysedNode -> node );
               
            }
            analysedNode = analysedNode -> next_short;
        }
    }
}

static void selectAllNodesR(tree* branch){
  

    tree* analysedBranch = branch;

    
    while (analysedBranch != NULL)
    {
        addAllNodesDirectlyLocated(analysedBranch);
        if(analysedBranch -> subbranch != NULL) selectAllNodesR(analysedBranch -> subbranch);
        analysedBranch = analysedBranch -> next_branch;
    }
       
        
}

void selectAllNodes(tree* branch){
    initNewSelectionItems();
    selectAllNodesR(branch);
}
