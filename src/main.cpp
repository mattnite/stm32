// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "svd-alias/svd-alias.hpp"

using STM32L0x3;

int main() {
    // enable gpio clock
    RCC::IOPENR::IOPAEN::write(1);

    // set gpio mode and turn on led
    GPIOA::MODER::MODE5::write(2);
    GPIOA::BSRR::BS5::write(1);

    // the usual infinite loop
    while (true) {
    }
}
