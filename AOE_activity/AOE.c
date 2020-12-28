#include <stdio.h>
#include <stdlib.h>

union element{
    int count;
    int duration;
};

typedef struct node{
    int edge;
    union element num;
    int vertice;
    struct node *next;
}node;

void top_sort(int, int);
void inverse_matrix(int);
int * e, *l, *ee, *el;
node **adj_matrix = NULL;
int main()
{
    int edge_num, ver_num = 1, etmp, vtmp, *criti_path = NULL;
    scanf("%d", &edge_num);
    for(int i = 0; i < edge_num; i++)
    {
        scanf("%d", &etmp);
        if(!adj_matrix)
        {
            ver_num++;
            adj_matrix = (node **)malloc(sizeof(node *) * ver_num);
            adj_matrix[0] = (node *)malloc(sizeof(node));
            scanf("%d", &(adj_matrix[0] -> vertice));
            adj_matrix[0] -> num.count = -1;
            adj_matrix[0] -> next = (node *)malloc(sizeof(node));
            scanf("%d", &(adj_matrix[0] -> next -> vertice));
            scanf("%d", &(adj_matrix[0] -> next -> num.duration));
            adj_matrix[0] -> next -> edge = etmp;
            adj_matrix[0] -> next -> next = NULL;
            adj_matrix[1] = (node *)malloc(sizeof(node));
            adj_matrix[1] -> num.count = 0;
            adj_matrix[1] -> vertice = adj_matrix[0] -> next -> vertice;
            adj_matrix[1] -> next = NULL;
        }
        else
        {
            scanf("%d", &vtmp);
            int i;
            node *p;
            for(i = 0; i < ver_num; i++)
                if(adj_matrix[i] -> vertice == vtmp)
                {
                    for(p = adj_matrix[i]; p -> next; p = p -> next);
                    break;
                }
            if(i == ver_num)
            {
                ver_num++;
                adj_matrix = (node **)realloc(adj_matrix, sizeof(node *) * ver_num);
                adj_matrix[ver_num - 1] = (node *)malloc(sizeof(node));
                adj_matrix[ver_num - 1] -> vertice = vtmp;
                adj_matrix[ver_num - 1] -> num.count = -1;
                adj_matrix[ver_num - 1] -> next = NULL;
                p = adj_matrix[ver_num - 1];
            }
            p -> next = (node *)malloc(sizeof(node));
            p -> next -> edge = etmp;
            scanf("%d", &(p -> next -> vertice));
            scanf("%d", &(p -> next -> num.duration));
            for(i = 0; i < ver_num; i++)
                if(adj_matrix[i] -> vertice == p -> next -> vertice)
                {
                    adj_matrix[i] -> num.count++;
                    break;
                }
            if(i == ver_num)
            {
                ver_num++;
                adj_matrix = (node **)realloc(adj_matrix, sizeof(node *) * ver_num);
                adj_matrix[ver_num - 1] = (node *)malloc(sizeof(node));
                adj_matrix[ver_num - 1] -> vertice = p -> next -> vertice;
                adj_matrix[ver_num - 1] -> num.count = 0;
                adj_matrix[ver_num - 1] -> next = NULL;
            }
        }
    }
    e = (int *)malloc(sizeof(int) * edge_num);
    l = (int *)malloc(sizeof(int) * edge_num);
    ee = (int *)malloc(sizeof(int) * ver_num);
    el = (int *)malloc(sizeof(int) * ver_num);
    for(int i = 0; i < edge_num; i++)
        e[i] = 0;
    for(int i = 0; i < ver_num; i++)
        ee[i] = 0;
    top_sort(ver_num, 0);

    for(int i = 0; i < edge_num; i++)
        l[i] = ee[ver_num - 1];
    for(int i = 0; i < ver_num; i++)
        el[i] = ee[ver_num - 1];
    inverse_matrix(ver_num);
    top_sort(ver_num, 1);

    for(int i = 0; i < edge_num; i++)
        printf("%d %d %d\n", i, e[i], l[i]);
    int f = 0;
    for(int i = 0; i < edge_num; i++)
        if(!f && e[i] == l[i])
        {
            f = 1;
            printf("%d", i);
        }
        else if(e[i] == l[i])
            printf(" %d", i);

/*
    for(int i = 0; i < ver_num; i++)
        printf("%d %d\n", adj_matrix[i] -> vertice, el[i]);
    for(int i = 0; i < edge_num; i++)
        printf("%d\n", l[i]);
    
    for(int i = 0; i < ver_num; i++)
    {
        for(node *p = adj_matrix[i]; p; p = p -> next)
        {
            printf("vertice: %d\n", p -> vertice);
            if(p == adj_matrix[i])
                printf("count: %d\n", p -> num.count);
            else
                printf("edge: %d dur: %d\n", p -> edge, p -> num.duration);
        }
        printf("\n");
    }
*/
}

void top_sort(int ver_num, int a)
{
    int top = -2;
    if(a == 1)
        for(int i = 0; i < ver_num; i++)
            if(adj_matrix[i] -> num.count == -1)
                for(node *p = adj_matrix[i] -> next; p; p = p -> next)
                    l[p -> edge] = l[p -> edge] - p -> num.duration;
    while(1)
    {
        for(int i = 0; i < ver_num; i++)
            if(adj_matrix[i] -> num.count == -1)
            {
                adj_matrix[i] -> num.count = top;
                //top = adj_matrix[i] -> vertice;
                top = i;
            }
        if(top == -2)
            break;
        else
        {
            int del = top;
            top = adj_matrix[top] -> num.count;
            for(node *p = adj_matrix[del] -> next; p; p = p -> next)
            {
                for(int i = 0; i < ver_num; i++)
                    if(adj_matrix[i] -> vertice == p -> vertice)
                    {
                        adj_matrix[i] -> num.count--;
                        if(a == 0 && ee[i] < ee[del] + p -> num.duration)
                            ee[i] = ee[del] + p -> num.duration;
                        if(a == 1 && el[i] > el[del] - p -> num.duration)
                            el[i] = el[del] - p -> num.duration;
                        for(node *q = adj_matrix[i] -> next; q; q = q -> next)
                        {
                            if(a == 0 && e[q -> edge] < ee[i])
                                e[q -> edge] = ee[i];
                            if(a == 1 && l[q -> edge] > el[i] - q -> num.duration)
                                l[q -> edge] = el[i] - q -> num.duration;
                        }
                        /*
                        if(adj_matrix[i] -> num.count == -1)
                        {
                            printf("add ver: %d\n", adj_matrix[i] -> vertice);
                            adj_matrix[i] -> num.count = top;
                            top = adj_matrix[i] -> vertice;
                        }
                        */
                        break;
                    }
            }
        }
    }
}

void inverse_matrix(int ver_num)
{
    node *inv[ver_num];
    for(int i = 0; i < ver_num; i++)
    {
        adj_matrix[i] -> num.count = -1;
        node *p;
        for(p = adj_matrix[i]; p -> next; p = p -> next);
        inv[i] = p;
    }
    for(int i = 0; i < ver_num; i++)
    {
        while(adj_matrix[i] != inv[i])
        {
            int j;
            for(j = 0; j < ver_num; j++)
                if(adj_matrix[j] -> vertice == adj_matrix[i] -> next -> vertice)
                {
                    adj_matrix[i] -> num.count++;
                    node *tmp = adj_matrix[i] -> next -> next;
                    adj_matrix[i] -> next -> next = inv[j] -> next;
                    inv[j] -> next = adj_matrix[i] -> next;
                    adj_matrix[i] -> next = tmp;
                    inv[j] -> next -> vertice = adj_matrix[i] -> vertice;
                    break;
                }
            if(inv[j] -> next == inv[i])
            {
                inv[i] = adj_matrix[i];
                break;
            }
        }
    }
}
