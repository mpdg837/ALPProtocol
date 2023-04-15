
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./shortcuts.c"

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
            
            while(1){
                if(analyseBranch -> next_branch == NULL) break;
                analyseBranch = analyseBranch -> next_branch;
            }
            
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
        printf(" (%d) ",analyseTree -> id);
        
        char* results = getValue(analyseTree -> id);    
        printf(" - '%s'",results);

        printf("\n");

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

static void addAllNodesDirectlyLocated(tree* analysedBranch,char* path, int level){
    
    char* relPath = malloc(sizeof(char) * DIRECOTRY_BUFFER_SIZE);
    copyPath(relPath,path);

    if(analysedBranch !=NULL){
        shortcut* analysedNode = analysedBranch -> nodes;    
        
        while (analysedNode != NULL)
        {
            if(analysedNode -> node != NULL){

                copyPath(relPath,path);
                
                char* newPath = malloc(sizeof(char) * DIRECOTRY_BUFFER_SIZE);
                copyPath(newPath,relPath);
                
                if(!analysedNode -> node -> driver || level == 1){
                    addSelectedItem(analysedNode -> node,newPath,0);
                }
                

            }
            analysedNode = analysedNode -> next_short;
        }
    }

    free(relPath);
}

static void selectAllNodesR(tree* branch,char* path,char addBranchToP, int level){
  
    level ++;
    char* relPath = malloc(sizeof(char) * DIRECOTRY_BUFFER_SIZE);
    memset(relPath,0,sizeof(char) * DIRECOTRY_BUFFER_SIZE);
    
    copyPath(relPath,path);    

    tree* analysedBranch = branch;

    
    while (analysedBranch != NULL)
    {
        copyPath(relPath,path);
        if(addBranchToP) addBranchToPath(relPath,analysedBranch -> id);

        addAllNodesDirectlyLocated(analysedBranch,relPath,level);
        if(analysedBranch -> subbranch != NULL) selectAllNodesR(analysedBranch -> subbranch,relPath,TRUE,level);
        analysedBranch = analysedBranch -> next_branch;
    }

    free(relPath);
       
        
}

void selectAllNodes(tree* branch,char* absPath){
    initNewSelectionItems();
    char* relPath = malloc(sizeof(char) * DIRECOTRY_BUFFER_SIZE);
    memset(relPath,0,sizeof(char) * DIRECOTRY_BUFFER_SIZE);

    copyPath(relPath,absPath);

    int level = 0;
    
    selectAllNodesR(branch,relPath,FALSE,level);
    free(relPath);
}
