#ifndef _QUEUE_H
#define _QUEUE_H
typedef struct Queuestr *Queue;

struct node
{
    char* val;
    struct node *next;
    struct node *prev;
};

struct Queuestr
{
    struct node *front;
    struct node *rear;
    int numelems;
    // int capacity;
};

int IsEmpty(Queue head);
Queue Init(Queue Q);
void Pushback(Queue head, char* val);
char* Popfront(Queue head);
void pushfront(Queue head, char* val);
char* popRear(Queue head);
void Print(Queue head);
void PrintReverse(Queue head);
char* findElem(Queue head, int pos);
void removeKElems(Queue head, int k);
void popatpos(Queue head,int pos);

#endif