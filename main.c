#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./packets.c"


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
    addWord("szafax",600);
    
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

    // Message 1
    char* path = "/budynek/mieszkanie1/pokoj1/szafa";
    char* spath = encodePath(path);
    char* query = newQuery(spath);
    
    path = "/budynek/mieszkanie2";
    spath = encodePath(path); 
    query = addPathToQuery(query,spath,OR_OPERATION);
    query = addWildcard(query,PRESSURE);
     
    char* message = makeSubscribeMessage(query);
    
    free(spath);
    
    receiveMessage(message,10);
    
    // Message2
    char* path1 = "/budynek/mieszkanie1/pokoj2";
    char* spath1 = encodePath(path1);
    char* query1 = newQuery(spath1);
    
    path1 = "/budynek/mieszkanie1/pokoj1/szafa";
    spath1 = encodePath(path1); 
    query1 = addPathToQuery(query1,spath1,OR_OPERATION);
    
    query1 = addWildcard(query1,TEMPERATURE);
     
    char* message1 = makeSubscribeMessage(query1);
    
    free(spath1);
    
    receiveMessage(message1,10);  

    // Message3
    char* path2 = "/budynek";
    char* spath2 = encodePath(path2);
    char* query2 = newQuery(spath2);
    
    query2 = addWildcard(query2,PRESSURE);
     
    char* message2 = makeUnSubscribeMessage(query2);
    
    free(spath2);
    
    receiveMessage(message2,10);  
    
        
    killNodesList();
    killSelectedItems();
    killDict();
    return 0;
}
