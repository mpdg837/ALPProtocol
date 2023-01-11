#include "./nodeSelector.c"

lnode* results = NULL;

char contains(lnode* set, short id){

    lnode* analysedItem = set;

    if(set != NULL)
        while(1){
            
            if(analysedItem -> myNode != NULL){
                if(analysedItem ->myNode ->numberNode == id) return TRUE;
            }            

            if(analysedItem -> next_item == NULL) break;
            analysedItem = analysedItem ->next_item;
        }

    return FALSE;
}

static void addOperElement(node* element,char* path){
    
    if(results == NULL){  
        results = newNodeItem(element);
        copyPath(results -> path,path);

    }else{
        lnode* analysedItem = results;

        while(1){
            
            if(analysedItem -> next_item == NULL){
                lnode* newItem = newNodeItem(element);
                copyPath(newItem -> path,path);

                analysedItem -> next_item = newItem;
                break;
            }
            analysedItem = analysedItem -> next_item;
        }
    }
}

lnode* andItem(lnode* set1, lnode* set2){

    
    if(set1 != NULL){
        lnode* analysedItem = set1;
        while (1)
        {
            if(analysedItem ->myNode != NULL){
                if(contains(set2,analysedItem -> myNode ->numberNode)){
                    addOperElement(analysedItem -> myNode,analysedItem ->path);
                }
            }

            if(analysedItem -> next_item == NULL) break;
            analysedItem = analysedItem -> next_item;
        }
        
    }

    killCopiedList(set1);
    killCopiedList(set2);

    return results;
}

void addAllItems(lnode* set){
    lnode* analysedItem = set;
    
    while(1){

        if(analysedItem -> myNode != NULL){

            if(!contains(results,analysedItem -> myNode ->numberNode)){
                addOperElement(analysedItem -> myNode,analysedItem -> path);
            }
        }

        if(analysedItem -> next_item == NULL) break;
        analysedItem = analysedItem -> next_item;            
    }
}
lnode* orItem(lnode* set1, lnode* set2){


    if(set1 != NULL) addAllItems(set1);
    if(set2 != NULL) addAllItems(set2);

    lnode* newRes = copySelectedItems(results);
    killCopiedList(set1);
    killCopiedList(set2);

    return newRes;
}