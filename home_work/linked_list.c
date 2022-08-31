#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct{
    char name[100];
}element;
typedef struct ListNode{
    element data;
    struct ListNode *link;
}ListNode;
ListNode* insert_first(ListNode *head, element value){
    ListNode *p = (ListNode *)malloc(sizeof(ListNode));
    p->data = value;
    p->link = head;
    head = p;
    return head;
}
int seach_list(ListNode *head,element x){
    ListNode *p = head;
    int i = 0;
    while (p != NULL){
        if (!strcmp(p->data.name,x.name)) return i;
        p = p->link;
        i ++;
    }return -1;
}
void delete(ListNode *head,int index){
    ListNode *removed,*p = head;
    int i = 0;
    if (index == -1) exit(1);
    while (++i < index) p = p->link;
    removed = p->link;
    p->link = removed->link;
    free(removed);
}
ListNode* reverse(ListNode *head){
    ListNode *p,*q,*r;
    p = head;
    q = NULL;
    while (p != NULL){
        r = q;
        q = p;
        p = p->link;
        q->link = r;
    }return q;
}
void print_list(ListNode *head){
    printf("haed -> ");
    for (ListNode *p = head; p != NULL; p = p->link) printf ("%s -> ",p->data.name);
    printf("NULL \n");
}
void split_list(ListNode *head,int index){
    ListNode *li1 = NULL,*li2 = NULL;
    ListNode *p = head;
    int i = 1;
    delete(head,index);
    while (p != NULL){
        if (i <= index) li1 = insert_first(li1,p->data);
        else li2 = insert_first(li2,p->data);
        i++;
        p = p->link;
    }
    printf("list1 : ");
    print_list(reverse(li1));
    printf("list2 : ");
    print_list(reverse(li2));
}
int main(void){
    ListNode *head = NULL;
    element data,ch;
    char li[10][10] = {"PEACH","MELON","MANGO","LEMON","BANANA","KIWI","APPLE","\0"};
    for (int i = 0;i < 7;i++){
        strcpy(data.name,li[i]);
        head = insert_first(head,data);
    }
    print_list(head);
    printf("삭제 단어 : ");
    scanf("%s",ch.name);
    int index = seach_list(head,ch);
    if (index == - 1) {
        printf("삭제할 단어가 리스트에 없습니다.");
        exit(1);
    }
    split_list(head,index);
    return 0;
}