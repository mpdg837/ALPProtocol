#include "./branch.c"
#include "./utils/extractNumber.c"

#define isNumber(character) character >= '0' && character <= '9'

#define SEPARATOR (char)'/'
#define CR 13
#define LF 10

int detectNewNode(char* myConfig){
    int number = getNumber(myConfig);
    
    if(number !=0)
        addNewNode(number,lastIsDriverNode);    
    
    return number;
}

tree* findBranch(tree* analysedTree,short idTopic){
    tree* analysedBranch = analysedTree;
    

    while (analysedBranch != NULL)
    {

        if(analysedBranch -> subbranch != NULL)
            if(analysedBranch -> id == idTopic){
                return analysedBranch -> subbranch;
                break;
            }

    
        analysedBranch = analysedBranch -> next_branch;
    }
    return NULL;
}

shortcut* findNodes(tree* analysedTree,short idTopic){
    tree* analysedBranch = analysedTree;

    while (analysedBranch != NULL)
    {

        if(analysedBranch -> nodes != NULL)
            if(analysedBranch -> id == idTopic){
                return analysedBranch -> nodes;
                
                break;
            }

    
        analysedBranch = analysedBranch -> next_branch;
    }
    return NULL;
}

tree* getBranch(char* shortConfig){
    
    if(shortConfig[0] == 0x0 && shortConfig[1] == 0x1){
        return mainTree;
    }else{
        tree* analyseTree = mainTree -> subbranch;
        shortcut* analyseNode = NULL;

        char pathBuffer[DIRECOTRY_BUFFER_SIZE];
        memset(pathBuffer,'\0',sizeof (char)*DIRECOTRY_BUFFER_SIZE);
        
        short idTopic;
        char finAnalyse = FALSE;

        int n=0;
        for(n=0;n<DIRECOTRY_BUFFER_SIZE;n+=2){
                
            idTopic = ((unsigned char)shortConfig[n] << 7) | (((unsigned char)shortConfig[n+1] & 0xFE) >> 1);

            analyseNode = findNodes(analyseTree,idTopic);
            analyseTree = findBranch(analyseTree,idTopic);
            
            if(shortConfig[n+1] & 0x1 == 0x1 || analyseTree == NULL){
                finAnalyse = TRUE;
                break;
            }
        }
        
        tree* selectedBranch = NULL;

        if(finAnalyse){
            selectedBranch = emptyBranch(idTopic);
            selectedBranch -> subbranch = analyseTree;
            selectedBranch -> nodes = analyseNode;
        }

        return selectedBranch;
    }
}


void addNodePath(char* myConfig){
    if(myConfig != NULL){
        if(strlen(myConfig) > 0){
          char pathBuffer[DIRECOTRY_BUFFER_SIZE];
      
          memset(pathBuffer,'\0',sizeof (char)*DIRECOTRY_BUFFER_SIZE);
      
          int k=0;
          char readNumber = FALSE;
      
          actBranch = mainTree;
          char init = FALSE;
      
          int finloop = FALSE;
          int nodeIndex = 0;
      
          int n=0;
      
          int numberNode = detectNewNode(myConfig);
          
              for(n=0;n<DIRECOTRY_BUFFER_SIZE;n++){
                  switch (myConfig[n]) {
                      case SEPARATOR:
                      case CR:
                      case LF:
                      case '\0':
                      case '=':
                      case '+':
                          if(myConfig[n] == SEPARATOR && !init && n==0){
                              init = TRUE;
                              k = 0;
                          }else if(init){
                              
                               char* destination = malloc(sizeof(char) * DIRECOTRY_BUFFER_SIZE);   
                               memset(destination, '\0', sizeof(char) * DIRECOTRY_BUFFER_SIZE);
                             
                              strcpy(destination, pathBuffer);
                              
                              goNextBranch(destination);
                              memset(pathBuffer, '\0', sizeof(char) * DIRECOTRY_BUFFER_SIZE);
                              k = 0;
                              
                              if (myConfig[n] == '+' || myConfig[n] == '=') {
                                  if(numberNode != 0)
                                      addNode(numberNode);
                               
                                  finloop = TRUE;
                              }
                              
                              
                          }
                          break;
                      default:
          
                          if(init) {
                              pathBuffer[k] = myConfig[n];
                              k++;
                          }
                          break;
                  }
          
                  if(finloop) break;
              }
          
        }
    }
}
