#include "./node.c"

static lnode* selectedItems;
static char firstSelection = TRUE;

void initNewSelectionItems(){
    if(!firstSelection){
        killSelectedItems();
    }
    selectedItems = NULL;
}

void killSelectedItems(){
    lnode* analysedItem = selectedItems;
    while (selectedItems != NULL)
    {
        lnode* savedItem = selectedItems;
        selectedItems = selectedItems-> next_item;

        free(savedItem);
    }
    
}

static void addSelectedItem(node* myNode){

    lnode* analysedItem = selectedItems;

    if(analysedItem == NULL){
        selectedItems = newNodeItem(myNode);
    }else{

        while(1){
            if(analysedItem -> next_item == NULL) break;
            analysedItem = analysedItem -> next_item;
        };

        lnode* newMyItem = newNodeItem(myNode);
        analysedItem -> next_item = newMyItem;
    }
}

