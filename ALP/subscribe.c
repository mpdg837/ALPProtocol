#include "./publish.c"


void forwardSubscrieMessage(char service){
    lnode* analysedNodes = lastOpeationSelectedNodes;
    
    if(analysedNodes != NULL)
      while(1){
          
          if(analysedNodes -> myNode != NULL){
          
              if(analysedNodes -> myNode -> ip != 0){
                  show_time();
                  printf("Sending to IP: %s SUBSCRIBE Message \n", decodeIPtoCharacters(analysedNodes -> myNode -> ip));
                  
                  char* spath = analysedNodes -> path;
                  char* query = newQuery(spath);
                   
                  query = addWildcard(query,service);
                   
                  char* myMessage = makeSubscribeMessage(query);
                  
                  free(spath);
                  
                  int len = packetLen(myMessage);
                  
                  send_it(myMessage,decodeIPtoCharacters(analysedNodes -> myNode -> ip), len);
                  
                  countSSubscribeMessages++;
                  free(myMessage);
              }
          }
          
          if(analysedNodes -> next_item == NULL) break;
          analysedNodes = analysedNodes -> next_item;
      }
}

void subscribe(int ip,char* context){
    if(context != NULL){
    
        subscriber* mySubscriber = NULL;
        mySubscriber = getSubscriberByIp(ip);
                  
        if(mySubscriber == NULL){
            mySubscriber = addSubscriber(ip);
                      
            printf("\n New subscriber IP: %s \n",ip_address_recv);
                     
        }
                      
        mySubscriber -> services = analyseOneSubscribe(mySubscriber -> services,context,ip);
                  
        showSelectedServices(mySubscriber -> services);
            
        printf("\n New subscribed nodes: \n");
        showSelectedItems(lastOpeationSelectedNodes);
                  
        forwardSubscrieMessage(lastService);
    }else{
        printf("ERROR: Bad form of message \n");  
    }
}

void unSubscribe(int ip,char* context){
    
    if(context != NULL){
        subscriber* mySubscriber = NULL;
        mySubscriber = getSubscriberByIp(ip);
                  
        if(mySubscriber == NULL){
            mySubscriber = addSubscriber(ip);
            printf("New subscriber IP: %s \n",ip_address_recv);
        }
                  
        mySubscriber -> services = analyseOneUnSubscribe(mySubscriber -> services,context,ip);
        
        if(mySubscriber -> services  == NULL){
            printf("Remove all. \n");
            
            removeSubscriber(ip);
        }else{
            showSelectedServices(mySubscriber -> services);
        }
    }else{
        printf("ERROR: Bad form of message \n");
    }
}