#include<stdio.h>
#include<stdlib.h>

typedef struct Item *pntItem;
typedef struct Item
{
    int number;
    char* name;
    int price;
}Item;

typedef struct AVLTree *pntAVL;
typedef struct AVLTree
{
    pntAVL left;
    pntAVL right;
    pntAVL parent;
    pntItem item;
    int height;
    int key;
    int Rank;
    pntItem MaxPriceItem;
}AVL;

typedef struct Series *pntSeries;
typedef struct Series
{
    pntAVL items;
    int number;
}Series;

typedef struct AVLSeries *pntAVLSeries;
typedef struct AVLSeries
{
    pntAVLSeries left;
    pntAVLSeries right;
    pntAVLSeries parent;
    pntSeries series;
    int height;
    int key;
    pntSeries MaxSeries;
}AVLSeries;


typedef struct Store *pntStore;
typedef struct Store
{
    pntAVLSeries AVLSerieses;
    pntSeries *SeriesesArr;
    int NextItemNumber;
    int size;
    int Capacity;
}Store;


/*AVLSeries*/
pntAVLSeries insertAVLSeries(int key,pntSeries series,pntAVLSeries t,pntAVLSeries parent);
pntAVLSeries deleteAVLSeries(int key, pntAVLSeries t);
pntAVLSeries findAVLSeries(pntAVLSeries t, int key);
pntAVLSeries minAVLSeries(pntAVLSeries t);
int heightAVLSeries(pntAVLSeries t);
pntAVLSeries BalanceAVLSeries(pntAVLSeries t);
pntAVLSeries LeftRotateAVLSeries(pntAVLSeries t);
pntAVLSeries RightRotateAVLSeries(pntAVLSeries t);
void freeAVLSeries(pntAVLSeries t);
void UpdateMaxSeries(pntAVLSeries t);
void UpdateMaxSeriesTree(int serNum,pntAVLSeries t);

/* AVL*/
pntAVL insertAVL(int key,pntItem item,pntAVL t,pntAVL parent);
pntAVL deleteAVL(int key, pntAVL t,int flag);
pntAVL findAVL(pntAVL t, int key);
pntAVL minAVL(pntAVL t);
void freeAVLNode(pntAVL t);
int heightAVL(pntAVL t);
int RankAVL(pntAVL t);
pntAVL BalanceAVL (pntAVL t);
pntAVL LeftRotateAVL(pntAVL t);
pntAVL RightRotateAVL(pntAVL t);
void freeAVL(pntAVL t);
void UpdateMaxPrice(pntAVL t);
void SplitAVL(pntAVL *small,pntAVL *big,pntAVL *x,int key,pntAVL t);
pntAVL concatenateAVL(pntAVL small,pntAVL big,pntAVL x);
pntAVL ConcatenateRight(pntAVL small,pntAVL big,pntAVL x);
pntAVL ConcatenateLeft(pntAVL small,pntAVL big,pntAVL x);
/*Q3 functions*/

pntStore init();
void insert(int price,char*name,int i,pntStore s);
void delete(int number,int i,pntStore s);
void split_series(int i,int j,pntStore s);
Item Public_Max(pntStore s);
Item Priv_Max(pntStore s,int i);

/*auxilary functions...*/
void freeStore(pntStore s);/*To Do*/
int maxNum(int x, int y);
pntItem CreateItem(int price,char*name,int number);
pntAVL CalcSplitNode(pntAVL root, int j);

/*AVL function Implementation*/

void UpdateMaxPrice(pntAVL t)
{
    if(t->left==NULL&&t->right==NULL)t->MaxPriceItem=t->item;
    else if(t->left==NULL && t->right!=NULL)t->MaxPriceItem=t->item->price > t->right->MaxPriceItem->price ? t->item: t->right->MaxPriceItem;
    else if(t->right==NULL && t->left!=NULL)t->MaxPriceItem=t->item->price > t->left->MaxPriceItem->price ? t->item: t->left->MaxPriceItem;
    else
    {
        t->MaxPriceItem=t->right->MaxPriceItem->price > t->left->MaxPriceItem->price ? t->right->MaxPriceItem: t->left->MaxPriceItem;
        t->MaxPriceItem=t->item->price > t->MaxPriceItem->price ? t->item:t->MaxPriceItem;
    }
}

pntAVL insertAVL(int key,pntItem item,pntAVL t,pntAVL parent)
{
    if(t==NULL)
    {
        pntAVL node=(pntAVL)malloc(sizeof(AVL));
        node->key=key;
        node->height=0;
        node->Rank=1;
        node->left=node->right=NULL;
        node->MaxPriceItem=node->item=item;
        node->parent=parent;
        return node;
    }
    else
    {
        if(key>t->key)t->right=insertAVL(key,item,t->right,t);
        else if (key<t->key)t->left=insertAVL(key,item,t->left,t);
        t->height=maxNum(heightAVL(t->left),heightAVL(t->right))+1;
        t->Rank=RankAVL(t->left)+RankAVL(t->right)+1;
        t= BalanceAVL(t);
        UpdateMaxPrice(t);
        return t;
    }
}

pntAVL deleteAVL(int key, pntAVL t, int flag)
{

    pntAVL min, temp1,temp2;
    int tempKey;
    pntItem tempItem;
    /* Search for the node to be deleted*/
    if (t == NULL)
        return t;

    if (key < t->key)
        t->left = deleteAVL(key, t->left,flag);
    else if (key > t->key)
        t->right = deleteAVL(key, t->right,flag);
    else
    {
        if (t->left == NULL || t->right == NULL)
        {
            temp1 = t->left ? t->left : t->right;
            if (temp1 == NULL)
            {
                temp1=t;
                t=NULL;
            }
            else
            {
                if (t->parent != NULL)
                {
                    if (t == t->parent->left)
                        t->parent->left = temp1;
                    else
                        t->parent->right = temp1;
                }
                temp2=t;
                t=temp1;
                temp1=temp2;
            }
          if(flag) freeAVLNode(temp1);
          else free(temp1);
        }
        else
        {
            /* Node has two children, find inorder successor*/
            min = minAVL(t->right);
            tempKey=min->key;
            tempItem=min->item;
            /*char*name = min->item->name;*/
            t->right = deleteAVL(min->key, t->right,0);
            t->key=tempKey;
            t->item=tempItem;
            /*t->item->name=name;*/
        }
    }

    if (t == NULL)
        return t;
    t->height = 1 + maxNum(heightAVL(t->left), heightAVL(t->right));
    t->Rank=RankAVL(t->left)+RankAVL(t->right)+1;
    UpdateMaxPrice(t);
    t = BalanceAVL(t);
    return t;
}

void freeAVL(pntAVL t)
{
    if(t==NULL)return;
    freeAVL(t->left);
    freeAVL(t->right);
    freeAVLNode(t);
}

void freeAVLNode(pntAVL t)
{
    if(t!=NULL)
    {
        free(t->item);
        free(t);
    }
}

pntAVL findAVL(pntAVL t,int key) /*O(logn)*/
{
    if(t->key==key)return t ;
    if(t->key>key)
    {
        if(t->left==NULL)return t;
        else return findAVL(t->left,key);
    }
    if(t->right==NULL)return t;
    else return findAVL(t->right,key);
}

int heightAVL(pntAVL t) {
    if (t == NULL)
        return -1;
    return t->height;
}

int RankAVL(pntAVL t) {
    if (t == NULL)
        return 0;
    return t->Rank;
}

pntAVL minAVL(pntAVL t)
{
    if(t->left==NULL)return t;
    return minAVL(t->left);
}

pntAVL RightRotate(pntAVL t)
{
    pntAVL y = t->left;
    t->left = y->right;
    if (y->right != NULL) {
        y->right->parent = t;
    }
    y->parent = t->parent;
    if (t->parent != NULL) {
        if (t == t->parent->right) {
            t->parent->right = y;
        } else {
            t->parent->left = y;
        }
    }
    y->right = t;
    t->parent = y;
    t->Rank=RankAVL(t->left)+RankAVL(t->right)+1;
    y->Rank=RankAVL(y->left)+RankAVL(y->right)+1;
    UpdateMaxPrice(t);
    UpdateMaxPrice(y);
    t->height = maxNum(heightAVL(t->left), heightAVL(t->right)) + 1;
    y->height = maxNum(heightAVL(y->left), heightAVL(y->right)) + 1;

    return y;
}

pntAVL LeftRotate(pntAVL t)
{
    pntAVL y = t->right;
    t->right = y->left;
    if (y->left != NULL) {
        y->left->parent = t;
    }
    y->parent = t->parent;
    if (t->parent != NULL) {
        if (t == t->parent->left) {
            t->parent->left = y;
        } else {
            t->parent->right = y;
        }
    }
    y->left = t;
    t->parent = y;
    t->Rank=RankAVL(t->left)+RankAVL(t->right)+1;
    y->Rank=RankAVL(y->left)+RankAVL(y->right)+1;
    UpdateMaxPrice(t);
    UpdateMaxPrice(y);
    t->height = maxNum(heightAVL(t->left), heightAVL(t->right)) + 1;
    y->height = maxNum(heightAVL(y->left), heightAVL(y->right)) + 1;

    return y;
}

pntAVL BalanceAVL(pntAVL t)
{
    int balanceFactor = heightAVL(t->left) - heightAVL(t->right);


    if (balanceFactor >= -1 && balanceFactor <= 1) {
        return t;
    }


    if (balanceFactor > 1) {

        if (heightAVL(t->left->right) > heightAVL(t->left->left)) {
            t->left = LeftRotate(t->left);
        }

        t = RightRotate(t);
    }

    else {

        if (heightAVL(t->right->left) > heightAVL(t->right->right)) {
            t->right = RightRotate(t->right);
        }

        t = LeftRotate(t);
    }

    return t;
}

/*AVLSeries function Implementation*/
void UpdateMaxSeries(pntAVLSeries t)
{
    if(t->left==NULL&&t->right==NULL)t->MaxSeries=t->series;
    else if(t->left==NULL && t->right!=NULL)t->MaxSeries=t->series->items->MaxPriceItem->price > t->right->MaxSeries->items->MaxPriceItem->price ? t->series: t->right->MaxSeries;
    else if(t->right==NULL && t->left!=NULL)t->MaxSeries=t->series->items->MaxPriceItem->price > t->left->MaxSeries->items->MaxPriceItem->price ? t->series: t->left->MaxSeries;
    else
    {
        t->MaxSeries=t->right->MaxSeries->items->MaxPriceItem->price > t->left->MaxSeries->items->MaxPriceItem->price ? t->right->MaxSeries: t->left->MaxSeries;
        t->MaxSeries=t->MaxSeries->items->MaxPriceItem->price > t->series->items->MaxPriceItem->price ? t->MaxSeries:t->series;
    }

}

pntAVLSeries insertAVLSeries(int key,pntSeries series,pntAVLSeries t,pntAVLSeries parent)
{
    if(t==NULL)
    {
        pntAVLSeries node=(pntAVLSeries)malloc(sizeof(AVLSeries));
        node->key=key;
        node->height=0;
        node->left=node->right=NULL;
        node->MaxSeries=node->series=series;
        node->parent=parent;
        return node;
    }
    else
    {
        if(key>t->key)t->right=insertAVLSeries(key,series,t->right,t);
        else if (key<t->key)t->left=insertAVLSeries(key,series,t->left,t);
        t->height=maxNum(heightAVLSeries(t->left),heightAVLSeries(t->right))+1;
        t= BalanceAVLSeries(t);
        UpdateMaxSeries(t);
        return t;
    }
}

pntAVLSeries deleteAVLSeries(int key, pntAVLSeries t)
{
    pntAVLSeries min, temp1,temp2;
    int tempKey;
    pntSeries tempSeries;
    /* Search for the node to be deleted*/
    if (t == NULL)
        return t;
    if (key < t->key)
        t->left = deleteAVLSeries(key, t->left);
    else if (key > t->key)
        t->right = deleteAVLSeries(key, t->right);
    else
    {
        if (t->left == NULL || t->right == NULL)
        {
            temp1 = t->left ? t->left : t->right;
            if (temp1 == NULL)
            {
                temp1=t;
                t=NULL;
            }
            else
            {
                if (t->parent != NULL)
                {
                    if (t == t->parent->left)
                        t->parent->left = temp1;
                    else
                        t->parent->right = temp1;
                }
                temp2=t;
                t=temp1;
                temp1=temp2;
            }

           free(temp1);
        }
        else
        {
            /* Node has two children, find inorder successor*/
            min = minAVLSeries(t->right);
            tempKey=min->key;
            tempSeries=min->series;
            t->right = deleteAVLSeries(min->key, t->right);
                t->key=tempKey;
                t->series=tempSeries;
        }
    }

    if (t == NULL)
        return t;
    t->height = 1 + maxNum(heightAVLSeries(t->left), heightAVLSeries(t->right));
    UpdateMaxSeries(t);
    t = BalanceAVLSeries(t);
    return t;
}

void freeAVLSeries(pntAVLSeries t)
{
    if(t==NULL)return;
    freeAVLSeries(t->left);
    freeAVLSeries(t->right);
    free(t);
}

pntAVLSeries findAVLSeries(pntAVLSeries t, int key) /*O(logn)*/
{
    if(t->key==key)return t ;
    if(t->key>key)
    {
        if(t->left==NULL)return t;
        else return findAVLSeries(t->left,key);
    }
    if(t->right==NULL)return t;
    else return findAVLSeries(t->right,key);
}

int heightAVLSeries(pntAVLSeries t) {
    if (t == NULL)
        return -1;
    return t->height;
}

pntAVLSeries minAVLSeries(pntAVLSeries t)
{
    if(t->left==NULL)return t;
    return minAVLSeries(t->left);
}

pntAVLSeries RightRotateAVLSeries(pntAVLSeries t)
{
    pntAVLSeries y = t->left;
    t->left = y->right;
    if (y->right != NULL) {
        y->right->parent = t;
    }
    y->parent = t->parent;
    if (t->parent != NULL) {
        if (t == t->parent->right) {
            t->parent->right = y;
        } else {
            t->parent->left = y;
        }
    }
    y->right = t;
    t->parent = y;
    UpdateMaxSeries(t);
    UpdateMaxSeries(y);
    t->height = maxNum(heightAVLSeries(t->left), heightAVLSeries(t->right)) + 1;
    y->height = maxNum(heightAVLSeries(y->left), heightAVLSeries(y->right)) + 1;

    return y;
}

pntAVLSeries LeftRotateAVLSeries(pntAVLSeries t)
{
    pntAVLSeries y = t->right;
    t->right = y->left;
    if (y->left != NULL) {
        y->left->parent = t;
    }
    y->parent = t->parent;
    if (t->parent != NULL) {
        if (t == t->parent->left) {
            t->parent->left = y;
        } else {
            t->parent->right = y;
        }
    }
    y->left = t;
    t->parent = y;
    UpdateMaxSeries(t);
    UpdateMaxSeries(y);
    t->height = maxNum(heightAVLSeries(t->left), heightAVLSeries(t->right)) + 1;
    y->height = maxNum(heightAVLSeries(y->left), heightAVLSeries(y->right)) + 1;

    return y;
}

pntAVLSeries BalanceAVLSeries(pntAVLSeries t)
{
    int balanceFactor = heightAVLSeries(t->left) - heightAVLSeries(t->right);


    if (balanceFactor >= -1 && balanceFactor <= 1) {
        return t;
    }


    if (balanceFactor > 1) {

        if (heightAVLSeries(t->left->right) > heightAVLSeries(t->left->left)) {
            t->left = LeftRotateAVLSeries(t->left);
        }

        t = RightRotateAVLSeries(t);
    }

    else {

        if (heightAVLSeries(t->right->left) > heightAVLSeries(t->right->right)) {
            t->right = RightRotateAVLSeries(t->right);
        }

        t = LeftRotateAVLSeries(t);
    }

    return t;
}

void UpdateMaxSeriesTree(int serNum,pntAVLSeries t)
{
    if(t==NULL) return;
    if(t->key>serNum)UpdateMaxSeriesTree(serNum,t->left);
    else if(t->key<serNum)UpdateMaxSeriesTree(serNum,t->right);
    UpdateMaxSeries(t);
}

/*auxilary functions implementation*/

void freeStore(pntStore s)
{
    int i;
    freeAVLSeries(s->AVLSerieses);
    for(i=1;i<s->size;i++)
    {
        freeAVL(s->SeriesesArr[i]->items);
        free(s->SeriesesArr[i]);
    }
    free(s->SeriesesArr);
    free(s);
}

int maxNum(int x, int y)
{
    return x>y?x:y;
}

pntItem CreateItem(int price,char*name,int number)
{
    pntItem item =(pntItem)malloc(sizeof(Item));
    item->name=name;
    item->number=number;
    item->price=price;
    return item;
}

/*Q3 functions implementations*/

pntStore init()/*we start adding to the array from index 1 (arr[i]->sequence i)*/
{
    pntSeries ser;
    pntStore s=(pntStore)malloc(sizeof(Store));
    s->SeriesesArr=(pntSeries*)malloc(2*sizeof(pntSeries));
    s->SeriesesArr[0]=NULL;
    ser=(pntSeries)malloc(sizeof(Series));
    ser->items=NULL;
    ser->number=1;
    s->SeriesesArr[1]=ser;
    s->NextItemNumber=1;
    s->size=2;
    s->Capacity=2;
    s->AVLSerieses=insertAVLSeries(1,ser,NULL,NULL);
    return s;
}


void insert(int price,char*name,int i,pntStore s)
{
    pntItem item=CreateItem(price,name,s->NextItemNumber);
    s->SeriesesArr[i]->items=insertAVL(item->number,item,s->SeriesesArr[i]->items,NULL);
    s->NextItemNumber++;
    UpdateMaxSeriesTree(i,s->AVLSerieses);
}

void delete(int number,int i,pntStore s)
{
    s->SeriesesArr[i]->items=deleteAVL(number,s->SeriesesArr[i]->items,1);
    UpdateMaxSeriesTree(i,s->AVLSerieses);
}

pntAVL CalcSplitNode(pntAVL root, int j) {
    int leftSize;
    if (root == NULL) {
        return NULL;
    }

    /* Calculate the size of the left subtree */
    leftSize = (root->left != NULL) ? root->left->Rank : 0;

    /* If j is less than or equal to the size of the left subtree,
       the jth largest node is in the left subtree */
    if (j <= leftSize) {
        return CalcSplitNode(root->left, j);
    }

    /* If j is greater than the size of the left subtree plus 1,
       the jth largest node is in the right subtree */
    else if (j > leftSize + 1) {
        return CalcSplitNode(root->right, j - leftSize - 1);
    }
        return root;
}


void split_series(int i, int j, pntStore s) 
{
    pntAVL small;
    pntAVL big;
    pntAVL x;
    int k=CalcSplitNode(s->SeriesesArr[i]->items, j)->key;
    int newCapacity,l;

    SplitAVL(&small, &big,&x , k, s->SeriesesArr[i]->items);
    s->size++;
    s->SeriesesArr[i]->items = small;
    s->SeriesesArr[i]->items=insertAVL(x->key,x->item,s->SeriesesArr[i]->items,NULL);
    free(x);
    if (s->size > s->Capacity)
    {

        /* Double the size of SeriesesArr */
        newCapacity = 2 * s->Capacity;
        pntSeries *newSeriesesArr = (pntSeries*)malloc(sizeof(pntSeries) * newCapacity);
       /* Copy existing elements to the new array */
        for ( l = 1; l < s->Capacity; l++) {
            newSeriesesArr[l] = s->SeriesesArr[l];
        }


        /* Free the old array */
        free(s->SeriesesArr);

        /* Update the capacity and SeriesesArr */
        s->Capacity = newCapacity;
        s->SeriesesArr = newSeriesesArr;
    }
    s->SeriesesArr[s->size-1]=(pntSeries)malloc(sizeof(Series));
    s->SeriesesArr[s->size-1]->items = big;
    /* Update the number of the series in the new array */
    s->SeriesesArr[s->size-1]->number = s->size - 1;
    UpdateMaxSeriesTree(i,s->AVLSerieses);
    s->AVLSerieses=insertAVLSeries(s->size-1,s->SeriesesArr[s->size-1],s->AVLSerieses,NULL);
}


Item Public_Max(pntStore s)
{
    return *(s->AVLSerieses->MaxSeries->items->MaxPriceItem);
}

Item Priv_Max(pntStore s,int i)
{
   return *(s->SeriesesArr[i]->items->MaxPriceItem);
}

pntAVL concatenateAVL(pntAVL small,pntAVL big,pntAVL x)
{
    if(heightAVL(small) == heightAVL(big))
    {
        x->left=small;
        x->right=big;
        small->parent=x;
        big->parent=x;
        return x;

    }
    if(heightAVL(small) > heightAVL(big)) return ConcatenateLeft(small, big, x) ;
    return ConcatenateRight(small,big, x) ;
}

pntAVL ConcatenateRight(pntAVL small,pntAVL big,pntAVL x)
{
    int factor;
    if(small==NULL)
    {
        big=insertAVL(x->key,x->item,big,NULL);
        free(x);
        return big;
    }
    if(big==NULL)
    {
        small=insertAVL(x->key,x->item,small,NULL);
        free(x);
        return small;
    }
    factor=big->height-small->height;
    if(factor==0 || factor==1)
    {
        /*x->parent=big->parent;*/
        x->left=small;
        x->right=big;
        /*big->parent->left=x;*/
        big->parent=x;
        small->parent=x;
        UpdateMaxPrice(x);
        x->Rank=RankAVL(x->left)+RankAVL(x->right)+1;
        x->height=heightAVL(x->left)+heightAVL(x->right)+1;
        x=BalanceAVL(x);
        return x;
    }
    big->left=ConcatenateRight(small,big->left,x);
    if(big->left!=NULL)
    big->left->parent=big;
    UpdateMaxPrice(big);
    x->Rank=RankAVL(big->left)+RankAVL(big->right)+1;
    x->height=maxNum( heightAVL(big->left),heightAVL(big->right))+1;
    big=BalanceAVL(big);
    return big;
}

pntAVL ConcatenateLeft(pntAVL small,pntAVL big,pntAVL x)
{
    int factor;
    if(small==NULL)
    {
        big=insertAVL(x->key,x->item,big,NULL);
        free(x);
        return big;
    }
    if(big==NULL)
    {
        small=insertAVL(x->key,x->item,small,NULL);
        free(x);
        return small;
    }
    factor=small->height-big->height;
    if(factor==0 || factor==1)
    {
        /*x->parent=small->parent;*/
        x->right=big;
        x->left=small;
        /*small->parent->right=x;*/
        small->parent=x;
        big->parent=x;
        UpdateMaxPrice(x);
        x->Rank=RankAVL(x->left)+RankAVL(x->right)+1;
        x->height=heightAVL(x->left)+heightAVL(x->right)+1;
        x=BalanceAVL(x);
        return x;
    }
    small->right=ConcatenateLeft(small->right,big,x);
    if(small->right!=NULL)
    small->right->parent=small;
    UpdateMaxPrice(small);
    x->Rank=RankAVL(small->left)+RankAVL(small->right)+1;
    x->height=maxNum( heightAVL(small->left)+heightAVL(small->right))+1;
    small=BalanceAVL(small);
    return small;
}

void SplitAVL(pntAVL *small,pntAVL *big,pntAVL *x,int key,pntAVL t)
{
    pntAVL temp;
    if(t->key < key)
    {
        SplitAVL(small,big,x,key,t->right);
        t->right=t->parent=NULL;
        temp=t->left;
        t->left=NULL;
        temp->parent=NULL;
        *small=concatenateAVL(temp,*small,t);
    }
    else if(t->key > key)
    {
        SplitAVL(small,big,x,key,t->left);
        t->left=t->parent=NULL;
        temp=t->right;
        t->right=NULL;
        temp->parent=NULL;
        *big=concatenateAVL(*big,temp,t);
    }
    else
    {
        *small=t->left;
        *big=t->right;
        t->right=NULL;
        t->left=NULL;
        t->parent=NULL;
        *x=t;
    }
}

int main()
{
   return 0;
}

