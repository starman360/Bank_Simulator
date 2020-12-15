/*
 * types.c
 *
 *  Created on: Apr 20, 2020
 *      Author: Anmol
 */

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

void init_customer(Customer *c){
	c->id = 0;
	c->queue_wait_time = 0;
	c->teller_wait_time = 0;
}

void del_customer(Customer *c){
//	free(c->queue_wait_time);
//	free(c->teller_wait_time);
	free(c);
}

void send_to_teller(Customer *c, Teller *t){

}

void init_teller(Teller *t){
	t->c = NULL;
	t->state = 0;
	t->customers_serverd = 0;
	t->wait_for_customers = 0;
	t->max_wait_for_customers = 0;
}

void del_teller(Teller *t){
	del_customer(t->c);
//	free(t->customers_serverd);
//	free(t->wait_for_customers);
	free(t);
}
