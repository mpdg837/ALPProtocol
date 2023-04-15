#include "./testing.c"

int testConnection(void){

    if( init_listener() !=0) return 0;
  
    if(get_basic_info() != 0) return 0;
    if(connect_me() != 0) return 0;
  
  
    test3();
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
