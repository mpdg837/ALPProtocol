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

                analysedItem -> next_item = newItem;
                break;
            }
            analysedItem = analysedItem -> next_item;
        }
    }
}

lnode* andItem(lnode* set1, lnode* set2){

    char detected = FALSE;

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

lnode* sensorFilter(lnode* set, char temperature, char pressure, char humitidy, char lightness){

    lnode* analysedItem = set;
    char detected = FALSE;

    while(1){

        if(analysedItem -> myNode != NULL){
            char decision = FALSE;

            if(temperature && analysedItem -> myNode ->temperature) decision = TRUE;
            if(pressure && analysedItem -> myNode ->pressure) decision = TRUE;
            if(humitidy && analysedItem -> myNode ->humitidy) decision = TRUE;
            if(lightness && analysedItem -> myNode ->lightness) decision = TRUE;

            if(decision){
                addOperElement(analysedItem -> myNode,analysedItem -> path);
                detected = TRUE;
            } 
        }

        if(analysedItem -> next_item == NULL) break;
        analysedItem = analysedItem -> next_item;
    }

    lnode* newRes = NULL;

    if(set != NULL) killCopiedList(set);

    if(detected){
        newRes = copySelectedItems(results);
        killCopiedList(results);
    }

    return newRes;
}

lnode* orItem(lnode* set1, lnode* set2){

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