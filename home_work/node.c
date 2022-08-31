#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct element{
    char name[100];
    char phone[100];
}element;
typedef struct TreeNode{
    element data;
    struct TreeNode *left, *right;
}TreeNode;

TreeNode *new_node(char name[],char phone[]){
    TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));
    strcpy(temp->data.name,name);
    strcpy(temp->data.phone,phone);
    temp->left = temp->right = NULL;
    return temp;
}

int get_node_count(TreeNode *node){
    int cnt = 0;
    if (node != NULL) cnt = 1 + get_node_count(node->left) + get_node_count(node->right);
    return cnt;
}

TreeNode *max_value_node(TreeNode *node){
    TreeNode *current = node;
    while (current->right != NULL) current = current->right;
    return current;
}

void inorder(TreeNode *root){
    if (root){
        inorder (root->left);
        printf("�̸� : %s\n",root->data.name);
        printf("��ȭ��ȣ : %s\n",root->data.phone);
        inorder(root->right);
    }
}

void print_node(TreeNode *root){
    printf("��ȭ��ȣ�� ���� : %d��\n",get_node_count(root));
    inorder(root);
}

TreeNode *search(TreeNode *node,char key[]){
    while (node != NULL){
        if (strcmp(key,node->data.name) == 0) return node;
        else if (strcmp(key,node->data.name) < 0) node = node->left;
        else node = node->right;
    }
    return NULL;
}

TreeNode *insert(TreeNode *node,char key[],char phone[]){
    if (node == NULL) return new_node(key,phone);
    if (strcmp(key,node->data.name) == 0) strcpy(node->data.phone,phone);
    else if (strcmp(key,node->data.name) < 0) node->left = insert(node->left,key,phone);
    else if (strcmp(key,node->data.name) > 0) node->right = insert(node->right,key,phone);
    return node;
}

TreeNode *delete(TreeNode *root,char key[]){
    if (root == NULL) return root;
    if (strcmp(key,root->data.name) < 0) root->left = delete(root->left,key);
    else if (strcmp(key,root->data.name) > 0) root->right = delete(root->right,key);
    else {
        if (root->left == NULL) {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }
        TreeNode *temp = max_value_node(root->left);
        root->data = temp->data;
        root->left = delete(root->left,temp->data.name);
    }
    return root;
}

int main(){
    char c[10] ,name[100],phone[100];
    TreeNode * root = NULL;
    TreeNode * tmp;

    while(1){

        printf("����(i), Ž��(s), ���(p), ����(d), ����(q): ");
        scanf("%s",c);

        if (strcmp(c,"q") == 0) break;

        else if (strcmp(c,"i") == 0){
            printf("�̸� : ");
            scanf("%s",name);
            printf("��ȭ��ȣ : ");
            scanf("%s",phone);
            root = insert(root,name,phone);
        }
        else if (strcmp(c,"s") == 0){
            printf("�˻� �̸� : ");
            scanf("%s",name);
            tmp = search(root,name);
            if (tmp == NULL) {
                printf("�ش��ϴ� �̸��� ã�� �� �����ϴ�.");
                continue;
            }
            printf("'%s'�� ��ȭ��ȣ : %s\n",name,tmp->data.phone);
        }
        else if (strcmp(c,"p") == 0){
            print_node(root);
        }
        else if (strcmp(c,"d") == 0){
            printf("������ �̸� : ");
            scanf("%s",name);
            root = delete(root,name);
        }
    }
    printf("���α׷��� ����Ǿ����ϴ�.");
    return 0;
}