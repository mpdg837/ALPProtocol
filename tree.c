#include "./branch.c"

#define DIRECOTRY_BUFFER_SIZE 128

#define isNumber(character) character >= '0' && character <= '9'

#define SEPARATOR (char)'/'
#define CR 13
#define LF 10

int getNumber(char* myConfig){
    char pathBuffer[DIRECOTRY_BUFFER_SIZE];

    memset(pathBuffer,'\0',sizeof (char));

    char finAnalyse = FALSE;
    char canRead = FALSE;

    int bufferNumber = 0;

    int n=0;
    for(n=0;n<DIRECOTRY_BUFFER_SIZE;n++){
        switch (myConfig[n]) {
            case CR:
            case LF:
            case '\0':
                finAnalyse = TRUE;
                break;
            case '=':
                canRead = TRUE;
                break;
            default:
                if(canRead){
                    if(isNumber(myConfig[n])){
                        bufferNumber *= 10;
                        bufferNumber += (unsigned char)myConfig[n] - (unsigned char)'0';
                    }
                }
                break;
        }

        if(finAnalyse) break;
    }

    return bufferNumber;
}
int detectNewNode(char* myConfig){
    int number = getNumber(myConfig);
    if(!addNewNode(number)) {
        printf("NEW NODE: %d \n",number);
    }
    return number;
}

void addNodePath(char* myConfig){

    char pathBuffer[DIRECOTRY_BUFFER_SIZE];

    memset(pathBuffer,'\0',sizeof (char));

    int k=0;
    char readNumber = FALSE;

    actBranch = mainTree;
    char init = FALSE;

    int finloop = FALSE;
    int nodeIndex = 0;

    int n=0;

    int numberNode = detectNewNode(myConfig);

    for(n=0;n<DIRECOTRY_BUFFER_SIZE;n++){
        switch (myConfig[n]) {
            case SEPARATOR:
            case CR:
            case LF:
            case '\0':
            case '=':
                if(myConfig[n] == SEPARATOR && !init && n==0){
                    init = TRUE;
                }else if(init){
                    
                    char *destination = malloc(sizeof(char) * strlen(pathBuffer));
                    strcpy(destination, pathBuffer);

                    goNextBranch(destination);
                    memset(pathBuffer, '\0', sizeof(char) * DIRECOTRY_BUFFER_SIZE);
                    k = 0;

                    if (myConfig[n] == '=') {
                        addNode(numberNode);
                        finloop = TRUE;
                    }
                }
                break;
            default:

                if(init) {
                    pathBuffer[k] = myConfig[n];
                    k++;
                }
                break;
        }

        if(finloop) break;
    }

}
