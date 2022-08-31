#include <stdio.h>
#include <stdlib.h>
typedef int element;
typedef struct{
    element *data;
    int capacity;
    int front,rear;
    int count;
}QueueType;
void error(char *message){
    fprintf(stderr,"%s\n",message);
    exit(1);
}
void init_queue(QueueType *q){
    q->count = q->front = q->rear = 0;
    q->capacity = 1;
}
int is_empty(QueueType *q){
    return q->count == 0;
}
int is_full(QueueType *q){
    return q->count == q->capacity;
}
void queue_print(QueueType *q){
    printf("QUEUE(front=%d rear=%d count=%d) =",q->front,q->rear,q->count);
    if (!is_empty(q)){
        int i = q->front;
        do {
            i = (i+1) % (q->capacity+1);
            printf(" %d |",q->data[i]);
            if(i == q->rear) break;
        }while (i != q->front);
    }printf("\n");
}
void resize(QueueType *q){
    q->capacity *= 2;
    q->data = (element*)realloc(q->data,q->capacity * sizeof(element));
}
void enqueue(QueueType *q,element item){
    if(is_full(q)) resize(q);
    q->rear = (q->rear + 1) % (q->capacity+1);
    q->data[q->rear] = item;
    q->count ++;
}
element dequeue(QueueType *q){
    if (is_empty(q)) error("큐가 공백상태입니다.");
    q->front = (q->front + 1)%(q->capacity+1);
    q->count --;
    return q->data[q->front];
}
int main(void){
    QueueType *queue;
    int element;
    queue->data = (int *)malloc(sizeof(int));
    init_queue(queue);
    for (int i = 1;i < 101;i++){
        enqueue(queue,i*10);
        queue_print(queue);
    }
    return 0;
}