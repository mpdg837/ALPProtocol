#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tree.c"


int main() {
    mainTree = emptyBranch("broker");

    addNodePath("/budynek=1");
    addNodePath("/budynek=15");
    addNodePath("/budynek/mieszkanie1/pokoj1=2");
    addNodePath("/budynek/mieszkanie1/pokoj1/szafa=3");
    addNodePath("/budynek/mieszkanie1/pokoj2=4");
    addNodePath("/budynek/mieszkanie2=5");
    addNodePath("/budynek/mieszkanie2/pokoj1=6");
    addNodePath("/budynek/mieszkanie2/pokoj1=7");
    addNodePath("/budynek/mieszkanie2/pokoj2=8");
    addNodePath("/budynek/mieszkanie2/pokoj2/szafa=9");



    printBranch(mainTree,0);

    return 0;
}
