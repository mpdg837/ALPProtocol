#include "./node.c"

static lnode* selectedItems = NULL;
static char firstSelection = TRUE;


void killSelectedItems(){
    lnode* analysedItem = selectedItems;
    while (selectedItems != NULL)
    {
        lnode* savedItem = selectedItems;
        selectedItems = selectedItems-> next_item;

        free(savedItem);
    }
    
}


void initNewSelectionItems(){
    if(!firstSelection){
        killSelectedItems();
    }
    selectedItems = NULL;
    firstSelection = TRUE;
}

static void addSelectedItem(node* myNode){

    lnode* analysedItem = selectedItems;
    char detected = FALSE;

    if(analysedItem == NULL){
        selectedItems = newNodeItem(myNode);
    }else{

        while(1){

            if(analysedItem -> myNode != NULL){
                if(analysedItem -> myNode == myNode){
                    return 0;
                }
            }
            if(analysedItem ->next_item == NULL) break;
            analysedItem = analysedItem -> next_item;
        };

        if(!detected){
            lnode* newMyItem = newNodeItem(myNode);
            analysedItem -> next_item = newMyItem;
        }
    }

    
}

lnode* copySelectedItems(){
    lnode* copySelected = NULL;
    lnode* lastElement = NULL;

    if(selectedItems != NULL){
        lnode* analysedItem = selectedItems;

        while(1){
            
            lnode* newItem = newNodeItem(analysedItem -> myNode);
            
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

    while (1)
    {
        lnode* savedItem = analysedItem;

        if(analysedItem -> next_item == NULL) break;
        analysedItem = analysedItem -> next_item;

        free(savedItem);
    }
    
}


void showSelectedItems(lnode* list){
    lnode* listElement = list;

    printf("Selected items: \n");
    
    while (listElement != NULL)
    {
        printf("* NODE ID: %d \n",listElement -> myNode ->numberNode);
        listElement = listElement -> next_item;
    }
    
}