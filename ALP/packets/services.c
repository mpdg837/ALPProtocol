#include "./wildcard.c"

typedef struct serviceNode{
    node* myNode;
    char* path;
    char typeSensor;
    
    struct serviceNode* next_item;
}serviceNode;

serviceNode* serviceSet = NULL;

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

void killServicesList(serviceNode* list){
    
    serviceNode* analysedItem = list;
    if(analysedItem != NULL)
        while(1){
            
            freeListSensorNode(analysedItem);
            
            if(analysedItem -> next_item == NULL) break;
            analysedItem = analysedItem -> next_item;
        }
}


serviceNode* addToDuplicatedServicesList(serviceNode* newList, serviceNode* newElement){
    
    serviceNode* analysedNode = newList;
    
    if(newList == NULL){
        if(newElement != NULL)
            newList = newListSensorNode(newElement -> myNode,newElement -> path, newElement -> typeSensor);
    }else{
        while(1){
            
            if(analysedNode -> next_item == NULL) {
                serviceNode* newE = newListSensorNode(newElement -> myNode,newElement -> path, newElement -> typeSensor);
                analysedNode -> next_item = newE;
              break;
            }
            analysedNode = analysedNode -> next_item;
        }
    }
    
    return newList;

}

serviceNode* duplicateServicesList(serviceNode* list){

    serviceNode* newList = NULL;
    serviceNode* analysedNode = list;
    
    if(analysedNode != NULL)
        while(1){
            
            newList = addToDuplicatedServicesList(newList,analysedNode);
            
            if(analysedNode -> next_item == NULL) break;
            analysedNode = analysedNode -> next_item;
        }

    return newList;
}
void removefromListSensorNode(node* myNode,char* path, char typeSensor){

    if(serviceSet != NULL){
   
        if(serviceSet -> typeSensor == typeSensor || typeSensor == ALL_SERVICES){
           if(serviceSet -> myNode == myNode){
                 
               serviceNode* saved = serviceSet -> next_item;
               freeListSensorNode(serviceSet);
               
               serviceSet = saved -> next_item;
               
               if(serviceSet == NULL) return (void) 0;
           }
        }
        
        serviceNode* analysedNode = serviceSet;
        serviceNode* lastItem = serviceSet;
        
        while(1){
            char removed = FALSE;
            
           if(analysedNode -> typeSensor == typeSensor || typeSensor == ALL_SERVICES){
              if(analysedNode -> myNode == myNode){
               
                    lastItem -> next_item = analysedNode -> next_item;
                    
                    freeListSensorNode(analysedNode);
                    
                    removed = TRUE;
                    
                }
                
            }
                    
            if(removed){
            
              if(lastItem -> next_item == NULL) break;
              analysedNode = lastItem -> next_item;
              
            }else{
            
              if(analysedNode -> next_item == NULL) break;
              lastItem = analysedNode;
              analysedNode = analysedNode -> next_item;
            }
        }
        
    }
    
    return (void)0;

}

void addtoListSensorNode(node* myNode,char* path, char typeSensor){

    
    if(serviceSet == NULL){
        
        serviceSet = newListSensorNode(myNode,path,typeSensor);
    }else{
        
        serviceNode* analysedNode = serviceSet;
        serviceNode* lastItem = serviceSet;
        
        char isAddedItem = FALSE;
        char isAllServices = FALSE;

        while(1){
            
            char removed = FALSE;
                
            if(analysedNode -> typeSensor == typeSensor || typeSensor == ALL_SERVICES){
                if(analysedNode -> myNode == myNode){
                      
                        if(!isAllServices){
                              
                              if(typeSensor == ALL_SERVICES) {
                                  analysedNode -> typeSensor = ALL_SERVICES;
                                  isAllServices = TRUE;   
                              }
                              
                              isAddedItem = TRUE;
                        }else{
                              
                              lastItem -> next_item = analysedNode -> next_item;
                              freeListSensorNode(analysedNode);
                              
                              removed = TRUE;                    
                        }
                      
                      
                    
                }
            }
            
            if(removed){
                
                if(lastItem -> next_item == NULL) {
                      if(!isAddedItem){
                          serviceNode* nitem = newListSensorNode(myNode,path,typeSensor);
                          lastItem -> next_item = nitem;
                      }             
                      break;
                }
                
                analysedNode = lastItem -> next_item;                
                
            }else{
                if(analysedNode -> next_item == NULL){
                    if(!isAddedItem){
                        serviceNode* nitem = newListSensorNode(myNode,path,typeSensor);
                        analysedNode -> next_item = nitem;
                    }
                    
                    break;
                }
                
                lastItem = analysedNode;
                analysedNode = analysedNode -> next_item;
            }
        }
        
    }
    

}


