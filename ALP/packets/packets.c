#include "./subscribers.c"
#include <stdlib.h>

#define ALP_IDENTITY 0x15

#define SUBSCRIBE 0x1
#define UNSUBSCRIBE 0x2
#define PUBLISH 0x3

#define HELLO 0x4
#define ISAVAILABLE 0x5
#define AVAILABLE 0x6
#define BROADCAST 0x7

#define VALUE_ERROR -1

static int valueLen = 0;


int packetLen(char* message){
    int len = 1;
    int n;
    
    char finishing = FALSE;
    
    for(n = 0;n < QUERY_LEN ; n++){
      
        if(message[n] & 0x1){
            if(finishing){
              break;
            }
            finishing = TRUE;
        }else finishing = FALSE;
        
        len++;
    }
    
    return len;
}

char isMessage(char* message){
    char identity = (message[0] >> 3) & 0x1F;
    
    if(identity == ALP_IDENTITY) return TRUE;
    
    return FALSE;
}

char getType(char* message){
    return message[0] & 0x7;
}

char* addHeader(char topic,char* restOfMessage){

    char* newMessage = malloc(sizeof(char)*QUERY_LEN);
    memset(newMessage,'\0',sizeof(char)*QUERY_LEN);
    
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
    
    
    char* newMessage = addHeader( (ALP_IDENTITY << 3) | SUBSCRIBE,context);
    
    free(context);
    return newMessage;
}

char* makeUnSubscribeMessage(char* context){

    char* newMessage = addHeader((ALP_IDENTITY << 3) | UNSUBSCRIBE,context);
    
    free(context);
    return newMessage;
}

char* initPublishMessage(){
    char* packetMessage = malloc(sizeof(char)*QUERY_LEN);
    memset(packetMessage,0,sizeof(char)*QUERY_LEN);
                
    char caption = (ALP_IDENTITY << 3) | PUBLISH;
    packetMessage[0] = caption;
    
    return packetMessage;
}


char* makePublishMessage(char* context){
    char* newMessage = addHeader( (ALP_IDENTITY << 3) | PUBLISH,context);
  
    int n=0;
    char finishing = FALSE;
    
    for(n=0;n<QUERY_LEN;n++){
        
        if(newMessage[n] & 0x1){
            if(finishing) break;
            else finishing = TRUE;
        }else finishing = FALSE;
    }
    
    newMessage[n+1] = 0x1;
    newMessage[n+2] = 0x5;
    
    free(context);
    return newMessage;
}

char separateService(char* context){
    char finishing = FALSE;
   
    int n;
    for(n = 0; n < QUERY_LEN; n++){
        char znak = context[n];
        
        if(znak & 0x1){
            if(finishing) return (znak >> 1) & 0xF;
            else finishing = TRUE;
        }else finishing = FALSE;
        
    }
    
    return SERVICE_ERROR;
}

char* separateValue(char* context){
    
    valueLen = VALUE_ERROR;
    
    char finishing = FALSE;
    char detected = FALSE;
     
    int n;
    
    for(n = 0; n < QUERY_LEN; n++){
        char znak = context[n];
        
        if(znak & 0x1){
            if(finishing) {
                detected = TRUE;
                break;    
            }
            else finishing = TRUE;
        }else finishing = FALSE;
        
    }
    
    n++;
    if(!detected) return NULL;
    
   
    int len = context[n];
    int k = 0;
    
    char* value = malloc(sizeof(char) * len);
    memset(value,0,sizeof(char) * len);
    
    for(k=0;k<len;k++){
        value[k] = context[n+k + 1]; 
    }
    
    valueLen = len;
        
    return value;
}


char* separatePath(char* message){
    char* context = malloc(sizeof(char)*QUERY_LEN);
    memset(context,0,sizeof(char)*QUERY_LEN);
    
    int n = 0;
    
    char finishing = FALSE;
    char detected = FALSE;
    
    char nextNextAnalyse = FALSE;
    char nextAnalyse = FALSE;
    
    unsigned char len = 0;
    int k = 0;
    
    for(n = 0; n < QUERY_LEN; n++){
        
        context[n] = message[n];
                
        if(context[n] & 0x1){
            detected = TRUE;
            break;
        }       

    };
    
    if(detected) return context;
    else {
        free(context);
        return NULL;
    }
    
}


char* separateContext(char* message){
    char* context = malloc(sizeof(char)*QUERY_LEN);
    memset(context,0,sizeof(char)*QUERY_LEN);
    
    int n = 0;
    
    char finishing = FALSE;
    char detected = FALSE;
    char final = FALSE;
    
    char nextNextAnalyse = FALSE;
    char nextAnalyse = FALSE;
    
    unsigned char len = 0;
    int k = 0;
    
    for(n = 0; n < QUERY_LEN; n++){
        
        context[n] = message[n+1];
        
        switch(getType(message)){
            case PUBLISH: 
            
                if(!nextAnalyse){
                    if(context[n] & 0x1){
                        if(finishing){
                          nextAnalyse = TRUE;
                        }
                        finishing = TRUE;
                    }else finishing = FALSE;           
                }else{
                  
                    if(!nextNextAnalyse){
                        len = context[n];
                    }else{
                        k++;
                        
                        if(k>=len){
                            detected = TRUE;
                            final = TRUE;
                        }
                    }
                    
                    nextNextAnalyse = TRUE;
                }
                
                break;
            case HELLO:
                
                if(n == 4) break;
                detected = TRUE;
                
                break;
            default:;
                 if(context[n] & 0x1){
                    if(finishing){
                      detected = TRUE;
                      final = TRUE;
                      break;
                    }
                    finishing = TRUE;
                }else finishing = FALSE;           
                break;
        }
        

        if(final) break;
    };
    
    if(detected) return context;
    else {
        free(context);
        return NULL;
    }
    
}
