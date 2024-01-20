#include "headers.h"

int IsEmpty(Queue head)
{
    return (head->numelems == 0);
}

Queue Init(Queue Q)
{
    Q = (Queue)malloc(sizeof(struct Queuestr));
    if (Q == NULL)
    {
        // assert(0);
    }
    Q->front = NULL;
    Q->rear = NULL;
    Q->numelems = 0;
    return Q;
}

void Pushback(Queue head, char* val)
{
    {
        struct node *temp = (struct node *)malloc(sizeof(struct node));
        if (temp == NULL)
        {
            // assert(0);
        }

        if (IsEmpty(head))
        {
            temp->val = val;
            head->rear = temp;
            head->front = temp;
            temp->next = temp;
            temp->prev = temp;
            head->numelems++;
        }
        else if (head->numelems == 1)
        {
            temp->val = val;
            temp->prev = head->rear;
            temp->next = head->front;
            head->rear = temp;
            head->front->prev = temp;
            head->front->next = temp;
            head->numelems++;
        }
        else if (head->numelems > 1)
        {
            temp->val = val;
            temp->prev = head->rear;
            temp->next = head->front;
            head->rear->next = temp;
            head->rear = temp;
            head->front->prev = temp;
            head->numelems++;
        }
    }
}

char* Popfront(Queue head)
{
    if (!IsEmpty(head))
    {
        if (head->numelems == 1)
        {
            char* e = head->front->val;
            head->numelems--;
            head->front = NULL;
            head->rear = NULL;
            return e;
        }
        else if (head->numelems > 1)
        {
            char*  e = head->front->val;
            head->numelems--;
            head->front = head->front->next;
            head->front->prev = head->rear;
            head->rear->next = head->front;
            return e;
        }
        else
        {
            return NULL;
        }
    }
   return NULL;
    
}

void pushfront(Queue head, char* val)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->val = val;
    temp->next = NULL;
    temp->prev = NULL;

    if (head->numelems == 0)
    {
        head->front = temp;
        head->rear = temp;
        temp->next = temp;
        temp->prev = temp;
        head->numelems++;
    }
    else if (head->numelems == 1)
    {
        head->numelems++;
        temp->next = head->front;
        temp->prev = head->front;
        head->front->next = temp;
        head->front->prev = temp;
        head->front = temp;
        head->rear = temp->next;
    }
    else if (head->numelems > 1)
    {
        head->numelems++;
        head->front->prev = temp;
        temp->next = head->front;
        temp->prev = head->rear;
        head->front = temp;
        head->rear->next = temp;
    }
}

char* popRear(Queue head)
{
    if (!IsEmpty(head))
    {
        if (head->numelems == 1)
        {
            head->numelems--;
            char* e = head->rear->val;
            head->front = NULL;
            head->rear = NULL;
            return e;
        }
        {
            head->numelems--;
            char* e = head->rear->val;
            head->rear->prev->next = head->front;
            head->front->prev = head->rear->prev;
            head->rear = head->rear->prev;
            return e;
        }
    }
    else
    {
        return NULL;
    }
}

void Print(Queue head)
{
    if (!IsEmpty(head))
    {
        struct node *ptr;
        ptr = head->front;
        do
        {
            printf("%s ", ptr->val);
            ptr = ptr->next;
        } while (ptr != head->front);
        printf("\n");
    }

    else
    {
        printf("-1");
        printf("\n");
    }
}

void PrintReverse(Queue head)
{
    if (!IsEmpty(head))
    {
        struct node *ptr;
        ptr = head->rear;
        do
        {
            printf("%s ", ptr->val);
            ptr = ptr->prev;
        } while (ptr != head->rear);
        printf("\n");
    }
    else
    {
        printf("-1");
        printf("\n");
    }
}

char* findElem(Queue head, int pos)
{
    if (!IsEmpty(head))
    {
        if (pos <= head->numelems)
        {
            struct node *ptr;
            ptr = head->front;
            while (pos != 1)
            {
                pos--;
                ptr = ptr->next;
            }
            return ptr->val;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

void removeKElems(Queue head, int k)
{
    if (!IsEmpty(head))
    {
        struct node *ptr;
        struct node *store;
        ptr = head->front;
        if (k < head->numelems)
        {
            while (k != 0)
            {
                store = ptr->next;
                ptr->next->prev = head->rear;
                head->rear->next = ptr->next;
                head->numelems--;
                head->front = ptr->next;
                ptr->next = NULL;
                ptr->prev = NULL;
                ptr = store;
                k--;
            }
        }
        else
        {
            head->numelems = 0;
            head->front = NULL;
            head->rear = NULL;
        }
    }
    else
    {
        return;
    }
}

