#include <iostream>
#include <time.h>
#include "Profiler.h"

#define MAX_SIZE 10000

Profiler p("medie");
typedef struct _Node{
    int key;
    int rank;
    struct _Node *parent;
}Node;
typedef struct{
    int cost;
    Node *x, *y;
}Edges;
typedef struct {
    int nrNoduri;
    int nrEdges;
    Node **nodes;
    Edges **edges;
}Graf;
Graf *graf=(Graf*)calloc(1,sizeof(Graf));
int n;
Node* Make_set(int x, int *comp, int *atrr)
{
    Node *node=(Node*)calloc(1,sizeof(Node));
    if(node==NULL)
    {
        printf("Not enough memory\n");
        exit(-1);
    }
    node->key=x;
    node->parent=node;
    node->rank=0;
    (*atrr)+=3;
    return node;
}
void Link(Node *x, Node *y, int *comp, int *atrr)
{
    (*comp)++;
    (*atrr)++;
    if(x->rank>y->rank)
        y->parent=x;
    else
    {
        x->parent=y;
        (*comp)++;
        if(x->rank==y->rank)
        {
            (*atrr)++;
            y->rank++;
        }
    }
}
Node* Find_set(Node *x, int *comp, int *atrr)
{
    (*comp)++;
    if (x!=x->parent)
    {
        (*comp)++;
        (*atrr)++;
        x->parent=Find_set(x->parent,comp,atrr);
    }
    return x->parent;
}
void Union(Node *x, Node *y, int *comp, int *atrr)
{
    int n1,n2;
    Link(Find_set(x,&n1,&n2),Find_set(y,&n1,&n2),comp,atrr);
}
void swap(Edges *x, Edges *y)
{
    Edges *aux=(Edges*)calloc(1,sizeof(Edges));
    if(aux==NULL)
    {
        printf("Not enough memory");
        exit(-1);
    }
    *aux=*x;
    *x=*y;
    *y=*aux;
}
int Partition(Edges **v, int left, int right, int *comp, int *atrr)
{
    int pivot=v[right]->cost;
    int i=left-1;

    for(int j=left;j<=right-1;j++)
    {
        (*comp)++;
        if(v[j]->cost<pivot)
        {
            i++;
            swap(v[i],v[j]);
            (*atrr)+=3;
        }
    }
    swap(v[i+1],v[right]);
    (*atrr)+=3;
    return (i+1);
}
void QuickSort(Edges **v, int left, int right, int *comp, int *atrr)
{
    if(left<right)
    {
        int pivot=Partition(v,left,right,comp,atrr);

        QuickSort(v,left,pivot-1,comp,atrr);
        QuickSort(v,pivot+1,right,comp,atrr);

    }
}
/// functia pentru demo()
Graf* Kruskal(Graf *graf)
{
    Graf* A=(Graf*)calloc(1,sizeof(Graf));
    A->edges=(Edges**)calloc(graf->nrEdges,sizeof(Edges*));
    if(A==NULL || A->edges==NULL)
    {
        printf("Not enough memeory");
        exit(-1);
    }
    int n1,n2;
    for(int i=0;i<graf->nrNoduri;i++)
        graf->nodes[i]=Make_set(i+1,&n1,&n2);
    graf->edges=(Edges**)calloc(graf->nrEdges,sizeof(Edges*));
    for(int i=0;i<graf->nrEdges;i++)
    {
        graf->edges[i]=(Edges*)calloc(1,sizeof(Edges));
        if(graf->edges[i]==NULL)
        {
            printf("Not enough memeory");
            exit(-1);
        }
        scanf("%d %d %d",&n1, &n2,&(graf->edges[i]->cost));
        graf->edges[i]->x=graf->nodes[n1-1];
        graf->edges[i]->y=graf->nodes[n2-1];
    }
    QuickSort(graf->edges,0,graf->nrEdges-1,&n1,&n2);

    for(int i=0;i<graf->nrEdges;i++)
    {
        if(Find_set(graf->edges[i]->x,&n1,&n2)!=Find_set(graf->edges[i]->y,&n1,&n2))
        {
            (*A).edges[(*A).nrEdges++]=(*graf).edges[i];
            Union(graf->edges[i]->x,graf->edges[i]->y,&n1,&n2);
        }
    }
    return A;
}
///functia pentru perf asemanatoare cu cea pentru demo
void Kruskal2()
{
    Operation oprMakeSet=p.createOperation("MakeSet",n);

    Operation oprUnion=p.createOperation("Union",n);

    Operation oprFind=p.createOperation("Find",n);

    Operation oprKruskal=p.createOperation("Kruskal",n);

    int comp=0, atrr=0;
    Graf* A=(Graf*)calloc(1,sizeof(Graf));
    A->edges=(Edges**)calloc(graf->nrEdges+1,sizeof(Edges*));
    if(A==NULL || A->edges==NULL)
    {
        printf("Not enough memeory");
        exit(-1);
    }
    int nrMuchii=0;
    for(int i=0;i<graf->nrNoduri;i++)
        graf->nodes[i]=Make_set(i+1,&comp,&atrr);
    oprMakeSet.count(atrr);
    oprMakeSet.count(comp);
    oprKruskal.count(atrr);
    oprKruskal.count(comp);

    comp=0;
    atrr=0;

    for(int i=0;i<graf->nrNoduri && nrMuchii<graf->nrEdges;i++)
    {
        for(int j=1;j<graf->nrNoduri-i && nrMuchii<graf->nrEdges;j++)
        {
            graf->edges[nrMuchii]=(Edges*)calloc(1,sizeof(Edges));
            graf->edges[nrMuchii]->x=graf->nodes[i];
            graf->edges[nrMuchii]->y=graf->nodes[j];
            graf->edges[nrMuchii]->cost=(rand()%graf->nrNoduri+1);
            nrMuchii++;
        }
    }
    QuickSort(graf->edges,0,graf->nrEdges-1,&comp,&atrr);
    oprKruskal.count(atrr);
    oprKruskal.count(comp);

    comp=0;
    atrr=0;

    int comp2=0, atrr2=0;
    for(int i=0;i<graf->nrEdges;i++)
    {
        if(Find_set(graf->edges[i]->x,&comp,&atrr)!=Find_set(graf->edges[i]->y,&comp,&atrr))
        {
            (*A).edges[(*A).nrEdges++]=(*graf).edges[i];
            Union(graf->edges[i]->x,graf->edges[i]->y,&comp2,&atrr2);
        }
    }
    oprFind.count(atrr);
    oprFind.count(comp);
    oprKruskal.count(atrr);
    oprKruskal.count(comp);

    oprUnion.count(atrr2);
    oprUnion.count(comp2);
    oprKruskal.count(atrr2);
    oprKruskal.count(comp2);

    free(A);
}
void demo()
{

    Node *node1=NULL, *node2=NULL, *node3=NULL, *node4=NULL, *node5=NULL, *node6=NULL, *node7=NULL, *node8=NULL, *node9=NULL, *node10=NULL;
    Node *x=NULL;
    int n1=0,n2=0;

    node1=Make_set(1,&n1,&n2);
    node2=Make_set(2,&n1,&n2);
    node3=Make_set(3,&n1,&n2);
    node4=Make_set(4,&n1,&n2);
    node5=Make_set(5,&n1,&n2);
    node6=Make_set(6,&n1,&n2);
    node7=Make_set(7,&n1,&n2);
    node8=Make_set(8,&n1,&n2);
    node9=Make_set(9,&n1,&n2);
    node10=Make_set(10,&n1,&n2);

    Union(node1,node2,&n1,&n2);
    Union(node7,node3,&n1,&n2);
    Union(node2,node10,&n1,&n2);
    Union(node5,node6,&n1,&n2);
    Union(node8,node9,&n1,&n2);

    printf("La nodul %d parintele este %d\n",node1->key,node1->parent->key);
    printf("La nodul %d parintele este %d\n",node2->key,node2->parent->key);
    printf("La nodul %d parintele este %d\n",node3->key,node3->parent->key);
    printf("La nodul %d parintele este %d\n",node4->key,node4->parent->key);
    printf("La nodul %d parintele este %d\n",node5->key,node5->parent->key);
    printf("La nodul %d parintele este %d\n",node6->key,node6->parent->key);
    printf("La nodul %d parintele este %d\n",node7->key,node7->parent->key);
    printf("La nodul %d parintele este %d\n",node8->key,node8->parent->key);
    printf("La nodul %d parintele este %d\n",node9->key,node9->parent->key);
    printf("La nodul %d parintele este %d\n",node10->key,node10->parent->key);

    x=Find_set(node4,&n1,&n2);
    printf("\n%d ",x->key);
    x=Find_set(node1,&n1,&n2);
    printf("\n%d ",x->key);
    x=Find_set(node7,&n1,&n2);
    printf("\n%d ",x->key);
    x=Find_set(node9,&n1,&n2);
    printf("\n%d ",x->key);
    x=Find_set(node3,&n1,&n2);
    printf("\n%d\n",x->key);


    ///pentru exemplul meu o sa am un graf cu 10 noduri si 15 muchii
    Graf *graf=(Graf*)calloc(1,sizeof(Graf));
    if(graf==NULL)
    {
        printf("Not enough memory");
        exit(-1);
    }
    Graf *A=NULL;
    graf->nrNoduri=10;
    graf->nrEdges=15;
    graf->nodes=(Node**)calloc(graf->nrNoduri,sizeof(Node*));
    if(graf->nodes==NULL)
    {
        printf("Not enough memory");
        exit(-1);
    }
    A=Kruskal(graf);
    for(int i=0;i<A->nrEdges;i++)
        printf("%d %d %d\n",A->edges[i]->x->key,A->edges[i]->y->key,A->edges[i]->cost);

}
void perf()
{
    int nrMuchii=0;
    srand(time(0));

    Graf *A=NULL;
    graf->edges=(Edges**)calloc(4*MAX_SIZE+1,sizeof(Edges*));
    graf->nodes=(Node**)calloc(MAX_SIZE+1,sizeof(Node*));
    if(graf->edges==NULL || graf->nodes==NULL)
    {
        printf("Not enough memory");
        exit(-1);
    }
    for(int i=0;i<=4*MAX_SIZE;i++)
    {
        graf->edges[i]=(Edges*)calloc(1,sizeof(Edges));
        if(graf->edges[i]==NULL)
        {
            printf("Not enough memory");
            exit(-1);
        }
    }

    for(n=100;n<=10000;n+=100)
    {
        printf("\n%d\n",n);
        graf->nrEdges=4*n;
        graf->nrNoduri=n;
        Kruskal2();
    }

}
int main() {

    demo();
    ///perf();
    ///p.showReport();
    return 0;
    /*
     1 2 7
1 8 2
1 10 9
2 3 5
2 6 7
2 9 3
3 4 2
4 5 1
5 6 4
5 10 7
6 7 8
6 9 6
7 8 5
8 9 1
9 10 2
     */
}
