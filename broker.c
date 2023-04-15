#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./ALP/messages.c"
#include "./utils/readFile.c"

void program(){
    printf("================================================ \n");
    printf(" Known nodes: \n");
    printf("------------------------------------------------ \n");
    showLNodeList();
    printf("================================================ \n");
    printf(" Main Tree: \n");
    printf("------------------------------------------------ \n");
            
    printBranch(mainTree,0);
            
    printf("Init server ... \n");
    printf("Server Log: \n");
    printf("------------------------------------------------ \n");
        
    listener();
}
int main() {
    createListNodes();
  
    if(readMe("dict.txt")){
          printf("Load dictionary ... \n");
          while(1){
              char* line = readLine();
                
              if(line == NULL) break;
                
              addWordByFile(line);
              free(line);    
          }
           closeFile();
           
        if(readMe("tree.txt")){
            printf("Load tree schematic ... \n");
            mainTree = emptyBranch(32767);
            while(1){
                char* line = readLine();
                
                if(line == NULL) break;
                
                addNodePath(line);
                free(line);    
            }
            
            closeFile();
            
            program();
        }else{
            printf("Cannot load tree to broker \n");
        } 
    }else{
        printf("Cannot load dictionary to decode paths \n");
    }
    killServicesList(serviceSet);
    
    killSubscriberList();
    killNodesList();
    killSelectedItems();
    killDict();
    
    return 0;
}
