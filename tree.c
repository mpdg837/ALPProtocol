#include "./branch.c"

#define DIRECOTRY_BUFFER_SIZE 128

#define isNumber(character) character >= '0' && character <= '9'

#define TRUE 1;
#define FALSE 0;

#define SEPARATOR (char)'/'
#define CR 13
#define LF 10




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
                    if (!readNumber) {
                        char *destination = malloc(sizeof(char) * strlen(pathBuffer));
                        strcpy(destination, pathBuffer);

                        goNextBranch(destination);
                        memset(pathBuffer, '\0', sizeof(char) * DIRECOTRY_BUFFER_SIZE);
                        k = 0;
                    } else {
                        addNode(nodeIndex);
                        finloop = TRUE;
                    }

                    if (myConfig[n] == '=') {
                        readNumber = TRUE;
                    }
                    else if (myConfig[n] != SEPARATOR) break;
                }
                break;
            default:

                if(init) {
                    if (!readNumber) pathBuffer[k] = myConfig[n];
                    else if (isNumber(myConfig[n])) {

                        int number = (int)(unsigned char)myConfig[n];
                        const int zero = (int)(unsigned char)'0';

                        nodeIndex *=(int)10;
                        nodeIndex += number - zero;
                    }

                    k++;
                }
                break;
        }

        if(finloop) break;
    }

}
