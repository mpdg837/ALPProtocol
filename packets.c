#include "./subscribents.c"
#include <stdlib.h>

#define ALP_IDENTITY 0x15

#define SUBSCRIBE 0x1
#define UNSUBSCRIBE 0x2
#define PUBLISH 0x3

#define HELLO 0x4
#define AVAILABLE 0x5


char isMessage(char* message){
    char identity = (message[0] >> 3) & 0x1F;
    
    if(identity == ALP_IDENTITY) return TRUE;
    
    return FALSE;
}

char getType(char* message){
    return message[0] & 0x7;
}

char* addHeader(char* newMessage, char topic,char* restOfMessage){
    newMessage[0] = topic;
    
    int n = 0;
    
    char finishing = FALSE;
    char detected = FALSE;
    
    for(n = 0; n < QUERY_LEN; n++){
        
        newMessage[n + 1] = restOfMessage[n];
        
        if(restOfMessage[n] & 0x1){
            if(finishing){
              detected = TRUE;
              break;
            }
            finishing = TRUE;
        }else finishing = FALSE;
        
    };
    
    if(detected) return newMessage;
    else {
        free(newMessage);
        return NULL;
    }
}

char* makeSubscribeMessage(char* context){
    char* newMessage = malloc(sizeof(char)*QUERY_LEN);
    
    newMessage = addHeader(newMessage, (ALP_IDENTITY << 3) | SUBSCRIBE,context);
    
    free(context);
    return newMessage;
}

char* makeUnSubscribeMessage(char* context){
    char* newMessage = malloc(sizeof(char)*QUERY_LEN);
    
    newMessage = addHeader(newMessage, (ALP_IDENTITY << 3) | UNSUBSCRIBE,context);
    
    free(context);
    return newMessage;
}


char* separateContext(char* message){
    char* context = malloc(sizeof(char)*QUERY_LEN);
    
    int n = 0;
    
    char finishing = FALSE;
    char detected = FALSE;
    
    for(n = 0; n < QUERY_LEN; n++){
        
        context[n] = message[n+1];
        
        if(context[n] & 0x1){
            if(finishing){
              detected = TRUE;
              break;
            }
            finishing = TRUE;
        }else finishing = FALSE;
        
    };
    
    if(detected) return context;
    else {
        free(context);
        return NULL;
    }
    
}

serviceNode* selectedServices = NULL;

void receiveMessage(char* message, int ip){
    if(isMessage(message)){
        
        char* context = separateContext(message);   
        
        switch(getType(message)){
            case SUBSCRIBE:
              printf("\n");
              printf("ALP Subscribe Message \n");
              printf("Selecting single nodes to forwadring ALP Publish messages from this nodes:");
              printf("\n");
             
              selectedServices = analyseOneSubscribe(selectedServices,context);
              
              showSelectedServices(selectedServices);
        
              break;
            case UNSUBSCRIBE:
              printf("\n");
              printf("ALP Unsubscribe Message \n");
              printf("Selecting single nodes to remove from forwarding list:");
              printf("\n");
                   
              selectedServices = analyseOneUnSubscribe(selectedServices,context);
              
              showSelectedServices(selectedServices);
              
              break;
            default:
              break;  
        }
        
        free(context);
    }
    
    free(message);
    
}

