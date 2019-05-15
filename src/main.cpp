// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "gpio.hpp"

using GpioA = Gpio<0x50000000>;

int main() {
	// enable gpio clock
	*reinterpret_cast<std::uint32_t*>(0x40021000 + 0x2C) |= 1;

	// set gpio mode and turn on led
	GpioA::Mode::set<11>();
	GpioA::Bsrr::set<5>();

	// the usual infinite loop
	while (true) {}
}
