/*
 * queue.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Anmol
 */


#include "queue.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define FULL 100


void initialize(queue *q)
{
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

int isEmpty(queue *q)
{
//    return (q->rear == NULL);
	return q->count == 0;
}

void enqueue(queue *q, Customer *c)
{
    if (q->count < FULL)
    {
//        node *tmp;
//        tmp = malloc(sizeof(node));
//        tmp->c = c;
//        tmp->next = NULL;
        c->next = NULL;

        if(!isEmpty(q))
        {
            q->rear->next = c;
            q->rear = c;
        }
        else
        {
            q->front = q->rear = c;
        }
        q->count++;
    }

}

Customer* dequeue(queue *q)
{
//    node *tmp;
    Customer *n = q->front;
//    tmp = q->front;
    q->front = n->next;
    q->count--;
//    free(tmp);
    return(n);
}

int get_size(queue *q){
	return q->count;
}


//int main()
//{
//    queue *q;
//    q = malloc(sizeof(queue));
//    initialize(q);
//    enqueue(q,10);
//    enqueue(q,20);
//    enqueue(q,30);
//    printf("Queue before dequeue\n");
//    display(q->front);
//    dequeue(q);
//    printf("Queue after dequeue\n");
//    display(q->front);
//    return 0;
//}
