// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application


#define BIT_10 0x400
#define BIT_5 0x20

#define MODER_REG	0x50000000
#define ODR_REG		0x50000014

#include "stm32l0xx.h"
#include <stdbool.h>

int main() {
	RCC->IOPENR |= 0x1;
	GPIOA->MODER &= BIT_10;
	GPIOA->ODR |= BIT_5;
}
