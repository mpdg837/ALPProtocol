#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define DIRECOTRY_BUFFER_SIZE 128
#define WORD_BUFFER_SIZE 32

#define DICT_SEPARATOR '='

typedef struct word{
    short id;
    char* value;
    
    struct word* next_word;
}word;

word* mainDictionary = NULL;

static word* createWord(char* value, short number){

    if(number == 0) number = 0x7FFF;
    if(number == 1) number = 0x7FFE;
    
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

void addWordByFile(char* line){
    short identity = 0;
    char* value = malloc(sizeof(char)*WORD_BUFFER_SIZE);
    memset(value,'\0',sizeof(char)*WORD_BUFFER_SIZE);
    
    if(line != NULL){
        int n=0;
        
        char mode = FALSE;
        
        for(n=0;n<strlen(line);n++){
            char znak = line[n];
            
            switch(znak){
                case DICT_SEPARATOR:
                    mode = TRUE;
                    break;
                default:
                    if(!mode){
                        if(n >= WORD_BUFFER_SIZE) return (void)0;
                        value[n] = znak;
                    }else{
                    
                        if(znak >= '0' && znak <= '9'){
                            identity *=10;
                            identity += znak - '0';
                        }
                        
                    }
                    break;
            }    
        }
        if(mode) addWord(value, identity);
    }
    

    free(value);
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
    memset(shortPath,'\0',sizeof(char) * DIRECOTRY_BUFFER_SIZE);

    if(strcmp(myConfig,"/") == 0){
        shortPath[0] = 0x0;
        shortPath[1] = 0x1;    
    }else{


        char pathBuffer[DIRECOTRY_BUFFER_SIZE];
        memset(pathBuffer,'\0',sizeof(char) * DIRECOTRY_BUFFER_SIZE);

        int nPath = 0;
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
                        
                        shortPath[nPath] = (id >> 7) & 0xFF;
                        shortPath[nPath + 1] = ((id & 0xFF) << 1);

                        nPath += 2;
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
        shortPath[nPath] = (id >> 7) & 0xFF;
        shortPath[nPath+1] = ((id & 0xFF) << 1) | 0x1;
        
    }
    return shortPath;
}
