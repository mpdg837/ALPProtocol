#include <string.h>
#include <stdlib.h>

#define BUFFER_LEN 128

typedef struct word{
    short id;
    char* value;
    
    struct word* next_word;
}word;

word* mainDictionary = NULL;

static word* createWord(char* value, short number){

    word* newWord = malloc(sizeof(word*));
    char* newValue = malloc(sizeof(char) * BUFFER_LEN);

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