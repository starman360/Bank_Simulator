/*
 * UART_Helpers.h
 *
 *  Created on: May 2, 2020
 *      Author: Anmol
 */

#ifndef SRC_UART_HELPERS_H_
#define SRC_UART_HELPERS_H_

#include  <string.h>
#define UART_prints(S) HAL_UART_Transmit(&huart2,S,strlen(S),HAL_MAX_DELAY)

#include "main.h"


extern UART_HandleTypeDef huart2;

void UART_print(int integer);
//void UART_prints(uint8_t* s);
void UART_println(int integer);
void UART_printlnf(float integer);
uint8_t UART_read();
void wait_for_key(uint8_t key);


#endif /* SRC_UART_HELPERS_H_ */
