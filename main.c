#include "func.h"

int main(){
    Table *table = NULL;
    table = malloc(sizeof(Table));
    int rc;
    int (*fptr[])(Table *) = {NULL, D_Add};

    if (D_Load(table) == 0)
        return 1;

    while(rc = dialog(msgs, NMsgs)){
        if(!fptr[rc](table)){
            break;
        }
    }
    save(table);
    printf("That's all. Bye!\n");
    //del(table);
    return 0;
}

