#ifndef __LIST__
#define __LIST__

typedef struct node{
    int data;
    struct node* next;
}Node;


int nodeExists(Node* head, int data);
Node* create_node(int data);
void append(Node **head, int data);
void delete_node(Node **head, int value);
void printList(Node* head);
#endif