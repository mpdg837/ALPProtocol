#include "./ALP/publishSubscriber.c"

void wait(int time){
    long long lastTime = current_timestamp();
    while(current_timestamp() - lastTime > time * 1000){
    }
}


void sendMessage(char* mess, int len){
    
    reWriteMessage(mess);   
    send_it(len);
    printf("sent it ! \n");
}

void tester(){
    int len = 0;
    
     // Message 1
    char* path = "/mieszkanie/salon/okno1";
    char* spath = encodePath(path);
    char* query = newQuery(spath);
    
    path = "/mieszkanie/salon/kuchenka";
    spath = encodePath(path); 
    query = addPathToQuery(query,spath,OR_OPERATION);
    query = addWildcard(query,ALL_SERVICES);
      
    char* message = makeSubscribeMessage(query);
    
    free(spath);
    
    len = packetLen(message);
    
    sendMessage(message,len);    
    free(message);
    
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
    
    
    wait(10);
    // Message4
    char* path3 = "/mieszkanie/salon/okno1/roleta";
    char* spath3 = encodePath(path3);
    char* query3 = newQuery(spath3);
    
    query3 = addWildcard(query3,COMMAND);
     
    char* message3 = makePublishMessage(query3);
      
    free(spath3);
    
    len = packetLen(message3) + 2;
    
    
    printf("length: %d \n",len);
    
    sendMessage(message3,len);
    
      
    for(int n=0;n<len;n++){
        printf("0x%x, ",(unsigned char)message3[n]);
    }
    
    
    free(message3);
    
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
void tester1(){
      // Message3
    int len = 0;
    
    char* path2 = "/budynek";
    char* spath2 = encodePath(path2);
    char* query2 = newQuery(spath2);
    
    query2 = addWildcard(query2,ALL_SERVICES);
     
    char* message2 = makeUnSubscribeMessage(query2);
      
    free(spath2);
    
    len = packetLen(message2);
    printf("length: %d \n",len);
    
    sendMessage(message2,len);
    free(message2);
    
    // Message4
    char* path3 = "/blok";
    char* spath3 = encodePath(path3);
    char* query3 = newQuery(spath3);
    
    query3 = addWildcard(query3,ALL_SERVICES);
     
    char* message3 = makeUnSubscribeMessage(query3);
      
    free(spath3);
    
    len = packetLen(message3);
    printf("length: %d \n",len);
    
    sendMessage(message3,len);
    free(message3);
}
int testConnection(void){

    if( init_listener() !=0) return 0;
  
    if(get_basic_info() != 0) return 0;
    if(connect_me() != 0) return 0;
  
  
    tester();
    while(1){
        int pos = receive_it();
    
        if(pos <0) return 0;
  
        if(pos > 0){
        
            if(isMessage(message)){
                switch(getType(message)){
                    case PUBLISH:
                        printf("\n \n \n");
                        show_time();
                        printf("MESSAGE ALP Publish Message  SOURCE IP: %s \n",ip_address);    
                        
                        analyseReceivedPublishBySubscriber(message);
                        break;
                    default: 
                        break;
                }
            }
            
        }
        
   }
   
   
  
    
    
}


int main(void){
    createListNodes();
    
    addWord("kategorie",0);
    addWord("mieszkanie",512);
    addWord("salon",513);
    addWord("okno1",514);
    addWord("kuchenka",515);
    addWord("lampa",516);
    addWord("sypialnia",517);
    addWord("osoby",518);
    addWord("Antek",519);
    addWord("Kajtek",520);
    addWord("roleta",521);
    
    
    mainTree = emptyBranch(32767);

   addNodePath("/mieszkanie/salon=0");
   addNodePath("/mieszkanie/salon/okno1=0");
   addNodePath("/mieszkanie/salon/okno1/roleta+0");
   addNodePath("/mieszkanie/salon/kuchenka=0");
   addNodePath("/mieszkanie/salon/lampa+0");
   addNodePath("/mieszkanie/sypialnia=0");
   addNodePath("/mieszkanie/sypialnia/lampa+0");
   addNodePath("/osoby/Antek=0");
   addNodePath("/osoby/Antek=0");
   addNodePath("/osoby/Antek/roleta+0");
   addNodePath("/osoby/Antek/lampa+0");
   addNodePath("/osoby/Kajtek=0");
   addNodePath("/osoby/Kajtek/lampa+0");

    printf("================================================ \n");
    printf(" Main Tree: \n");
    printf("------------------------------------------------ \n");
    
    printBranch(mainTree,0);
    
    printf("\n Init subscriber ... \n");
    
    testConnection();
    
    killServicesList(serviceSet);
    
    killSubscriberList();
    killNodesList();
    killSelectedItems();
    killDict();
}
