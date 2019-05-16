// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "peripherals/gpio.hpp"
#include "peripherals/rcc.hpp"

#include "bit-field.hpp"

int main() {
    // enable gpio clock
    Rcc::GpioClkEn::set<0>();

    // set gpio mode and turn on led
    GpioA::Mode::set<11>();
    GpioA::Bsrr::set<5>();

    // the usual infinite loop
    while (true) {
    }
}
