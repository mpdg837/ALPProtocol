#include "./query.c"

#define ALL_SERVICES 0x0

#define TEMPERATURE 0x1
#define PRESSURE 0x2
#define HUMITIDY 0x3
#define LIGHTNESS 0x4
#define COMMAND 0x5
#define INFO 0x6

#define SERVICE_ERROR 0xFF;

void printService(char service){
            
    switch(service){
        case ALL_SERVICES:
            printf("All");
            break;
        case TEMPERATURE:
            printf("+ temperature");
            break;
        case PRESSURE:
            printf("+ pressure");
            break;
        case HUMITIDY:
            printf("+ humitidy");
            break;
        case LIGHTNESS:
            printf("+ lightness");
            break;
        case COMMAND:
            printf("+ command");
            break;
        case INFO:
            printf("+ info");
            break;
        default:
            break;
    }
    
    
}

lnode* sensorFilter(lnode* set, char service){

    results = NULL;
    
    lnode* analysedItem = set;
    char detected = FALSE;

    while(1){

        if(analysedItem -> myNode != NULL){
            
            node* myNode = analysedItem -> myNode;
            char decision = FALSE;
            
            switch(service){
                case ALL_SERVICES:
                  decision = TRUE;
                  break;
                case TEMPERATURE:
                  if(myNode ->temperature) decision = TRUE;
                  break;
                case PRESSURE:
                  if(myNode -> pressure) decision = TRUE;
                  break;
                case HUMITIDY:
                  if(myNode -> humitidy) decision = TRUE;
                  break;
                case LIGHTNESS:
                  if(myNode -> lightness) decision = TRUE;
                  break;
                case COMMAND:
                  if(myNode -> command) decision = TRUE;
                  break;
                case INFO:
                  if(myNode -> info) decision = TRUE;
                  break;
                default:
                  break;
            }

            if(decision){
                addOperElement(myNode,analysedItem -> path);
                detected = TRUE;
            } 
        }

        if(analysedItem -> next_item == NULL) break;
        analysedItem = analysedItem -> next_item;
    }

    lnode* newRes = NULL;

    if(set != NULL) killCopiedList(set);

    if(detected){
        newRes = copySelectedItems(results);
        killCopiedList(results);
    }

    return newRes;
}

char* addWildcard(char* query,char service){
    int n=0;
    
    char finishing = FALSE;
    
    for(n=0;n<QUERY_LEN;n++){
          char znak = query[n];
          
      
            if(znak & 0x1){
              if(finishing){
                            
                query[n] =  (WILDCARD << 6) | ((service & 0xF) << 1) | 0x1;
                break;
              }
              else finishing = TRUE;
            }else finishing = FALSE;
          
    }
    
    return query;
}