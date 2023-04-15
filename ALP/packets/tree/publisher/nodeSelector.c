#include "./node.c"
#include "./utils/dictionary.c"

static lnode* selectedItems = NULL;
static char firstSelection = TRUE;


void printShortPath(char* path){
    int n=0;
        for(n=0;n<DIRECOTRY_BUFFER_SIZE;n+=2){
            
            short number = (((unsigned char) path[n]) << 7) | ((((unsigned char) path[n+1]) & 0xFE) >> 1);
            char* value = getValue(number);

            printf("%s",value);

            if(path[n+1] & 0x1 == 1) break;
            else printf("/");
        }
}

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

static void addSelectedItem(node* myNode,char* path, int ip){

    lnode* analysedItem = selectedItems;
    char detected = FALSE;

    if(analysedItem == NULL){
        selectedItems = newNodeItem(myNode);
        copyPath(selectedItems -> path,path);
        selectedItems -> ipSubscriber = ip;
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
            analysedItem -> ipSubscriber = ip;
        }
    }

    
}

lnode* copySelectedItems(lnode* selected){
    lnode* copySelected = NULL;
    lnode* lastElement = NULL;

    if(selected != NULL){
        lnode* analysedItem = selected;

        while(1){
            
            lnode* newItem = newNodeItem(analysedItem -> myNode);
            copyPath(newItem -> path,analysedItem -> path);
            
            newItem -> ipSubscriber = analysedItem -> ipSubscriber;
            
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

            if(savedItem -> path != NULL) free(savedItem ->path);

            if(analysedItem -> next_item == NULL) break;
            analysedItem = analysedItem -> next_item;

            free(savedItem);
        }
    
}


void showSelectedItems(lnode* list){
    lnode* listElement = list;
    
    printf("------------------------------------------------- \n");
    
    while (listElement != NULL)
    {
        printf("* NODE ID  : %d in path: /",listElement -> myNode ->numberNode);   
        printShortPath(listElement -> path);  
        printf("\n");

        listElement = listElement -> next_item;
    }
    printf("------------------------------------------------- \n");
}


