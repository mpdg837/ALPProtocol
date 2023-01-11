#include "./wildcard.c"

typedef struct serviceNode{
    node* myNode;
    char* path;
    char typeSensor;
    
    struct serviceNode* next_item;
}serviceNode;

static serviceNode* serviceSet = NULL;

serviceNode* newListSensorNode(node* myNode,char* path, char typeSensor){
           
    serviceNode* newList = malloc(sizeof(serviceNode));
    
    newList -> myNode = myNode;
    newList -> path = NULL;
    
    newList -> path = malloc(sizeof(char)*DIRECOTRY_BUFFER_SIZE);
    copyPath(newList -> path , path);
    
    newList -> typeSensor = typeSensor;
    newList -> next_item = NULL;
    
    return newList;
} 

void freeListSensorNode(serviceNode* item){
    
    free(item -> path);
    free(item);
}

void removefromListSensorNode(node* myNode,char* path, char typeSensor){

    
    if(serviceSet != NULL){
   
        if(serviceSet -> typeSensor == typeSensor){
           if(serviceSet -> myNode == myNode){
               
               serviceNode* saved = serviceSet -> next_item;
               freeListSensorNode(serviceSet);
               
               serviceSet = saved -> next_item;
               
               return (void)0;
           }
        }
        
        serviceNode* analysedNode = serviceSet;
        serviceNode* lastItem = serviceSet;
        
        while(1){
            if(analysedNode -> myNode == myNode){
                if(analysedNode -> typeSensor == typeSensor){
                    
                    lastItem -> next_item = analysedNode -> next_item;
                    
                    freeListSensorNode(analysedNode);
                    return (void)0;
                }
                
            }
            
            if(analysedNode -> next_item == NULL) break;
            lastItem = analysedNode;
            analysedNode = analysedNode -> next_item;
        }
        
    }
    
    return (void)0;

}

void addtoListSensorNode(node* myNode,char* path, char typeSensor){

    
    if(serviceSet == NULL){
        
        serviceSet = newListSensorNode(myNode,path,typeSensor);
    }else{
        serviceNode* analysedNode = serviceSet;
        
        while(1){
            if(analysedNode -> myNode == myNode){
                if(shortPathComparer(analysedNode -> path, path)){
                    if(analysedNode -> typeSensor == typeSensor){
                      return (void)0;
                    }
                }
            }
            
            if(analysedNode -> next_item == NULL){
                serviceNode* nitem = newListSensorNode(myNode,path,typeSensor);
                analysedNode -> next_item = nitem;
                break;
            }
            analysedNode = analysedNode -> next_item;
        }
        
    }
    
    return (void)0;

}

void addAllToListSensorNode(lnode* listNodes, char service){
    lnode* analysedNode = listNodes;
    
    if(analysedNode != NULL)
        while(1){
            addtoListSensorNode(analysedNode -> myNode,analysedNode -> path, service);
            
            if(analysedNode -> next_item == NULL) break;
            analysedNode = analysedNode -> next_item;
        }
}

void removeAllToListSensorNode(lnode* listNodes, char service){
    lnode* analysedNode = listNodes;
    
    if(analysedNode != NULL)
        while(1){
            removefromListSensorNode(analysedNode -> myNode,analysedNode -> path, service);
            
            if(analysedNode -> next_item == NULL) break;
            analysedNode = analysedNode -> next_item;
        }
}

serviceNode* analyseOneUnSubscribe(serviceNode* list, char* subscribe){

    serviceSet = list;
    
    lnode* orset = analyseQuery(subscribe);

    if(orset == NULL) return NULL;
    
    int n=0;
    
    char finishing = FALSE;
    char service = 0;
    char detected = FALSE;
    
    for(n=0;n<QUERY_LEN;n++){
          char znak = subscribe[n];
          
      
            if(znak & 0x1){
              if(finishing){
                if((subscribe[n] >> 6)& 0x3 == WILDCARD){                        
                  service = (subscribe[n] >> 1) & 0xF;
              
                  
                  detected = TRUE;
                }
                break;
              }
              else finishing = TRUE;
            }else finishing = FALSE;
          
    }
    
    printf("Service %d \n",service);
    printf("\n \n SUBSCRIBED SERVICES BY 1: \n");
    
    if(!detected) return NULL;
    else{
      lnode* prefinal = sensorFilter(orset,service);
    
    
      removeAllToListSensorNode(prefinal, service);
    
      serviceNode* answer = serviceSet;
      serviceSet = NULL;
    
    
      return answer;
    }
}


serviceNode* analyseOneSubscribe(serviceNode* list, char* subscribe){

    serviceSet = list;
    
    lnode* orset = analyseQuery(subscribe);

    if(orset == NULL) return NULL;
    
    
    
    int n=0;
    
    char finishing = FALSE;
    char service = 0;
    char detected = FALSE;
    
    for(n=0;n<QUERY_LEN;n++){
          char znak = subscribe[n];
          
      
            if(znak & 0x1){
              if(finishing){
                if((subscribe[n] >> 6)& 0x3 == WILDCARD){                        
                  service = (subscribe[n] >> 1) & 0xF;
              
                  
                  detected = TRUE;
                }
                break;
              }
              else finishing = TRUE;
            }else finishing = FALSE;
          
    }
    
    printf("Service %d \n",service);
    printf("\n \n SUBSCRIBED SERVICES BY 1: \n");
    
    if(!detected) return NULL;
    else{
      lnode* prefinal = sensorFilter(orset,service);
    
    
      addAllToListSensorNode(prefinal, service);
    
      serviceNode* answer = serviceSet;
      serviceSet = NULL;
    
    
      return answer;
    }
}

void showSelectedServices(serviceNode* list){
    serviceNode* listElement = list;
    
    printf("------------------------------------------------- \n");
    
    while (listElement != NULL)
    {
        printf("* NODE ID  : %d in path: /",listElement -> myNode ->numberNode);  
        
        printShortPath(listElement -> path);  
        
        printf(" service: %d",listElement -> typeSensor);
        printf("\n");

        listElement = listElement -> next_item;
    }
    printf("------------------------------------------------- \n");
}
