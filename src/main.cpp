// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "gpio.hpp"

using GPIOA = Gpio<0x50000000>;

int main() {
	// enable gpio clock
	memory(0x40021000 + 0x2C) |= 1;
	// set gpio mode and turn on led
	GPIOA::mode() &= ~(1 << 11);
	GPIOA::bsrr() |= 1 << 5;

	// the usual infinite loop
	while (true) {}
}
