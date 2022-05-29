#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

const char *msgs[5];
const int NMsgs ;
const char *errmsgs[3] ;

typedef struct Item{
    int key;
    int offset;
    int len;
}Item;

typedef struct Table{
    int n;
    int SZ;
    FILE *fd;
    Item *first;
}Table;

int load (Table *ptab, char *fname);
int create(Table *ptab, char *fname, int sz);
int D_Load(Table *ptab);
int save(Table *ptab);
void del(Table *ptab);
int hash(char *info);
int find(Table ptab, int k);
int find_add(Table *ptab, int k);
int insert(Table *ptab, int k, char info[]);
int getInt(int *a);
int D_Del(Table *ptab);
int D_Add(Table *ptab);
int D_Find(Table *ptab);
int D_Show(Table *ptab);
int dialog(const char *msgs[], int N);
