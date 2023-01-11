#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tree.c"


lnode* selectAll(char* path){
    printf("================================================ \n");
    printf("SELECT ALL FROM %s/*  :\n",path);

    char* encPath = encodePath(path);
  
    
    lnode* selected = NULL;

    tree* branchM = getBranch(encPath);
    
    if(branchM != NULL){
        printShortPath(encPath);
         printf("\n");
        printf("------------------------------------------------\n");
        selectAllNodes(branchM,encPath);

        selected = copySelectedItems();
        killSelectedItems();

        showSelectedItems(selected);

        printf("================================================ \n");

        
    }else{
        printf("Brak \n");
    }

    free(branchM);
    return selected;
}
int main() {
    createListNodes();
    
    addWord("kategorie",0);
    addWord("budynek",512);
    addWord("mieszkanie1",513);
    addWord("mieszkanie2",514);
    addWord("pokoj1",515);
    addWord("pokoj2",516);
    addWord("szafa",517);
    addWord("blok",518);

    mainTree = emptyBranch(32767);

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
    addNodePath("/blok=533");

    printf("================================================ \n");
    printf(" Registered Nodes: \n");
    printf("------------------------------------------------ \n");
    showLNodeList();
    printf("================================================ \n");
    printf(" Main Tree: \n");
    printf("------------------------------------------------ \n");
    
    printBranch(mainTree,0);

    char* path = "/budynek/mieszkanie1";
    lnode* set1 = selectAll(path);
    
    path = "/budynek/mieszkanie2/pokoj2";
    lnode* set2 = selectAll(path);

    printf("================================================ \n");
    printf(" OR OPERATION: \n");
    printf("------------------------------------------------ \n");

    lnode* orset = orItem(set1,set2);

    showSelectedItems(orset);

    killNodesList();
    killSelectedItems();
    killDict();
    return 0;
}
