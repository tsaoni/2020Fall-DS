#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    int balance_factor;
    char *name;
    char *phone_number;
    struct node *left;
    struct node *right;
}node;

void preorder_print(node *);
node *AVL_search(char *, node *);
node *AVL_insert(char *, node *);
void AVL_delete(node *);
node *LL_rotate(node *);
node *LR_rotate(node *);
node *RL_rotate(node *);
node *RR_rotate(node *);
node *root;
int main()
{
    char input[31];
    scanf("%[^\n]%*c", input);
    while(1)
        switch(input[0])
        {
        case 'D':
            while(1)
            {
                scanf("%[^\n]%*c", input);
                if(strcmp(input, "S") == 0 || strcmp(input, "E") == 0)
                    break;
                node *r = AVL_insert(input, root);
                if(r)
                    root = r;
            }
            preorder_print(root);
            printf("\n");
        case 'S':
            while(1)
            {
                scanf("%[^\n]%*c", input);
                if(strcmp(input, "D") == 0 || strcmp(input, "E") == 0)
                    break;
                node *r = AVL_search(input, root);
                if(r)
                    printf("%s %s\n", r -> name, r -> phone_number);
                else
                    printf("%s null\n", input);
            }
        case 'E':
            return 0;
        }
        AVL_delete(root);
        /*
    int ins;
    while(1)
    {
        scanf("%d", &ins);
        node *r = AVL_insert(ins, root);
        if(r)
            root = r;
        preorder_print(root);
        printf("\n");
    }
    */
}

void preorder_print(node *cur)
{
    if(!cur)
        return;
    printf("%s ", cur -> name);
    preorder_print(cur -> left);
    preorder_print(cur -> right);
}

node *AVL_search(char *input, node *cur)
{
    if(!cur || (cur && strcmp(cur -> name, input) == 0))
        return cur;
    node *find = AVL_search(input, cur -> left);
    if(find)
        return find;
    else
        return AVL_search(input, cur -> right);
}

void AVL_delete(node *cur)
{
    if(!cur)
        return;
    AVL_delete(cur -> left);
    AVL_delete(cur -> right);
    free(cur);
}

node *AVL_insert(char *input, node *cur)
{
    node *a;
    if(!cur)
    {
        cur = (node *)malloc(sizeof(node));
        cur -> name = (char *)malloc(sizeof(char) * 20);
        strcpy(cur -> name, strtok(input, " "));
        cur -> phone_number = (char *)malloc(sizeof(char) * 11);
        strcpy(cur -> phone_number, strtok(NULL, " "));
        cur -> left = cur -> right = NULL;
        cur -> balance_factor = 0;
        return cur;
    }
    if(cur -> name[0] > input[0])
    {
        if(!cur -> left)
        {
            cur -> left = (node *)malloc(sizeof(node));
            cur -> left -> name = (char *)malloc(sizeof(char) * 20);
            strcpy(cur -> left -> name, strtok(input, " "));
            cur -> left -> phone_number = (char *)malloc(sizeof(char) * 11);
            strcpy(cur -> left -> phone_number, strtok(NULL, " "));
            cur -> left -> left = cur -> left -> right = NULL;
            cur -> left -> balance_factor = 0;
            if(!cur -> right)
            {
                cur -> balance_factor = -1;
                return cur;
            }
            else
            {
                cur -> balance_factor = 0;
                return NULL;
            }
        }
        else
            a = AVL_insert(input, cur -> left);
        if(!a)
            return NULL;
        else
        {
            if(a != cur -> left)
            {
                cur -> left = a;
                return NULL;
            }
            if(cur -> balance_factor == 1)
            {
                cur -> balance_factor = 0;
                return NULL;
            }
            else if(cur -> balance_factor == 0)
            {
                cur -> balance_factor = -1;
                return cur;
            }
            else
            {
                if(cur -> left -> balance_factor == 1)
                    return LR_rotate(cur);
                else
    //                printf("cur: %d bf: %d\n", cur -> test, cur -> balance_factor);
                    return LL_rotate(cur);
            }
        }
    }
    else
    {
        if(!cur -> right)
        {
            cur -> right = (node *)malloc(sizeof(node));
            cur -> right -> name = (char *)malloc(sizeof(char) * 20);
            strcpy(cur -> right -> name, strtok(input, " "));
            cur -> right -> phone_number = (char *)malloc(sizeof(char) * 11);
            strcpy(cur -> right -> phone_number, strtok(NULL, " "));
            cur -> right -> left = cur -> right -> right = NULL;
            cur -> right -> balance_factor = 0;
            if(!cur -> left)
            {
                cur -> balance_factor = 1;
                return cur;
            }
            else
            {
                cur -> balance_factor = 0;
                return NULL;
            }
        }
        else
            a = AVL_insert(input, cur -> right);
        if(!a)
            return NULL;
        else
        {
            if(a != cur -> right)
            {
                cur -> right = a;
                return NULL;
            }
            if(cur -> balance_factor == -1)
            {
                cur -> balance_factor = 0;
                return NULL;
            }
            else if(cur -> balance_factor == 0)
            {
                cur -> balance_factor = 1;
                return cur;
            }
            else
            {
                if(cur -> right -> balance_factor == -1)
                    return RL_rotate(cur);
                else
                    return RR_rotate(cur);
            }
        }
    }

}

node *LL_rotate(node *cur)
{
    node *tmp = cur -> left;
    cur -> left = cur -> left -> right;
    tmp -> right = cur;
    cur -> balance_factor = 0;
    tmp -> balance_factor = 0;
    return tmp;
}

node *LR_rotate(node *cur)
{
    node *tmp = cur -> left -> right;
    cur -> left -> right = tmp -> left;
    tmp -> left = cur -> left;
    cur -> left = tmp -> right;
    tmp -> right = cur;
    if(tmp -> balance_factor == -1)
        cur -> balance_factor = 1;
    else
        cur -> balance_factor = 0;
    if(tmp -> balance_factor == 1)
        tmp -> left -> balance_factor = -1;
    else
        tmp -> left -> balance_factor = 0;
    tmp -> balance_factor = 0;
    return tmp;
}

node *RL_rotate(node *cur)
{
    node *tmp = cur -> right -> left;
    cur -> right -> left = tmp -> right;
    tmp -> right = cur -> right;
    cur -> right = tmp -> left;
    tmp -> left = cur;
    if(tmp -> balance_factor == -1)
        cur -> balance_factor = 0;
    else
        cur -> balance_factor = -1;
    if(tmp -> balance_factor == 1)
        tmp -> right -> balance_factor = 0;
    else
        tmp -> right -> balance_factor = -1;
    tmp -> balance_factor = 0;
    return tmp;
}

node *RR_rotate(node *cur)
{
    node *tmp = cur -> right;
    cur -> right = cur -> right -> left;
    tmp -> left = cur;
    cur -> balance_factor = 0;
    tmp -> balance_factor = 0;
    return tmp;
}
