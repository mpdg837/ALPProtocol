#include "./nodeSelector.c"

lnode* results = NULL;
char collectAllDesision = FALSE;

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
                newItem -> next_item = NULL;
                
                analysedItem -> next_item = newItem;
                
                break;
            }
            analysedItem = analysedItem -> next_item;
        }
    }
}

lnode* andItem(lnode* set1, lnode* set2){

    char detected = FALSE;
    results = NULL;
    
    if(set1 != NULL){
        lnode* analysedItem = set1;
        while (1)
        {
            if(analysedItem ->myNode != NULL){
                if(contains(set2,analysedItem -> myNode ->numberNode)){
                    detected = TRUE;
                    addOperElement(analysedItem -> myNode,analysedItem ->path);
                }
            }

            if(analysedItem -> next_item == NULL) break;
            analysedItem = analysedItem -> next_item;
        }
        
    }

    lnode* newRes = NULL;

    if(set1 != NULL) killCopiedList(set1);
    if(set2 != NULL) killCopiedList(set2);

    if(detected){
        newRes = copySelectedItems(results);

        killCopiedList(results);
    }
    return newRes;
}

void addAllItems(lnode* set){
    lnode* analysedItem = set;
    
    while(1){

        if(analysedItem -> myNode != NULL){

            if(!contains(results,analysedItem -> myNode ->numberNode)){
                collectAllDesision = TRUE;
                addOperElement(analysedItem -> myNode,analysedItem -> path);
            }
        }

        if(analysedItem -> next_item == NULL) break;
        analysedItem = analysedItem -> next_item;            
    }
}


lnode* orItem(lnode* set1, lnode* set2){
    results = NULL;
 
    collectAllDesision = FALSE;

    if(set1 != NULL) addAllItems(set1);
    if(set2 != NULL) addAllItems(set2);

    lnode* newRes = NULL;
    
    if(collectAllDesision){
        newRes = copySelectedItems(results);
    }

    if(set1 != NULL) killCopiedList(set1);
    if(set2 != NULL) killCopiedList(set2);
    if(collectAllDesision) killCopiedList(results);

    return newRes;
}