/*
 * types.h
 *
 *  Created on: Apr 17, 2020
 *      Author: Anmol
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef struct Customer
{
	int id;
	int queue_wait_time; // Time waited in Queue
	int teller_wait_time; // Time with Teller
	struct Customer *next;
} Customer;

void init_customer(Customer *c);
void del_customer(Customer *c);

typedef struct Teller
{
	Customer *c;
	int state; // 0 -> idle, 1 -> busy
	int customers_serverd;
	int wait_for_customers;
	int max_wait_for_customers;
} Teller;

void init_teller(Teller *t);
void del_teller(Teller *t);

void send_to_teller(Customer *c, Teller *t);





//void display(node *head)
//{
//    if(head == NULL)
//    {
//        printf("NULL\n");
//    }
//    else
//    {
//        printf("%d\n", head -> data);
//        display(head->next);
//    }
//}

#endif /* TYPES_H_ */
