/*
 * queue.h
 *
 *  Created on: Apr 17, 2020
 *      Author: Anmol
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "types.h"

struct node
{
    Customer *c;
    struct node *next;
};
typedef struct node node;


struct queue
{
    int count;
    Customer *front;
    Customer *rear;
};
typedef struct queue queue;

void initialize(queue *q);
int isEmpty(queue *q);
void enqueue(queue *q, Customer *c);
Customer* dequeue(queue *q);

#endif /* QUEUE_H_ */
