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
    pntItem MaxPriceItem;
}AVL;

typedef struct ListNode *pntListNode;

typedef struct Store *pntStore;
typedef struct Store
{
    pntAVL items;
    int NextNum;
    pntListNode Month_Hits;
}Store;

typedef enum type{tree,node}Type;

typedef struct ListNode {
    Type flag; /* Flag indicating if it's a tree node*/
    pntAVL subtree; /* Pointer to a subtree (AVLTree)*/
    pntListNode next; /* Pointer to the next node in the linked list*/
} ListNode;

/*q1 Functions*/

pntStore init ();
void insert(pntStore s, int price,char* name);
void delete(pntStore s, int number);
Item Max_Price(pntStore s);

/* q2 funcs */
pntListNode AddIPath (pntAVL t , pntListNode last , int i);
pntListNode AddJPath (pntAVL t , pntListNode last , int i);
pntAVL findSplitNode(pntAVL root, int i, int j);
pntListNode createListNode(Type flag, pntAVL subtree);
void Pick_Month_Hits(pntStore s, int i, int j);
void Show_Month_Hits(pntStore s);
void freeListNode(pntListNode l);
void End_Month(pntStore s);
void printAVLNames (pntAVL t);

/* AVL functions*/

pntAVL insertAVL(int key,pntItem item,pntAVL t,pntAVL parent);
pntAVL deleteAVL(int key, pntAVL t,int flag);
pntAVL findAVL(pntAVL t, int key);
pntAVL minAVL(pntAVL t);
void freeAVLNode(pntAVL t);
int heightAVL(pntAVL t);
pntAVL BalanceAVL (pntAVL t);
pntAVL LeftRotateAVL(pntAVL t);
pntAVL RightRotateAVL(pntAVL t);
void freeAVL(pntAVL t);
void UpdateMaxPrice(pntAVL t);

/*tests functions and debugging*/
void printLevelOrder(pntAVL root);
void printGivenLevel(pntAVL root, int level);
void printAVL(pntAVL t);

/*auxilary functions...*/
void freeStore(pntStore s);
int maxNum(int x, int y);
pntItem CreateItem(int price,char*name,int number);

/*auxilary functions implementation*/

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

/*q1 Functions Implementation*/

pntStore init()
{
    pntStore s = (pntStore)malloc(sizeof(Store));
    if (s != NULL) {
        s->items = NULL;
        s->NextNum = 1;
    }
    return s;
}

Item Max_Price(pntStore s) /*O(1)*/
{
    return *(s->items->MaxPriceItem);
}

void delete(pntStore s, int number)
{
    s->items=deleteAVL(number,s->items,1);
}

void insert(pntStore s, int price,char* name)
{
    pntItem item=CreateItem(price,name,s->NextNum);
    /*s->NextNum++;*/s->NextNum+=2;
    s->items=insertAVL(item->number,item,s->items,NULL);
}

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

        t= BalanceAVL(t);
         UpdateMaxPrice(t);
        return t;
    }
}
void freeStore(pntStore s)
{
     End_Month(s);
     freeAVL(s->items);
     free(s);
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
            char*name = min->item->name;
            t->right = deleteAVL(min->key, t->right,0);
            t->key=tempKey;
            t->item=tempItem;
            t->item->name=name;
        }
    }

    if (t == NULL)
        return t;
    t->height = 1 + maxNum(heightAVL(t->left), heightAVL(t->right));
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


/*implementation of test and debug functions*/

void printAVL(pntAVL t)
{
    if(t==NULL)return;
    printAVL(t->left);
    printf("%d-%s (%d)  <%d>\n",t->key,t->item->name,t->item->price,t->MaxPriceItem->price);
    printAVL(t->right);
}

void printGivenLevel(pntAVL root, int level) {
    if (root == NULL)
        return;
    if (level == 0)
        printf("%d-%s (%d)  maxprice = <%d>   ||",root->key,root->item->name,root->item->price,root->MaxPriceItem->price);

    else if (level > 0) {
        printGivenLevel(root->left, level - 1);
        printGivenLevel(root->right, level - 1);
    }
}

void printLevelOrder(pntAVL root) {
    int h = heightAVL(root);
    int i;
  /* printf(" h = %d",h); */
    for (i = 0; i <= h; i++) {
        printGivenLevel(root, i);
        printf("\n");
    }
}


/* q2 functions implementations */

pntAVL findSplitNode(pntAVL root, int i, int j)
{
    if (root == NULL || root->key == i || root->key == j)
    {
        /* If the root is NULL or its key matches i or j, return root*/
        return root;
    }

    /* If both i and j are less than the key of the current root, search in the left subtree*/
    if (i < root->key && j < root->key) {
        return findSplitNode(root->left, i, j);
    }
    /* If both i and j are greater than the key of the current root, search in the right subtree*/
    else if (i > root->key && j > root->key)
    {
        return findSplitNode(root->right, i, j);
    }
    /* Otherwise, return the current root as the split node*/
    else
    {
        return root;
    }
}

pntListNode createListNode(Type flag, pntAVL subtree)
{
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    if (node == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    node->flag = flag;
    node->subtree = subtree;
    node->next = NULL;
    return node;
}

void Pick_Month_Hits(pntStore s, int i, int j)
{
    /* Find the split node v*/
    pntAVL splitNode = findSplitNode(s->items, i, j);
    pntListNode head,NewHead;
    /* Create the head of the linked list with flag 0 (split node)*/
    head = createListNode(node, splitNode);
    if(splitNode->key == i)NewHead=AddJPath(splitNode->right,head,j);
    else if(splitNode->key == j) NewHead=AddIPath(splitNode->left,head,i);
    else
    {
        NewHead = AddIPath(splitNode->left,head,i); /* this func should return the last node added to the list*/
        AddJPath(splitNode->right,NewHead,j);
    }
    s->Month_Hits= head;
}

pntListNode AddIPath (pntAVL t , pntListNode last , int i)
{
    pntListNode temp;
    if(t==NULL)return last;
    if (t->key==i)
    {
        last->next=createListNode(node,t);
        last=last->next;
        last->next=createListNode(tree,t->right);
        return last->next;
    }
    else if(t->key<i)
    {
        temp=AddIPath(t->right,last,i);
    }
    else if(t->key>i)
    {
        last->next=createListNode(node,t);
        last=last->next;
        last->next=createListNode(tree,t->right);
        temp=AddIPath(t->left,last->next,i);
    }
    return temp;
}

pntListNode AddJPath (pntAVL t , pntListNode last , int j)
{
    pntListNode temp;
    if(t==NULL)return last;
    if (t->key==j)
    {
        last->next=createListNode(node,t);
        last=last->next;
        last->next=createListNode(tree,t->left);
        return last->next;
    }
    else if(t->key>j)
    {
        temp=AddJPath(t->left,last,j);
    }
    else if(t->key<j)
    {
        last->next=createListNode(node,t);
        last=last->next;
        last->next=createListNode(tree,t->left);
        temp=AddJPath(t->right,last->next,j);
    }
    return temp;
}

void Show_Month_Hits(pntStore s)
{
    pntListNode p=s->Month_Hits;
    while (p!=NULL)
    {
        if(p->flag==node)printf("%s\n",p->subtree->item->name);
        else if (p->flag==tree)printAVLNames(p->subtree);
        p=p->next;
    }
}


void printAVLNames (pntAVL t)
{
    if(t==NULL)return;
    printAVLNames(t->left);
    printf("%s\n",t->item->name);
    printAVLNames(t->right);
}

void freeListNode(pntListNode l)
{
    if(l==NULL)return;

    freeListNode(l->next);
    free(l);
}

void End_Month(pntStore s)
{
    if(s->Month_Hits==NULL)return;
    else
    {
        freeListNode(s->Month_Hits);
    }
}

int main()
{
    return 0;
}

