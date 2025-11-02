#include "assign2.h"

void append(Node **head, int data) {
    Node *new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node *curr = *head;
    while (curr->next != NULL)
        curr = curr->next;
    curr->next = new_node;
}

Node* create_node(int data) {
    Node *n = malloc(sizeof(Node));
    n->data = data;
    n->next = NULL;
    return n;
}

int nodeExists(Node* head, int data){
    Node *t = malloc(sizeof(Node));
    t = head;
    while(t!=NULL){
        if(t->data == data) return 1;
        else t = t->next;
    }
    free(t);
    return 0;
}

void delete_node(Node **head, int value) {
    if (*head == NULL) return;  

    Node *temp = *head;
    Node *prev = NULL;

    
    if (temp != NULL && temp->data == value) {
        *head = temp->next; 
        free(temp);
        return;
    }

    
    while (temp != NULL && temp->data != value) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

   
    
    prev->next = temp->next;
    free(temp);
}

void printList(Node* head){
    Node *temp = head;
    
    while(temp != NULL){
        temp = temp->next;
    }
    
    return;
}