#include <string.h>
#include <stdlib.h>

#define IP_ERROR_CODE -1
#define IP_SEPARATOR '.'
#define IPV4_SIZE 256
#define IPV4_SECTORS 4

#define IP_SIZE 15

#define TRUE 1
#define FALSE 0

char decodedIP[IP_SIZE];

int addToIpSector(int answer, unsigned char numberBuf){
                  
    if(numberBuf >= IPV4_SIZE){
        return IP_ERROR_CODE;
    }else{
        answer = (answer << 8) | numberBuf;
    }
                   
    return answer;
}

int encodeIPtoInt(char* ip){
    
    int answer = 0;
    
    int n=0;
    unsigned char numberBuf = 0;
    unsigned char sectors = 0;
        
    for(n=0;n<strlen(ip);n++){
        char znak = ip[n];
        
        switch(znak){
            case IP_SEPARATOR: 
                  
                  answer = addToIpSector(answer,numberBuf);
                  sectors ++;
                  
                  if(sectors >= IPV4_SECTORS) return -1;
                  
                  numberBuf = 0;
                  break;
            default:
                  if(znak >= '0' && znak <= '9'){
                      numberBuf *= 10;
                      numberBuf += znak - '0';                  
                  }else{
                      return IP_ERROR_CODE;
                  }
                  break;   
        }
    }
    
    answer = addToIpSector(answer,numberBuf);
    
    
    
    return answer;
    
}

char* decodeIPtoCharacters(int ip){
    
    memset(decodedIP,'\0',sizeof(char) * 12);
    
    int n = 0;
    int ip_b = ip;
    
    int ip_index = 0;
    
    unsigned char sector[IPV4_SECTORS];
    memset(sector,0,sizeof(char) * IPV4_SECTORS);
    
    for(n = 0; n < IPV4_SECTORS ; n++){
        sector[n] = ip_b & 0xFF;
        
        ip_b = ip_b >> 8;
    }
    
    snprintf(decodedIP,IP_SIZE,"%d.%d.%d.%d" ,sector[3],sector[2],sector[1],sector[0]);
    
    return decodedIP;        
}