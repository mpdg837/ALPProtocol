#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define DIRECOTRY_BUFFER_SIZE 128

char* newShortPath(){
    char* npath = malloc(sizeof(char)*DIRECOTRY_BUFFER_SIZE);
    npath [0] = 0;
    npath [1] = 0x1;

    return npath;
}


char shortPathComparer(char* path1, char* path2){
    int n=0;
    

    for(n=0;n<DIRECOTRY_BUFFER_SIZE;n+=2){
    
        if(n+1 < DIRECOTRY_BUFFER_SIZE){
          if(path1[n] != path2[n] || path1[n+1] != path2[n+1]) return FALSE;
          
          if(path1[n+1] & 0x1) break;
        }else{
          return FALSE;
        }
    }
    
    return TRUE;
}

static void copyPath(char* to, char* from){
    int k=0;

    memset(to,'\0',sizeof(char) * DIRECOTRY_BUFFER_SIZE);

    for(k=0;k<DIRECOTRY_BUFFER_SIZE;k+=2){
        to[k] = from[k];
        to[k+1] = from[k+1];

        if(from[k+1] & 0x1 == 1) break;       
    }
}

static char* addBranchToPath(char* path, short id){
    
    int k = 0;

    if(path[0] != 0x0 || path[1] != 0x1){

        for(k=0;k<DIRECOTRY_BUFFER_SIZE;k+=2){
            if(path[k+1] & 0x1 == 1) {
                path[k+1] = path[k+1] & 0xFE;
                break;
            }       
        }

        k += 2;
    }
    
    path[k] = (id >> 8) & 0xFF;
    path[k+1] = ((id & 0xFF) << 1) | 0x1;
    
}