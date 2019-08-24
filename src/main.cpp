// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "basic-timer.hpp"
#include "config.hpp"
#include "gpio.hpp"
#include "super-loop.hpp"

#include "svd-alias/sys-tick.hpp"
#include "svd-alias/vector-table.hpp"

#include <array>
#include <tuple>

int main() {
    Gpio::Port<Mcu::GPIOA> port;
    auto pin = port.output<5>();

    // the usual infinite loop
    while (true) {
        for (auto i = 0; i < 100000; i++)
            ;

        pin.toggle();
    }
}
