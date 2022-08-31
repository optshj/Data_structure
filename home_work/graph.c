#include <stdio.h>
#include <stdlib.h>

typedef int element;
element stack[100];
element visited [100];
int top = -1;

typedef struct GraphType{
    int n;
    int adj_mat[50][50];
} GraphType;
void init(GraphType *g){
    int r,c;
    g->n = 0;
    for (r=0;r<50;r++)
        for (c=0;c<50;c++) g->adj_mat[r][c] = 0;
}
void insert_vertex(GraphType *g,int v){
    if (((g->n)+1) > 50){
        fprintf(stderr,"그래프 : 정점의 개수 초과");
        return;
    }
    g->n++;
}
void insert_edge(GraphType *g,int start,int end){
    if (start >= g->n||end>= g->n){
        fprintf(stderr,"그래프 : 정점 번호 오류");
        return;
    }
    g->adj_mat[start][end] = 1;
    g->adj_mat[end][start] = 1;
}
int is_empty(){
    return (top == -1);
}
int is_full(){
    return (top == 99);
}
void push(element item){
    if (is_full()){
        fprintf(stderr,"스택 포화 에러\n");
        return;
    }
    else stack[++top] = item;
}
element pop(){
    if(is_empty()){
        fprintf(stderr,"스택 공백 에러\n");
        exit(1);
    }
    else return stack[top--];
}
void dfs_iterate(GraphType *g,element v){
    int w;
    push(v);
    while (!is_empty()){
        v = pop();
        if (!visited[v]){
            visited[v] = 1;
            for (w=0;w<g->n;w++) 
                printf("정점 %d -> ",w);
                if (g->adj_mat[v][w] && !visited[w]) {
                    push(w);
                }
        }
    }
}
int main(){
    GraphType *g;
    g = (GraphType *)malloc(sizeof(GraphType));
    init(g);
    for (int i =0;i<4;i++) insert_vertex(g,i);
    insert_edge(g,0,1);
    insert_edge(g,0,2);
    insert_edge(g,0,3);
    insert_edge(g,1,2);
    insert_edge(g,2,3);
    dfs_iterate(g,0);
    free(g);
    return 0;
}