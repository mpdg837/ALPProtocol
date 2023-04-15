#include "./packets/packets.c"
#include "./udp/udpSubscriber.c"

#define DATA_NODATA 0
#define DATA_COMMAND 1
#define DATA_SENSOR 2
#define DATA_METADATE 3

char initizedReceivedPath = FALSE;

struct receivedPublish {
    int dataType;
    char* dataPath;
    char dataService;
    char* dataValue;
    
    short convertedDataValue;
};

struct receivedPublish lastReceivedPublish;


void reWriteMessage(char* mess){
    int n=0;
    char finishing = FALSE;
    
    memset(message,'\0',sizeof(char) * MAX_BUF);
    
    char publisher = FALSE;
    int len = 0;
    for(n=0;n<MAX_BUF;n++){
        
        message[n] = mess[n];
        
        if(mess[n] & 0x1){
            if(finishing) {
                if(getType(mess) == PUBLISH){
                    publisher = TRUE;
                    break;
                }
                else break;
                
            }
            finishing = TRUE;
        }else finishing = FALSE;
        
    }
    
    if(publisher){
        n++;
        int len = mess[n];
        message[n] = mess[n];
        
        n++;
        for(int k=0;k<len;k++){
           message[n] = mess[n];
           n++;   
        }
    }
    
    
    
}

void showDataType(){
     printf("  Type: ");
     switch(valueLen){
         case DATA_COMMAND: 
             lastReceivedPublish.dataType = DATA_COMMAND;
             printf("Short command");
             break;
         case DATA_SENSOR:
             lastReceivedPublish.dataType = DATA_SENSOR;
             printf("Sensor value");
             break;
         default:
             lastReceivedPublish.dataType = DATA_METADATE;
             printf("Metadata");
             break;
    }
    
    printf("\n");
                        
}

void analyseReceivedPublishBySubscriber(char* message){

          lastReceivedPublish.dataPath = NULL;
          lastReceivedPublish.dataValue = NULL;
      
      lastReceivedPublish.convertedDataValue = 0;
      lastReceivedPublish.dataService = -1;
      lastReceivedPublish.dataType = 0;
      
      printf("Content Path: ");    
      if(lastReceivedPublish.dataPath != NULL) free(lastReceivedPublish.dataPath);
      lastReceivedPublish.dataPath = separateContext(message);
      printShortPath(lastReceivedPublish.dataPath);
                        
      printf("\n");
      printf("Service:");
                        
      lastReceivedPublish.dataService = separateService(lastReceivedPublish.dataPath);
      printService(lastReceivedPublish.dataService);
                        
      printf("\n");
      
      if(lastReceivedPublish.dataValue != NULL) free(lastReceivedPublish.dataValue);                  
      lastReceivedPublish.dataValue = separateValue(lastReceivedPublish.dataPath);
      printf("Message: \n");
                        
      showDataType();
      lastReceivedPublish.dataType = valueLen;              
      printf("\n");
      
      double temperature =0;
      double humitidy = 0;
       double info =0;
       double lightness =0;
       double pressure = 0;
      switch(valueLen){
            case 0:
                printf("No data \n");
                break;
            case DATA_SENSOR:
               lastReceivedPublish.convertedDataValue = ((unsigned char)lastReceivedPublish.dataValue[0] << 8) | (unsigned char)lastReceivedPublish.dataValue[1];
               
                printf("%d \n", lastReceivedPublish.convertedDataValue);
               
               break;
            default:
               for(int k=0;k<valueLen;k++){
                    printf("%c",lastReceivedPublish.dataValue[k]);
               }
               
               printf("\n");
               break;
      }
     
      printf("\n");
}