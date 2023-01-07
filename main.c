#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tree.c"


int main() {
    createListNodes();

    mainTree = emptyBranch("broker");

    addNodePath("/budynek=1");
    addNodePath("/budynek=3");
    addNodePath("/budynek/mieszkanie1/pokoj1=8");
    addNodePath("/budynek/mieszkanie1/pokoj1/szafa=15");
    addNodePath("/budynek/mieszkanie1/pokoj2=12");
    addNodePath("/budynek/mieszkanie2=1");
    addNodePath("/budynek/mieszkanie2/pokoj1=3");
    addNodePath("/budynek/mieszkanie2/pokoj1=8");
    addNodePath("/budynek/mieszkanie2/pokoj2=12");
    addNodePath("/budynek/mieszkanie2/pokoj2/szafa=15");

    printf("\n \n Main Raport: \n \n");
    showLNodeList();
    printBranch(mainTree,0);
    
    printf("==== \n");
    char* path = "/budynek/mieszkanie2/pokoj2";
    printf("SELECT ALL FROM %s/*  :\n",path);
    
    tree* branchM = getBranch(path);

    if(branchM != NULL){
        printf("Selected branch %s \n",branchM ->name_of_branch);
        printf("-----\n");
        printBranch(branchM,0);
        
        printf("-----\n");
        selectAllNodes(branchM);

        showSelectedItems();
        printf("-----\n");

        
    }else{
        printf("Brak \n");
    }

    free(branchM);
        
    killNodesList();
    killSelectedItems();
    return 0;
}
