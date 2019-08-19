// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "basic-timer.hpp"
#include "constants.hpp"
#include "gpio.hpp"
#include "super-loop.hpp"

#include "svd-alias/sys-tick.hpp"
#include "svd-alias/vector-table.hpp"

#include <array>
#include <tuple>

int main() {
    /*
    Gpio::Port<Mcu::GPIOA> port;
    auto pin1 = port.output<5>();
    auto pin2 = port.output<6>();
    auto pin3 = port.output<7>();

	auto superloop = make_super_loop<Mcu>(
		make_task<Tick<100>>([&](){
			pin1.toggle();
		}),
		make_task<Tick<33>>([&](){
			pin2.toggle();
		})
		,
		make_task<Interrupt<Mcu::Interrupts::ADC_COMP, Mcu::ADC::ISR::EOS, Mcu::ADC::IER::EOSIE>>([&]() {
			pin3.toggle();
		})
	);

	superloop.loop();
    */
    Gpio::Port<Mcu::GPIOA> port;
    auto pin = port.output<5>();

    // the usual infinite loop
    while (true) {
        for (auto i = 0; i < 100000; i++)
            ;

        pin.toggle();
    }
}
