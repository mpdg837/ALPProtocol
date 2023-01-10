#include "./node.c"
#include "./dictionary.c"

static lnode* selectedItems = NULL;
static char firstSelection = TRUE;


void killSelectedItems(void){
    lnode* analysedItem = selectedItems;
    while (selectedItems != NULL)
    {
        lnode* savedItem = selectedItems;

        free(savedItem -> path);
        selectedItems = selectedItems-> next_item;

        free(savedItem);
    }
    
}


void initNewSelectionItems(void){
    if(!firstSelection){
        killSelectedItems();
    }
    selectedItems = NULL;
    firstSelection = TRUE;
}

static void addSelectedItem(node* myNode,char* path){

    lnode* analysedItem = selectedItems;
    char detected = FALSE;

    if(analysedItem == NULL){
        selectedItems = newNodeItem(myNode);
        copyPath(selectedItems -> path,path);
    }else{

        while(1){

            if(analysedItem -> myNode != NULL){
                if(analysedItem -> myNode == myNode){
                    return (void)0;
                }
            }
            if(analysedItem ->next_item == NULL) break;
            analysedItem = analysedItem -> next_item;
        };

        if(!detected){
            lnode* newMyItem = newNodeItem(myNode);
            copyPath(newMyItem -> path,path);
            analysedItem -> next_item = newMyItem;
        }
    }

    
}

lnode* copySelectedItems(void){
    lnode* copySelected = NULL;
    lnode* lastElement = NULL;

    if(selectedItems != NULL){
        lnode* analysedItem = selectedItems;

        while(1){
            
            lnode* newItem = newNodeItem(analysedItem -> myNode);
            copyPath(newItem -> path,analysedItem -> path);

            if(copySelected == NULL){
                copySelected = newItem;
                lastElement = copySelected;
            }else{
                lastElement -> next_item = newItem;
                lastElement = newItem;
            }

            if(analysedItem -> next_item == NULL) break;
            analysedItem = analysedItem -> next_item;
        }
    }

    return copySelected;
}

void killCopiedList(lnode* list){
    lnode* analysedItem = list;

    if(list != NULL)
        while (1)
        {
            lnode* savedItem = analysedItem;

            free(savedItem ->path);

            if(analysedItem -> next_item == NULL) break;
            analysedItem = analysedItem -> next_item;

            free(savedItem);
        }
    
}

void printShortPath(char* path){
    int n=0;
        for(n=0;n<DIRECOTRY_BUFFER_SIZE;n+=2){
            
            short number = (((unsigned char) path[n]) << 8) | ((((unsigned char) path[n+1]) & 0xFE) >> 1);
            char* value = getValue(number);

            printf("%s",value);

            if(path[n+1] & 0x1 == 1) break;
            else printf("/");
        }
}
void showSelectedItems(lnode* list){
    lnode* listElement = list;

    printf("Selected items: \n");
    
    while (listElement != NULL)
    {
        printf("* NODE ID  : %d in path: /",listElement -> myNode ->numberNode);   
        printShortPath(listElement -> path);  
        printf("\n");

        listElement = listElement -> next_item;
    }
    
}

void copyPath(char* to, char* from){
    int k=0;

    memset(to,'\0',sizeof(char) * DIRECOTRY_BUFFER_SIZE);

    for(k=0;k<DIRECOTRY_BUFFER_SIZE;k+=2){
        to[k] = from[k];
        to[k+1] = from[k+1];

        if(from[k+1] & 0x1 == 1) break;       
    }
}

char* addBranchToPath(char* path, short id){
    
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