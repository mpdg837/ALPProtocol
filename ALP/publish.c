#include "./packets/packets.c"
#include "./udp/udpBroker.c"

int countSSubscribeMessages = 0;
int countSPublishMessages = 0;


void helloNode(int ip,char* context){
    if(context != NULL){
        
        short numberNode = ((unsigned char)context[0] << 8) | (unsigned char)context[1];
        
        node* myNode = getNode(numberNode);
        
        if(myNode != NULL){
            printf("Registered node number %d \n", numberNode);
            
            printf("\n");
            
            printf("Node's services: \n");
            updateServices(myNode,context);
            
            myNode -> connected = TRUE;
            myNode -> ip = ip;
            printf("\n");
            
            printf("Actual status of nodes: \n");
            showLNodeList();
            
        }else{
            printf("ERROR: Cannot find node in main tree of broker \n");
        }
    }else{
        printf("ERROR: Bad form of message \n");
    }
}




void forwardPublishMessage(lnode* nodes,char* message,unsigned char service, int len){
    lnode* analysedNode = nodes;
    if(analysedNode != NULL)
        while(1){
            
            if(analysedNode -> myNode != NULL){
                char* packetMessage = initPublishMessage();
                
                int n = 0;
                
                char* nodePath = analysedNode->path;
                
                while(1){
                    packetMessage[n+1] = nodePath[n];
                    
                    if(packetMessage[n+1] & 0x1) break;
                    n++;
                }
                n++;
                packetMessage[n + 1] = ((WILDCARD << 6) | (service << 1) | 0x1)&0xFF;
                n++;
                packetMessage[n + 1] = len;
                n++;
                
                for(int k=0;k<len;k++){
                    packetMessage[n + 1] = message[k];
                    n++;
                }
                
                int len = n+1;
                
                show_time();
                printf("Sending to IP: %s PUBLISH Message \n", decodeIPtoCharacters(analysedNode -> ipSubscriber));
         
                send_it(packetMessage,decodeIPtoCharacters(analysedNode -> ipSubscriber), len);
                
                free(packetMessage);
                countSPublishMessages++;
            }
            
            if(analysedNode -> next_item == NULL) break;
            analysedNode = analysedNode -> next_item;
        }
      
}


void analyseSimplePublish(int ip,char* context){
    // Publish Simple
                    
    char* content = malloc(sizeof(char)*QUERY_LEN);
    memset(content,'\0',sizeof(char)*QUERY_LEN);
            
    unsigned char service = (context[2] >> 1) & 0xF;
    unsigned char len = context[3];
            
    printf("Service: ");
    printService(service);
    printf("\n");
    printf("Length: %d \n",len);
    printf("Message: ");
    printf("\n");
    printf("Type: sensor \n");
    printf("\n");
    printf("[");
            
    for(int n=0;n<len;n++){
        printf("0x%x ",(unsigned char)context[n+4]);
        content[n] = context[n+4];
    }
            
    printf("]\n");
            
    node* node = getNodeByIp(ip);
            
    if(node != NULL){
        printf("Publish message is associated with * NODE %d \n",node -> numberNode);
                
        lnode* selected = sendAboutNode(node -> numberNode,service);
        showSelectedItems(selected );
        printf("\n");
        printf("Sending messages PUBLISH: \n");
        printf("\n");
                        
        forwardPublishMessage(selected,content,service,len);
        killCopiedList(selected);
    }else{ 
        printf("ALERT! Service from this node is not subscribed by any subscriber \n");
    }
            
    free(content);
}


void analyseDriverPublish(int ip,char* context){
 
    
    char* path = separatePath(context);
    
    printf("Path: ");
    printShortPath(path);
    printf("\n");
    printf("Service: ");
    
    char service = separateService(context);
    
    printService(service);
    printf("\n");
            
    printf("Message: \n[");
    
    char* value = separateValue(context);
    for(int n=0;n<valueLen;n++){
        printf("0x%x ",value[n]);
              
    }
    printf("]\n");
    
    int ipNext = getIpOfSubscriber(path);
    
    if(ipNext != -1){
        printf("Sending message ALP Publish to IP %s",decodeIPtoCharacters(ipNext));
        
        char* packetMessage = initPublishMessage();
        packetMessage[1] = 0x0;
        packetMessage[2] = 0x1;
        packetMessage[3] = (WILDCARD << 6) | (service << 1) | 0x1;
        packetMessage[4] = valueLen;
        
        int k=0;
        for(k=0;k<valueLen;k++){
            packetMessage[5+k] = value[k];
        }
        
        int nlen = 5+valueLen;
        
        send_it(packetMessage,decodeIPtoCharacters(ipNext), nlen);
        countSPublishMessages++;
        free(packetMessage);
    }else{
        printf("ERROR: I cannot detect associated with this driver node. \n");
    }
    
    free(value);
    free(path);
    
}

void analysePublish(int ip,char* context){
    if(context != NULL){
        if(context[0] == 0x0 && context[1] == 0x1){
            analyseSimplePublish(ip,context);
        }else{
            // Publish driver
            analyseDriverPublish(ip,context);

            
        }
    }else{
        printf("ERROR: Bad form of message \n");
    }
}

