#include "./services.c"

lnode* lastOpeationSelectedNodes = NULL;
char lastService = ALL_SERVICES;

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


void printFilters(char service){

    printf("Filters : ");
    
    if(service == ALL_SERVICES)
      printf("All services \n");
    else 
      printf("Service %d \n",service);
      
}


serviceNode* analyseOneUnSubscribe(serviceNode* list, char* subscribe,int ip){
    
    if(subscribe[0] == 0x0 && subscribe[1] == 0x1){
        killServicesList(list);
        return NULL;
    }
    
    killServicesList(serviceSet);
    serviceSet = duplicateServicesList(list);
    killServicesList(list);
    
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
    
    printFilters(service);
    
    if(!detected) return NULL;
    else{
      lnode* prefinal = sensorFilter(orset,service);
    
      showSelectedItems(prefinal);  
      removeAllToListSensorNode(prefinal, service);
      
      copySelectedItems(prefinal);
      killCopiedList(prefinal);
    
      printf("\n \n Subscribed services by ");
      printf("%s",decodeIPtoCharacters(ip));
      printf("\n");
    
      return duplicateServicesList(serviceSet);
    }
}


serviceNode* analyseOneSubscribe(serviceNode* list, char* subscribe, int ip){
    
    if(lastOpeationSelectedNodes == NULL) killCopiedList(lastOpeationSelectedNodes);
    
    if(subscribe[0] == 0x0 && subscribe[1] == 0x1){
        printf("ERROR: Prohibited path \n");
        return list;
    }
    
    
    killServicesList(serviceSet);
    serviceSet = duplicateServicesList(list);
    killServicesList(list);
    
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
    
    printFilters(service);
    
    lastService = service;
    
    if(!detected) return NULL;
    else{
    
      
      lnode* prefinal = sensorFilter(orset,service);
        
      showSelectedItems(prefinal);  
      addAllToListSensorNode(prefinal, service);
      
      lastOpeationSelectedNodes = copySelectedItems(prefinal);
      
      killCopiedList(prefinal);
      printf("\n \n Subscribed services by ");
      printf("%s",decodeIPtoCharacters(ip));
      printf("\n");
    
      return duplicateServicesList(serviceSet);
    }
}

void showSelectedServices(serviceNode* list){
    serviceNode* listElement = list;
    
    printf("------------------------------------------------- \n");
    
    while (listElement != NULL)
    {
        printf("* NODE ID  : %d in path: /",listElement -> myNode ->numberNode);  
        
        printShortPath(listElement -> path);  
        
        printf(" service: ");
        printService(listElement -> typeSensor);
        
        printf("\n");

        listElement = listElement -> next_item;
    }
    printf("------------------------------------------------- \n");
}


