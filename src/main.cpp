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
#include "interrupt-table.hpp"

//#include "svd-alias/sys-tick.hpp"

#include <array>
#include <tuple>

void foo() {}

unsigned getPc() {
    unsigned pc;
    asm("mov %0, pc" : "=r"(pc));
    return pc;
}

void setPc(unsigned pc) {
    asm("mov pc, %0" : "=r"(pc));
}

int main() {
//    Svd::SysTick<Mcu> tick(Svd::Milliseconds(1));
	Mcu::GPIOA::ODR::write<
		Svd::FieldPair<Mcu::GPIOA::ODR::OD3, 1>,
		Svd::FieldPair<Mcu::GPIOA::ODR::OD5, 1>
	//	Svd::FieldPair<Mcu::GPIOA::MODER::MODE5, 1>
	>();
	/*
	const InterruptVectorTable<Mcu> ivt{
        foo,
		std::make_pair(Mcu::Interrupts::USB, foo),
		std::make_pair(Mcu::Interrupts::RCC,
					   []() {
						   Mcu::GPIOA::BSRR::BS5::write(1);
						   Mcu::GPIOA::BSRR::BS5::write(0);
					   }),
		std::make_pair(2, []() { Gpio::Pulse<Mcu::GPIOA, 5> pulse; })};

	Gpio::Port<Mcu::GPIOA> port;
	auto pin = port.output<5>();
    // the usual infinite loop
    while (true) {
        for (auto i = 0; i < 100000; i++)
            ;

		pin.toggle();
    }
	*/
}
