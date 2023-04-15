

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#define SEND_PORT "16747"
#define LISTEN_NUMBER "16746"
#define SEND_PORT_NUM 16747

#define NETWORK_ADAPTER "enp0s3"

#define MAX_BUF 128

fd_set readfds;
struct timeval tv;

	
   int en = 1;
   
static unsigned char message[MAX_BUF];
static unsigned char ip_address[15];


 struct addrinfo cb;
    struct addrinfo hb, *rb = NULL;
    int sb, c_lenb = sizeof(cb);
    
	int s;
	unsigned char mip_str[INET_ADDRSTRLEN];
	struct addrinfo h,c;
	struct addrinfo *r = NULL;
	struct addrinfo *p;

  int c_len = sizeof(c);

static unsigned char ip_address_recv[15];
static unsigned char ip_broadcast[15];

long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}

void show_time(void){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("[%d-%02d-%02d %02d:%02d:%02d] ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}
	
void close_connection(void){
	freeaddrinfo(r);
  
  show_time();
	close(s);	
 
  printf("Serwer zostal wylaczony poprawnie \n");
}

int error_message(char* mess){

  show_time();
  printf("%s \n",mess);
  close_connection();
  return 0;
}

int getBroadcastIP(){
 int fd;
 struct ifreq ifr;

 fd = socket(AF_INET, SOCK_DGRAM, 0);

 /* I want to get an IPv4 IP address */
 ifr.ifr_addr.sa_family = AF_INET;

 /* I want IP address attached to "eth0" */
 strncpy(ifr.ifr_name, NETWORK_ADAPTER, IFNAMSIZ-1);

 ioctl(fd, SIOCGIFADDR, &ifr);

 close(fd);

 /* display result */
 printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

 return 0;
}

int getMyIP(void){
	int fd;
	struct ifreq ifr;
	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	ifr.ifr_addr.sa_family = AF_INET;
	
	memcpy(ifr.ifr_name,"eth0",IFNAMSIZ - 1);
	ioctl(fd,SIOCGIFADDR, &ifr);

	close(fd);

	strcpy(ip_address,inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr) -> sin_addr));
	
	if(strcmp(ip_address,"0.0.0.0") == 0){
		return -1;
	}
	return 0;
}

int get_basic_info(void){
	memset(&h,0,sizeof(struct addrinfo));
	
	h.ai_family = PF_INET;
	h.ai_socktype = SOCK_DGRAM;
	h.ai_flags = AI_PASSIVE;

	if(getMyIP() == 0){
		printf("IP Serwera: %s \n",ip_address);
	}else{
		printf("Nie mozna wyznaczyc prawidlowego IP serwera \n");

		return -1;
	}
  return 0;
}

int connect_me(void){
  tv.tv_sec=0;
  tv.tv_usec=5000;
  
	if(getaddrinfo(NULL,LISTEN_NUMBER,&h,&r) != 0){
    error_message("Nie udalo siê pobraæ infomracji o gniezdzie");
		return -1;
	}
	
	s = socket(r -> ai_family, r-> ai_socktype, r-> ai_protocol);
	
	if(s == -1){
 
    error_message(" Nie udalo sie zarezerwowac gniazda \n");
		return -1;
	}
   

  show_time();
	printf(" (Operacja SOCKET) Ustanowiono gniazdo \n");
  
	if(bind(s, r->ai_addr, r->ai_addrlen)!= 0){
   
    error_message(" Nie mozna skojarzyc gniazda z aplikacj¹ serwera \n");
		return -1;
	}
 
   show_time();
   printf(" (Operacja BIND) Polaczenie gniazda z portem lokalnej maszyny \n");

 return 0;
}

int send_it(char* myMessage, char* ip_addr, int len){
  memset(&hb,0,sizeof(struct addrinfo));
    hb.ai_family = PF_INET;
    hb.ai_socktype = SOCK_DGRAM;
    
    if(getaddrinfo(ip_addr,SEND_PORT,&hb,&rb)!=0){
       error_message("Nie udalo sie pobrac infomracji o gniezdzie");
       
       return -1;
    }
    
    if((sb = socket(rb->ai_family, rb-> ai_socktype, rb->ai_protocol)) == -1){
       error_message(" Nie udalo siê zarezerwowaæ gniazda \n");
       
       return -1;
    }
    
    int en=1;
    if(setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &en, sizeof(en))==-1){
      error_message(" Nie udalo siê zarezerwowac gniazda w trybie rozsiewczym \n");
      
      return -1;
    }
    
   
   int pos = sendto(sb,myMessage,len,MSG_NOSIGNAL,rb->ai_addr, rb->ai_addrlen);
      
    if(pos < 0){
      error_message("Blad podczas wysylania wiadomosci");
      return -1;
    }
    
    
    freeaddrinfo(rb);
    close(sb);	
  return 0;
}

void save_source_ip(void){
      struct sockaddr_in* ip_addr = (struct sockaddr_in*)&c;
      char* ip = inet_ntoa(ip_addr -> sin_addr);
       
       int n=0;
       for(n=0;n<16;n++){
         ip_address_recv[n] = ip[n];
         if(ip[n] == '\0') break;
       }

}

int receive_it(void){
  FD_ZERO(&readfds);
  FD_SET(s, &readfds);
  int r;
  
  if((r=select(s+1, &readfds, NULL, NULL, &tv))<0){
    error_message("Blad podczas proby aktualizacji stanu serwera");
    return -1;
  }else if(r>0){
  
    if(FD_ISSET(s, &readfds)){
       int pos = 0;
      
       if((pos = recvfrom(s,message,MAX_BUF,0,(struct sockaddr*)&c, &c_len)) < 0){
        error_message("Blad podczas odbierania wiadomosci");
        return -1;
      }
      
      save_source_ip();
      message[pos]= '\0';
      
      return pos;
    }
  }
  return 0;
}