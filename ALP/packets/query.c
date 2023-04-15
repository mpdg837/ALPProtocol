#include "./tree/tree.c"

#define QUERY_LEN 256

#define NO_OPERATION 0x0
#define OR_OPERATION 0x1
#define AND_OPERATION 0x2
#define WILDCARD 0x3

lnode* selectAll(char* encPath){

    lnode* selected = NULL;

    tree* branchM = getBranch(encPath);
    
    if(branchM != NULL){
        selectAllNodes(branchM,encPath);

        selected = copySelectedItems(selectedItems);
        if(selected != NULL) killSelectedItems();

    }else{
        printf("Brak \n");
    }

    if(branchM != NULL) free(branchM);
    return selected;
}

char* newQuery(char* shortPath){
    char* newQuery = malloc(sizeof(char)*QUERY_LEN);
    
    int n=0;
    
    char decision = FALSE;
    
    for(n=0;n<QUERY_LEN;n+=2){
          newQuery[n] = shortPath[n];
          newQuery[n+1] = shortPath[n+1];
          
          if(newQuery[n+1] & 0x1){
            decision = TRUE;
            break;
          }
    }
    
    if(n+2 < QUERY_LEN)
      newQuery[n+2] = 0x1;
    else decision = FALSE;
    
    if(decision) return newQuery;
    else NULL;
}

char* addPathToQuery(char* query, char* shortPath,char andorOr){
    int n=0;
    
    char finishing = FALSE;
    char start = FALSE;
    
    int k = 0;
    
    for(n=0;n<QUERY_LEN;n++){
          char znak = query[n];
          
          
          if(start){
            
            query[n] = shortPath[k];
            
            if(n+1 < QUERY_LEN){
                  query[n+1] = shortPath[k+1];
              
                  if(query[n+1] & 0x1){
                      break;
                  }
            }else{
                return NULL;
            }
            
            n++;
            k+=2;
          }else{
            if(znak & 0x1){
              if(finishing){
                start = TRUE;
                
                query[n] = ((andorOr & 0x3) << 6);
              }
              else finishing = TRUE;
            }else finishing = FALSE;
          }
    }
    
    query[n+2] = 0x1;
    return query;
}

lnode* analyseQuery(char* query){
    
    lnode* finalResult = NULL;
    
    int n = 0;
    int k = 0;
    
    char finished = FALSE;
    char lastOperation = NO_OPERATION;
    
    char* pathBuffer = malloc(sizeof(char) * QUERY_LEN);
    memset(pathBuffer,'\0',sizeof(char)*QUERY_LEN);
    
    printf("\n \n Operations : \n");
                    
                    
    for(n=0;n<QUERY_LEN;n+=2){
        if(n+1 < QUERY_LEN){
            
            pathBuffer[k] = query[n];
            pathBuffer[k+1] = query[n+1];
            
            if(query[n+1] & 0x1){
                
                if(n+2 < QUERY_LEN){

                    
                    
                       unsigned char operation = (query[n+2] >> 6);
                       
                       lnode* set = NULL;
                          
                       switch (lastOperation){
                          case NO_OPERATION:
                             
                              printf("PATH: ");
                              printShortPath(pathBuffer);
                              printf("\n");
                    
                              finalResult = selectAll(pathBuffer);
                              break;
                          case OR_OPERATION:
                          
                              printf("OR PATH: ");
                              printShortPath(pathBuffer);
                              printf("\n");
                                                          
                              set = selectAll(pathBuffer);
                              finalResult = orItem(finalResult,set);
                              
                              
                              break;
                          case AND_OPERATION: 

                              printf("AND PATH: ");          
                              printShortPath(pathBuffer);
                              
                              set = selectAll(pathBuffer);
                              finalResult = andItem(finalResult,set);
                              
                              printf("\n");
                              
                              
                              
                              break;
                           default:
                             break;
                       }
                       
                       lastOperation = operation;
                       
                    
                    if(query[n+2] & 0x1) {
                      finished = TRUE;
                      break;
                    }
                    
                    memset(pathBuffer,'\0',sizeof(char)*QUERY_LEN);
                    
                    n++;
                    k = 0;
                }else{
                    free(pathBuffer);
                    return NULL;
                }
            }else k+=2;
        }else{
            free(pathBuffer);
            return NULL;
        }
        
    }
    
    free(pathBuffer);
    
    if(finished) {
       printf(" \n Results: \n");      
      return finalResult;
      
    }
    else return NULL;
}


