#include "./branch.c"
#include "./extractNumber.c"

#define DIRECOTRY_BUFFER_SIZE 128

#define isNumber(character) character >= '0' && character <= '9'

#define SEPARATOR (char)'/'
#define CR 13
#define LF 10

int detectNewNode(char* myConfig){
    int number = getNumber(myConfig);
    if(!addNewNode(number)) {
        printf("NEW NODE: %d \n",number);
    }
    return number;
}

tree* findBranch(tree* analysedTree,char* topic){
    tree* analysedBranch = analysedTree;

    while (analysedBranch != NULL)
    {

        if(analysedBranch -> subbranch != NULL)
            if(strcmp(analysedBranch -> name_of_branch,topic) == 0){
                return analysedBranch -> subbranch;
                break;
            }

    
        analysedBranch = analysedBranch -> next_branch;
    }
    return NULL;
}

shortcut* findNodes(tree* analysedTree,char* topic){
    tree* analysedBranch = analysedTree;

    while (analysedBranch != NULL)
    {

        if(analysedBranch -> nodes != NULL)
            if(strcmp(analysedBranch -> name_of_branch,topic) == 0){
                return analysedBranch -> nodes;
                break;
            }

    
        analysedBranch = analysedBranch -> next_branch;
    }
    return NULL;
}

tree* getBranch(char* myConfig){
    tree* analyseTree = mainTree -> subbranch;
    shortcut* analyseNode = NULL;

    char pathBuffer[DIRECOTRY_BUFFER_SIZE];

    memset(pathBuffer,'\0',sizeof (char));

    char finAnalyse = FALSE;
    int bufferNumber = 0;

    int k=0;
    int n=0;
    if(strlen(myConfig) > 1){
        for(n=1;n<DIRECOTRY_BUFFER_SIZE;n++){
            switch (myConfig[n]) {
                case CR:
                case LF:
                case '\0':
                case '/':
                    
                    pathBuffer[k] = '\0';
                     char *destination = malloc(sizeof(char) * strlen(pathBuffer));
                     strcpy(destination, pathBuffer);

                     analyseNode = findNodes(analyseTree,destination);
                     analyseTree = findBranch(analyseTree,destination);

                 
                     if(myConfig[n] != '/') finAnalyse = TRUE;
                     else {
                        memset(pathBuffer,'\0',sizeof (char));
                        k=0;
                     }
                    break;
                default:
                    pathBuffer[k] = myConfig[n];
                    k++;
                    break;
            }

            if(finAnalyse) break;
        }
    }
    
    char *destination = malloc(sizeof(char) * strlen(pathBuffer));
    strcpy(destination, pathBuffer);

    tree* selectedBranch = emptyBranch(destination);
    selectedBranch -> subbranch = analyseTree;
    selectedBranch -> nodes = analyseNode;

    return selectedBranch;
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
                    
                     char* destination = malloc(sizeof(char) * DIRECOTRY_BUFFER_SIZE);                    
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
