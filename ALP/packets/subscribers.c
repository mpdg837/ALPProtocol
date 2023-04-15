#include "./servicesOperations.c"

typedef struct subscriber{
    int ip;
    char connected;
    
    serviceNode* services;
    
    struct subscriber* next_sub;
}subscriber;

static subscriber* allSubscribers = NULL;


subscriber* newSubscriber(int ip){
    subscriber* newSub = malloc(sizeof(subscriber));
    
    newSub -> ip = ip;
    newSub -> connected = FALSE;
    
    newSub -> services = NULL;
    newSub -> next_sub = NULL;
    
    return newSub;
}

void killSubscriber(subscriber* item){
    
    killServicesList(item -> services);
    free(item);
}

int getIpOfSubscriber(char* path){
    subscriber* analysedSubscriber = allSubscribers;
    
    if(analysedSubscriber != NULL)
        while(1){
            
            int ip = analysedSubscriber -> ip;
            
            if(analysedSubscriber ->services != NULL){
                
                serviceNode* analysed = analysedSubscriber ->services;
                while(1){
                
                    if(analysed -> path != NULL){
                        if(pathComparer(path,analysed -> path)) {
                            return ip;
                        }
                    }
                    
                    if(analysed -> next_item == NULL) break;
                    analysed  = analysed -> next_item;
                }
            }
            
            if(analysedSubscriber -> next_sub == NULL) return -1;
            
            analysedSubscriber = analysedSubscriber -> next_sub;
        }
    return -1;
}

void removeSubscriber(int ip){
    subscriber* analysedSubscriber = allSubscribers;
    subscriber* lastSubscriber = allSubscribers;
    
    char removed = FALSE;
    
    if(analysedSubscriber != NULL){    
       
        if(analysedSubscriber -> ip == ip){
           lastSubscriber = allSubscribers -> next_sub;
           killSubscriber(allSubscribers);
           
           allSubscribers = lastSubscriber;
           
           return (void)0;
       }
       
        while(1){
            
            if(analysedSubscriber -> ip == ip){
                
                lastSubscriber -> next_sub = analysedSubscriber -> next_sub;
                removed = TRUE;
                killSubscriber(analysedSubscriber);
                            
                break;
            }
            
            if(removed){
                if(lastSubscriber -> next_sub == NULL) break;
                analysedSubscriber = lastSubscriber -> next_sub;
            }else{
                if(analysedSubscriber -> next_sub == NULL) break;
                 
                lastSubscriber = analysedSubscriber;
                analysedSubscriber = analysedSubscriber -> next_sub;
            }
        }
    }
}

subscriber* addSubscriber(int ip){
    if(allSubscribers == NULL){
        allSubscribers = newSubscriber(ip);
        
        return allSubscribers;
    }else{
        
        subscriber* analysedSub = allSubscribers;
        
        while(1){
            if(analysedSub -> next_sub == NULL) {
                 
                subscriber* newSub = newSubscriber(ip);
             
                analysedSub -> next_sub = newSub;
                    
                return analysedSub -> next_sub;
            }
          
            analysedSub = analysedSub -> next_sub;
        }
        
    }

}

subscriber* getSubscriberByIp(int ip){
    if(allSubscribers == NULL){
        return NULL;
    }else{
        
        subscriber* analysedSub = allSubscribers;
        
        while(1){
        
            if(analysedSub -> ip == ip){
                return analysedSub;
            }
            
            if(analysedSub -> next_sub == NULL) {
                return NULL;
            }
            analysedSub = analysedSub -> next_sub;
        }
        
    }
}

void killSubscriberList(void){
    if(allSubscribers != NULL){
        subscriber* analysedSub = allSubscribers;
        
        while(analysedSub != NULL){
           
            subscriber* saved = analysedSub;
            
            analysedSub = analysedSub -> next_sub;
            
            killServicesList(saved -> services);
            free(saved);
        }
    }
}


void analyseAboutNodeBySubscriber(short numberNode, unsigned char service, serviceNode* services, int ip){
    
    serviceNode* analysedService = services;
    
    while(1){
        
        if(analysedService -> myNode != NULL){
            
            
            if(analysedService -> typeSensor == service || analysedService -> typeSensor == ALL_SERVICES){
                if(analysedService -> myNode -> numberNode == numberNode){
                    
                    node* myNode = analysedService -> myNode;
                    
                    addSelectedItem(myNode,analysedService ->path,ip);
                    
                    
                }
            }
            
        }
        
        if(analysedService -> next_item == NULL) break;
        analysedService = analysedService -> next_item;
    }
}

lnode* sendAboutNode(short numberNode,unsigned char service){
    
    initNewSelectionItems();
    subscriber* analysedSubscriber = allSubscribers;
    
    if(analysedSubscriber != NULL)
        while(1){
        
             if(analysedSubscriber -> services != NULL){
                 
                 analyseAboutNodeBySubscriber(numberNode,service,analysedSubscriber -> services,analysedSubscriber -> ip);
             }
             
             if(analysedSubscriber -> next_sub == NULL) break;
             analysedSubscriber = analysedSubscriber -> next_sub;   
        }
    
    return copySelectedItems(selectedItems);        
}
