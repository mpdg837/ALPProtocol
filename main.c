#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tree.c"


int main() {
    createListNodes();
    
    addWord("broker",511);
    addWord("budynek",512);
    addWord("mieszkanie1",513);
    addWord("mieszkanie2",514);
    addWord("pokoj1",515);
    addWord("pokoj2",516);
    addWord("szafa",517);

    mainTree = emptyBranch(encode("broker"));

    addNodePath("/budynek=511");
    addNodePath("/budynek=512");
    addNodePath("/budynek/mieszkanie1/pokoj1=513");
    addNodePath("/budynek/mieszkanie1/pokoj1/szafa=514");
    addNodePath("/budynek/mieszkanie1/pokoj2=515");
    addNodePath("/budynek/mieszkanie2=516");
    addNodePath("/budynek/mieszkanie2/pokoj1=517");
    addNodePath("/budynek/mieszkanie2/pokoj1=518");
    addNodePath("/budynek/mieszkanie2/pokoj2=519");
    addNodePath("/budynek/mieszkanie2/pokoj2/szafa=520");

    printf("================================================ \n");
    printf(" Registered Nodes: \n");
    printf("------------------------------------------------ \n");
    showLNodeList();
    printf("================================================ \n");
    printf(" Main Tree: \n");
    printf("------------------------------------------------ \n");
    
    printBranch(mainTree,0);
    
    printf("================================================ \n");
    char* path = "/budynek/mieszkanie1/pokoj1";
    printf("SELECT ALL FROM %s/*  :\n",path);

    char* encPath = encodePath(path);
    printShortPath(encPath);
    printf("\n");

    tree* branchM = getBranch(encPath);
    
    if(branchM != NULL){
        printf("------------------------------------------------\n");
        selectAllNodes(branchM,encPath);

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
