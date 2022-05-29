#include "func.h"

const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow"};

int find(Table ptab, int k){
    int i = 0, s = 0, j = ptab.n - 1;
    printf("++");
    while (i<=j){
        s = (i+j)/2;
        if (ptab.first[s].key == k)
        return s;
        if (ptab.first[s].key < k)
        i = s + 1;
        if (ptab.first[s].key > k)
        j = s - 1;
    }
    printf("++");
    return -1;
}

int find_add(Table *ptab, int k){
    int i = 0, s = 0, j = ptab->n - 1;

    while (i<=j){
        s = (i+j)/2;
        if (ptab->first[s].key == k)
        return -1;
        if (ptab->first[s].key < k)
        i = s + 1;
        if (ptab->first[s].key > k)
        j = s - 1;
    }
    return i;
}

int create(Table *ptab, char *fname, int sz){
    ptab->SZ = sz;
    ptab->n = 0;

    ptab->first = (Item *)calloc(ptab->SZ, sizeof(Item));
    fwrite(&ptab->SZ, sizeof(int), 1, ptab->fd);
    fwrite(&ptab->n, sizeof(int), 1, ptab->fd);
    fwrite(ptab->first, sizeof(Item), ptab->SZ, ptab->fd);

    return 1;
}

int D_Load(Table *ptab){
	int SZ;
	char *fname = NULL;
	fname = readline("Enter file name: ");

	if(fname == NULL)
	return 0;

	if (load(ptab, fname) == 0){
		printf("Enter possible vector size: -->");
		if (getInt(&SZ) == 0)
		return 0;
		create(ptab, fname, SZ);
	}
	free(fname);
	return 1;
}

int save(Table *ptab){
    fseek(ptab->fd, sizeof(int), SEEK_SET);
    fwrite(&ptab->n, sizeof(int), 1, ptab->fd);
    fwrite(ptab->first, sizeof(Item), ptab->n, ptab->fd);
    fclose(ptab->fd);
    ptab->fd = NULL;
    return 1;
}

int load(Table *ptab, char *fname){

    fopen(fname, "r+b");
    if (ptab->fd == NULL)
    return 0;

    fread(&ptab->n, sizeof(int), 1, ptab->fd);
    ptab->first = (Item *)calloc(ptab->n, sizeof(Item));
    fread(&ptab->n, sizeof(int), 1, ptab->fd);
    fread(ptab->first, sizeof(Item), ptab->n, ptab->fd);

    return 1;
}

int insert(Table *ptab, int k, char *info){
    printf("++");
    int r = find_add(ptab, k);

    if(r == -1)
    return 1;

    if (ptab->n == 0 || (r == (ptab->n))){
	ptab->first[ptab->n].len = strlen(info)+1;
        ptab->first[r].key = k;
        fseek(ptab->fd, 0, SEEK_END);
	ptab->first[ptab->n].offset = ftell(ptab->fd);
	fwrite(info, sizeof(char), ptab->first[ptab->n].len, ptab->fd);
    }else{
        int i = r, j = ptab->n-1;
        for(j = ptab->n-1; j >= i; j--){
	    ptab->first[j+1].len = strlen(info)+1;
            ptab->first[j+1].key = ptab->first[j].key;
            fseek(ptab->fd, 0, SEEK_END);
	    ptab->first[j+1].offset = ftell(ptab->fd);
            fwrite(info, sizeof(char), ptab->first[j+1].len, ptab->fd);
        }
	ptab->first[r].len = strlen(info)+1;
        ptab->first[r].key = k;
	fseek(ptab->fd, 0, SEEK_END);
        ptab->first[r].offset = ftell(ptab->fd);
        fwrite(info, sizeof(char), ptab->first[r].len, ptab->fd);

    }
    ++(ptab->n);
    return 0;
}

int getInt(int *a){
    int e;

    do{
        e = scanf("%d", a);
        if (e < 0){
            return 0;
        }
        if (e == 0){
            printf("%s\n", "Error! Repeat input");
            scanf("%*c");
        }
    } while (e == 0);
    return 1;
}

/*int D_Show(Table *ptab){
    int i;
    int j = (ptab->n);

    for (i = 0; i < j; i++)
    printf("key: %d | name: %s\n", ptab->first[i].key, ptab->first[i].name);
    return 1;
}

int D_Find(Table *ptab){
    int k, c, res;

    printf("Enter key: \n");
    c = getInt(&k);
    if(c == 0)
    return 0;

    res = find(*ptab, k);
    if (res >= 0)
    printf("key = %d, name = %s\n", k, ptab->first[res].name);
    else
    printf("Item %d was not found\n", k);
    return 1;
}

int D_Del(Table *ptab){
    int k, c, res;

    printf("Enter key: \n");
    c = getInt(&k);
    if(c == 0)
    return 0;

    res = find(*ptab, k);
    if (res >= 0){
        int i, j = (ptab->n)-1 ;
        for(i = res; i < j; i++){
	    free(ptab->first[i].name);
	    ptab->first[i].name = (char*) malloc(sizeof(char)*(strlen(ptab->first[i+1].name) + 1));
            ptab->first[i].key = ptab->first[i+1].key;
            *strcpy(ptab->first[i].name, ptab->first[i+1].name);
        }
	free(ptab->first[i].name);
        printf("Item %d was successfully deleted\n", k);
        --(ptab->n);
    }else{
        printf("Item %d was not found\n", k);
    }
    return 1;
}*/

int hash(char *info){
    int s = 0, i = 0, res = 0, j = strlen(info);

    for(i = 0; i < j; i++){
    	s = s + info[i];
    }
    res = s % 60;
    return res;
}

int D_Add(Table *ptab){
    int k, rc;
    char *info = NULL;
    info = readline("Enter name: ");
    printf("ee");
    k = hash(info);
    rc = insert(ptab, k, info);

    free(info);
    printf("%s: %d\n", errmsgs[rc], k);
    return 1;
}

/*void del(Table *ptab){
    int i , j = ptab->n;

    for (i = 0; i < j; i++)
    free(ptab->first[i].name);
    free(ptab->first);
    free(ptab);
}*/

int dialog(const char *msgs[], int N){
    char *errmsg = "";
    int rc;
    int i, n;

    for(i = 0; i < N; ++i)
    puts(msgs[i]);

    do{
        puts(errmsg);
        errmsg = "You are wrong. Repeate, please!";
        puts("Make your choice: ");
        n = getInt(&rc);
        if(n == 0)
        rc = 0;
    } while(rc < 0 || rc >= N);

    return rc;
}
