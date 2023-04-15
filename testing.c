#include "./ALP/publishSubscriber.c"
#define LIGHT_ON 0x1
#define LIGHT_OFF 0x2

#define TURN_ON_LIGHT_ACTIVITY    0x1
#define TURN_OFF_LIGHT_ACTIVITY   0x2
#define TURN_ON_ENGINE_OF_BLINDS  0x3
#define TURN_OFF_ENGINE_OF_BLINDS 0x4
#define BLINDS_UP                 0x5
#define BLINDS_DOWN               0x6


void wait(int time){
    long long lastTime = current_timestamp();
    while(current_timestamp() - lastTime <= time * 1000){
    }
}


void sendMessage(char* mess, int len){
    
    reWriteMessage(mess);   
    send_it(len);
    printf("sent it ! \n");
}

void test0(){
    int len = 0;
    
    // Test1
    
     // Message 1
    char* path = "/mieszkanie/salon/okno1";
    char* spath = encodePath(path);
    char* query = newQuery(spath);
    
    query = addWildcard(query,LIGHTNESS);
      
    char* message = makeSubscribeMessage(query);
    
    free(spath);


    len = packetLen(message);
    
    sendMessage(message,len);    
    free(message);
}
void test1(){
    int len = 0;  
    // Test2
    // Message2
    char* path1 = "/osoby/Kajtek";
    char* spath1 = encodePath(path1);
    char* query1 = newQuery(spath1);
    
    path1 = "/osoby/Antek";
    spath1 = encodePath(path1); 
    query1 = addPathToQuery(query1,spath1,OR_OPERATION);
    
    query1 = addWildcard(query1,ALL_SERVICES);
     
    char* message1 = makeSubscribeMessage(query1);
    
    free(spath1);
    
    len = packetLen(message1);
    
    sendMessage(message1,len);    
    free(message1);
}
void test2(){
    int len = 0;
    // Test3
    
    // Message 3
    char* path = "/osoby/Kajtek";
    char* spath = encodePath(path);
    char* query = newQuery(spath);
    
    query = addWildcard(query,ALL_SERVICES);
      
    char* message = makeUnSubscribeMessage(query);
    
    free(spath);
    
    len = packetLen(message);
    
    sendMessage(message,len);    
    free(message);   
}
void test3(){
    int len = 0;
    // Test4
    
    // Message4
    char* path3 = "/mieszkanie/salon/okno1/roleta";
    char* spath3 = encodePath(path3);
    char* query3 = newQuery(spath3);
    
    query3 = addWildcard(query3,COMMAND);
     
    char* message3 = makePublishMessage(query3);
      
    free(spath3);
    
    len = packetLen(message3) + 2;
    message3[len - 1] = TURN_ON_ENGINE_OF_BLINDS;
    
    printf("length: %d \n",len);
    
    sendMessage(message3,len);
    
    free(message3);

    // Message4
    char* path4 = "/mieszkanie/salon/okno1/roleta";
    char* spath4 = encodePath(path4);
    char* query4 = newQuery(spath4);
    
    query4 = addWildcard(query4,COMMAND);
     
    char* message4 = makePublishMessage(query4);
      
    free(spath4);
    
    len = packetLen(message4) + 2;
    message4[len - 1] = BLINDS_UP;
    
    printf("length: %d \n",len);
    
    sendMessage(message4,len);
    
    free(message4);
        
}

void unSubscribeAll(void){

    int len = 0;
    // Message3
    
    char* path2 = "/";
    char* spath2 = encodePath(path2);
    char* query2 = newQuery(spath2);
    
    query2 = addWildcard(query2,COMMAND);
     
    char* message2 = makeUnSubscribeMessage(query2);
      
    free(spath2);
    
    len = packetLen(message2);
    
    sendMessage(message2,len);
    free(message2);
    
}

void test4(){
    int len = 0;
    // Message4
    char* path4 = "/mieszkanie/salon/lampa";
    char* spath4 = encodePath(path4);
    char* query4 = newQuery(spath4);
    
    query4 = addWildcard(query4,COMMAND);
     
    char* message4 = makePublishMessage(query4);
      
    free(spath4);
    
    len = packetLen(message4) + 2;
    message4[len - 1] =TURN_ON_LIGHT_ACTIVITY;
    
    printf("length: %d \n",len);
    
    sendMessage(message4,len);
    
    free(message4);
    
    wait(5);
    
     // Message4
    char* path5 = "/mieszkanie/salon/lampa";
    char* spath5 = encodePath(path5);
    char* query5 = newQuery(spath5);
    
    query5 = addWildcard(query5,COMMAND);
     
    char* message5 = makePublishMessage(query5);
      
    free(spath5);
    
    len = packetLen(message5) + 5;
    message5[len - 1] =TURN_ON_LIGHT_ACTIVITY;
    
    printf("length: %d \n",len);
    
    sendMessage(message5,len);
    
    free(message5);   
        
}