/*
 * UART_Helpers.c
 *
 *  Created on: May 2, 2020
 *      Author: Anmol
 */

#include "UART_Helpers.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int Rx_data[];

void UART_print(int integer) {
	uint8_t buffer[64];
	int n = sprintf((char *)buffer, "%d", integer);
	HAL_UART_Transmit(&huart2, buffer, n, HAL_MAX_DELAY);
}

//void UART_prints(uint8_t* s) {
//	HAL_UART_Transmit(&huart2, s, strlen(s), HAL_MAX_DELAY);
//}

uint8_t UART_read(){
	uint8_t buffer = Rx_data[0];
	Rx_data[0] = 0;
	return buffer;
}

void wait_for_key(uint8_t key){
	while(Rx_data[0] != key);
	Rx_data[0] = 0;
}

void UART_println(int integer) {
	uint8_t buffer[64];
	int n = sprintf((char *)buffer, "%d\r\n", integer);
	HAL_UART_Transmit(&huart2, buffer, n, HAL_MAX_DELAY);
}

void UART_printlnf(float integer) {
	uint8_t buffer[64];
	int n = sprintf((char *)buffer, "%f\r\n", integer);
	HAL_UART_Transmit(&huart2, buffer, n, HAL_MAX_DELAY);
}
