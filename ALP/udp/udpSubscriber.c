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

#define SERVER_IP "192.168.0.227"

#define LISTEN_PORT "16747"
#define SEND_PORT "16746"

#define NETWORK_ADAPTER "enp0s3"

#define TIME_DELAY 960
#define MAX_BUF 512


fd_set readfds;
struct timeval tv;

static unsigned char message[MAX_BUF];
static unsigned char ip_address[15];
static unsigned char current_time[MAX_BUF];

  struct addrinfo cb; // LISTENER
  struct addrinfo hb, *rb = NULL;
  int sb, c_lenb = sizeof(cb);
    
	int s; // SENDER
	unsigned char mip_str[INET_ADDRSTRLEN];
	struct addrinfo h,c;
	struct addrinfo *r = NULL;
	struct addrinfo *p;
  int c_len = sizeof(c);
  
  static int protocol_addr = -1;


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

void buffer_time(void){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(current_time,MAX_BUF,"%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}
	
void close_connection(void){
	freeaddrinfo(r);
  
  show_time();
  printf("(Operacja CLOSE) Zamkniecie gniazda klienta \n");
	close(s);
 
    freeaddrinfo(rb);
    close(sb);	
  printf("Klient zostal wylaczony poprawnie \n");
}

int error_message(char* mess){

  show_time();
  printf("%s \n",mess);
  close_connection();
  
  return 0;
}
    
int init_listener(){

   memset(&hb,0,sizeof(struct addrinfo));
    hb.ai_family = PF_INET;
    hb.ai_socktype = SOCK_DGRAM;
    hb.ai_flags = AI_PASSIVE;
    
    if(getaddrinfo(NULL,LISTEN_PORT,&hb,&rb)!=0){
      error_message("Nie udalo siê pobraæ infomracji o gnieŸdzie");
      
      return -1;
    }
    
    if((sb = socket(rb->ai_family, rb-> ai_socktype, rb->ai_protocol)) == -1){
       error_message(" Nie udalo siê zarezerwowaæ gniazda \n");
      
      return -1;
    }
    
    if(bind(sb, rb->ai_addr, rb->ai_addrlen)!= 0){
   
      error_message(" Nie mozna skojarzyc gniazda z aplikacj¹ serwera \n");
   
  		return -1;
  	}
   
    int en=1;
    if(setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &en, sizeof(en))==-1){
       error_message(" Nie udalo siê zarezerwowac gniazda w trybie rozsiewczym \n");
      return -1;
    }
    
    show_time();
    printf("[SOCKET] Ustanowiono gniazdo nasluchowe \n");
    return 0;
    
}


int getMyIP(void){
	int fd;
	struct ifreq ifr;
	
	fd = socket(AF_INET,SOCK_DGRAM,0);
	ifr.ifr_addr.sa_family = AF_INET;
	
	memcpy(ifr.ifr_name,NETWORK_ADAPTER,IFNAMSIZ - 1);
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

	if(getMyIP() == 0){
		printf("IP: %s \n",ip_address);
	}else{
		printf("Nie mozna wyznaczyc prawid³owego IP \n");

		return -1;
	}
  return 0;
}

int connect_me(void){

  if(getaddrinfo(SERVER_IP,SEND_PORT,&h,&r) != 0){
    error_message("Nie udalo siê pobrac infomracji o gniezdzie");
		return -1;
	}
  
	s = socket(r -> ai_family, r-> ai_socktype, r-> ai_protocol);
	
	if(s == -1){
 
    error_message(" Nie udalo siê zarezerwowac gniazda \n");
		return -1;
	
  }
  
  int en = 1;
   if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en))==-1){
      printf("ERROR: %s\n", strerror(errno));
    }
  show_time();
	printf(" (Operacja SOCKET) Ustanowiono gniazdo \n");
	
 return 0;
}

void make_message(void){
  buffer_time();
  snprintf(message,MAX_BUF,"");
}

int send_it(int len){
    
  int pos = sendto(s,message,len,MSG_NOSIGNAL,r->ai_addr, r-> ai_addrlen);
  if(pos < 0){
    error_message("B³¹d podczas wysy³ania wiadomoœci");
    return -1;
  }
  return 0;
}

int en = 1;

int receive_it(void){
  
  FD_ZERO(&readfds);
  FD_SET(sb, &readfds);
  int r;

  
  if((r=select(sb+1, &readfds, NULL, NULL, &tv))<0){
    error_message("B³¹d podczas próby aktualizacji stanu serwera");
    return -1;
  }else{
  
    if(FD_ISSET(sb, &readfds)){
      
      int pos;
      
       if((pos = recvfrom(sb,message,MAX_BUF,0,(struct sockaddr*)&cb, &c_lenb)) < 0){
        error_message("B³¹d podczas odbierania wiadomoœci");
        return -1;
      }
      
      message[pos]= '\0';
      
      return pos;
    }
  }
  return 0;
}