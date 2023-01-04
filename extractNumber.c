#define DIRECOTRY_BUFFER_SIZE 128

#define TRUE 1
#define FALSE 0

#define isNumber(character) character >= '0' && character <= '9'

#define SEPARATOR (char)'/'
#define CR 13
#define LF 10


int getNumber(char* myConfig){

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