#include <string.h>
#include <stdlib.h>

#define DIRECOTRY_BUFFER_SIZE 128

typedef struct word{
    short id;
    char* value;
    
    struct word* next_word;
}word;

word* mainDictionary = NULL;

static word* createWord(char* value, short number){

    word* newWord = malloc(sizeof(word*));
    char* newValue = malloc(sizeof(char) * DIRECOTRY_BUFFER_SIZE);

    strcpy(newValue,value);

    newWord -> id = number;
    newWord -> value =  newValue;
    newWord -> next_word = NULL;

    return newWord;
}
int addWord(char* value, short number){
        // Prawidlowa wartosc
        word* newWord = createWord(value,number);

        if(mainDictionary == NULL){
            mainDictionary = newWord;
        }else{

            word* analysedWord = mainDictionary;

            while(1){
                if(analysedWord -> next_word == NULL) break; 
                analysedWord = analysedWord -> next_word;
            }

            analysedWord -> next_word = newWord;
        }            

        return 0;
  
}

short encode(char* value){
    word* analysedWord = mainDictionary;

    if(analysedWord != NULL)
        while(1){
           
            if(strcmp(analysedWord -> value , value) == 0){
                return analysedWord -> id;
            }

            if(analysedWord -> next_word == NULL) break;
            analysedWord = analysedWord -> next_word;
        }

    return -1;
}

void killDict(void){
    word* analysedWord = mainDictionary;
    while(analysedWord != NULL){

        word* savedWord = analysedWord;

        analysedWord = analysedWord -> next_word;
        free(savedWord);
    }

}

char* getValue(short id){

    word* analysedWord = mainDictionary;

    while (analysedWord != NULL)
    {
        if(analysedWord -> id == id){
            return analysedWord -> value;
        }

        analysedWord = analysedWord -> next_word;
    }

    return 0;
}

char* encodePath(char* myConfig){
    char* shortPath=malloc(sizeof(char) * DIRECOTRY_BUFFER_SIZE);
    
    char pathBuffer[DIRECOTRY_BUFFER_SIZE];
    memset(pathBuffer,'\0',sizeof(char) * DIRECOTRY_BUFFER_SIZE);

    int n=0;
    int k=0;
    char initized = FALSE;

    for(n=0;n<strlen(myConfig);n++){
        
        char znak = myConfig[n];
        switch (znak)
        {
            case '/':
                if(!initized){
                    initized = TRUE;
                }else{
                    pathBuffer[k] = '\0';

                    short id = encode(pathBuffer);
                    printf("KOLEJNA: %d \n",id);

                    memset(pathBuffer,'\0',sizeof(char) * DIRECOTRY_BUFFER_SIZE);
                    k=0;
                }
                break;
            default:
                pathBuffer[k] = znak;
                k++;
                break;
        }
    }
    
    pathBuffer[k] = '\0';

    short id = encode(pathBuffer);
    printf("KOLEJNA: %d \n",id);


    return shortPath;
}
