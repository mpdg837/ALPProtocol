
#include "./subscribe.c"
#define SERVER_IP "192.168.116.58"

#define DIAGNOSTIC_TIMEOUT (long long)(10 * 1000)

int countSubscribeMessages = 0;
int countUnSubscribeMessages = 0;
int countHelloMessages = 0;
int countPublishMessages = 0;
int countAllMessages = 0;

long long lastTime = 0;
long long sumOfLen = 0;


void diagnosticMonitor(void){
    printf("\n \n \n");
    show_time();
    
    printf("ALP Protocol Diagnostic Raport: \n");
    printf("\n");
    printf("All received messages : %d ",countAllMessages);
    printf(" + SUBSCRIBE %d \n",countSubscribeMessages);
    printf(" + UNSUBSCRIBE %d \n",countUnSubscribeMessages);
    printf(" + HELLO %d \n",countHelloMessages);
    printf(" + PUBLISH %d \n",countPublishMessages);
    printf("\n");
    printf("All messages send to next node: \n");
    printf(" + SUBSCRIBE %d \n",countSSubscribeMessages);
    printf(" + PUBLISH %d \n",countSPublishMessages);
    
    
    printf(" All bytes received : %llu \n", sumOfLen);  
    
    if(countAllMessages > 0){
        double average = ((double)sumOfLen / (double)countAllMessages);
    
        printf(" Average size : %f \n", average);
        printf("\n");
    }
}
void receiveMessage(char* message,int len){
    if(isMessage(message)){
        
  
        int ip = encodeIPtoInt(ip_address_recv);
        char* context = separateContext(message);   
        
        sumOfLen += (long long) len;
        countAllMessages ++;
        
        switch(getType(message)){
            case SUBSCRIBE:
              printf("\n \n \n");
              show_time();
              printf("MESSAGE: ALP Subscribe Message. SOURCE IP %s \n",ip_address_recv);
              printf("Selecting single nodes to forwadring ALP Publish messages from this nodes:");
              printf("\n");
             
              subscribe(ip,context);
              countSubscribeMessages++;
              break;
            case UNSUBSCRIBE:
              printf("\n \n \n");
              show_time();
              printf("MESSAGE ALP Unsubscribe Message  SOURCE IP: %s \n",ip_address_recv);
              printf("Selecting single nodes to remove from forwarding list:");
              printf("\n");
                   
              unSubscribe(ip,context);
              countUnSubscribeMessages ++;
              break;
            case HELLO:
              printf("\n \n \n");
              show_time();
              printf("MESSAGE ALP Hello Message  SOURCE IP: %s \n",ip_address_recv);
              
              helloNode(ip,context);
              countHelloMessages++;
              break;
            case PUBLISH:
              printf("\n \n \n");
              show_time();
              printf("MESSAGE ALP Publish Message  SOURCE IP: %s \n",ip_address_recv);
              analysePublish(ip,context);
                          
              countPublishMessages ++;
              break;
            default:
              break;  
        }
        
        free(context);
    }
    
    
}

int listener(void){

    lastTime = current_timestamp();
    
    if(get_basic_info() != 0) return 0;
    if(connect_me() != 0) return 0;
    
    char sentit = FALSE;
   
    while(1){
        int pos = receive_it();
    
        if(pos <0) return 0;
  
        if(pos > 0){
            
            receiveMessage(message,pos);
            memset(message,'\0',sizeof(char) * QUERY_LEN);
        }
        
        if(current_timestamp() - lastTime >= DIAGNOSTIC_TIMEOUT){
            
            diagnosticMonitor();
                             
            lastTime = current_timestamp();
        }
        
        
   }
   
   return 0;
}
