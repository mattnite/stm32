// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "gpio.hpp"

int main() {
	Gpio a(0x50000000);
	a.mode().set<3>();
}
