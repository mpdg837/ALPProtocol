#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tree.c"


int main() {
    createListNodes();
    
    addWord("broker",0);
    addWord("budynek",1);
    addWord("mieszkanie1",2);
    addWord("mieszkanie2",3);
    addWord("pokoj1",4);
    addWord("pokoj2",5);
    addWord("szafa",6);

    mainTree = emptyBranch(encode("broker"));

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

    printf("================================================ \n");
    printf(" Registered Nodes: \n");
    printf("------------------------------------------------ \n");
    showLNodeList();
    printf("================================================ \n");
    printf(" Main Tree: \n");
    printf("------------------------------------------------ \n");
    
    printBranch(mainTree,0);
    
    printf("================================================ \n");
    char* path = "/budynek/mieszkanie2/pokoj1";
    printf("SELECT ALL FROM %s/*  :\n",path);

    char* encPath = encodePath(path);
    tree* branchM = getBranch(encPath);

    if(branchM != NULL){
        printf("------------------------------------------------\n");
        selectAllNodes(branchM);

        lnode* selected = copySelectedItems();
        killSelectedItems();

        showSelectedItems(selected);
        killCopiedList(selected);

        printf("================================================ \n");

        
    }else{
        printf("Brak \n");
    }

    free(branchM);

    killNodesList();
    killSelectedItems();
    killDict();
    return 0;
}
