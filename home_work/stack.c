#include <stdio.h>
#define MAZE_SIZE 6
#define MAX_STACK_SIZE 100

typedef struct {
    short r;
    short c;
} element;
typedef struct{
    element data[MAX_STACK_SIZE];
    int top;
} StackType;


void init_stack(StackType *s){
    s -> top = -1;
}
int is_empty(StackType *s){
    return (s->top == -1);
}
int is_full(StackType *s){
    return (s->top == (MAX_STACK_SIZE -1));
}
void push(StackType *s,element item){
    if(is_full(s)){
        fprintf(stderr,"스택 포화 에러\n");
        return;
    }
    else s-> data[++(s->top)] = item;
}
element pop(StackType *s){
    if (is_empty(s)){
        fprintf(stderr,"스택 공백 에러\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}


element here = {1,0}, entry = {1,0};

char maze[MAZE_SIZE][MAZE_SIZE] = {
    {'1','1','1','1','1','1'},
    {'e','0','0','0','1','1'},
    {'1','1','0','1','1','1'},
    {'1','1','0','1','1','1'},
    {'1','0','0','0','0','x'},
    {'1','1','1','1','1','1'}
};
void push_loc(StackType *s,int r,int c){
    if (r < 0 || c < 0) return;
    if (maze[r][c] != '1' && maze[r][c] !='.'){
        element tmp;
        tmp.r = r;
        tmp.c = c;
        push(s,tmp);
    } 
}
void maze_print(char mzae[MAZE_SIZE][MAZE_SIZE]){
    printf("\n");
    for (int i = 0; i < MAZE_SIZE;i++){
        for (int j = 0; j < MAZE_SIZE;j++) printf("%c",maze[i][j]);
        printf("\n");
    }
    
}
 
int main(void)
{
    int r, c;
    StackType s;
    init_stack(&s);
    here = entry;
    while (maze[here.r][here.c] != 'x'){
        r = here.r;
        c = here.c;
        maze[r][c] = '.';
        maze_print(maze);
        push_loc(&s,r-1,c);
        push_loc(&s,r+1,c);
        push_loc(&s,r,c-1);
        push_loc(&s,r,c+1);
        if (is_empty(&s)){
            printf("실패\n");
            return 0;
        }
        else here = pop(&s);
    }
    printf("성공\n");
    return 0;
}