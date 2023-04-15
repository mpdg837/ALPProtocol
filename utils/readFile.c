#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_LINE_SIZE 128

#define TRUE 1
#define FALSE 0
char readFileBuffer[FILE_LINE_SIZE];
FILE* fptr;

int readMe(char* fileName)
{    
    
    fptr = fopen(fileName, "r");
    if (NULL == fptr) {
        printf("Error: File %s can not open \n",fileName);
        return -1;
    }
    
    return 1;
}

char* readLine(){
    char initized = FALSE;
    
    int k = 0;
    memset(readFileBuffer,'\0',sizeof(char) * FILE_LINE_SIZE);
    
    char chr;
    
    do {
        chr = fgetc(fptr);
        
        if(chr == EOF && !initized) return NULL;
        
        readFileBuffer[k] = chr;
        if( readFileBuffer[k] == 13 ||  readFileBuffer[k] == 10 || readFileBuffer[k]== '\n'){
            k++;
            break;
        }
        k++;
        
        initized = TRUE;
    } while (chr != EOF);
    

    
    char* answer = malloc(sizeof(char) * (k));
    int n=0;
    for(n=0;n<k;n++){
        answer[n] = readFileBuffer[n];
    }
    
    
    return answer;
}

void closeFile(){
    fclose(fptr);
}