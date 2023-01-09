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

void showSelectedItems(){
    lnode* listElement = selectedItems;

    printf("Selected items: \n");
    
    while (listElement != NULL)
    {
        printf("* NODE ID: %d \n",listElement -> myNode ->numberNode);
        listElement = listElement -> next_item;
    }
    
}