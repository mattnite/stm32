// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "svd-alias/svd-alias.hpp"

#include <array>

using Mcu = STM32L0x3;

void foo() {

}

using Isr = void (*)();

template <auto size, std::size_t alignment> struct InterruptVectorTableBase {
    __attribute__((aligned(alignment))) Isr table[size];
};

template <typename Mcu>
using InterruptVectorTable
    = InterruptVectorTableBase<4, 1 << Mcu::SCB::VTOR::TBLOFF::offset>;

int main() {
    volatile InterruptVectorTable<Mcu> ivt{foo, foo, foo, foo};

    Mcu::SCB::VTOR::write(reinterpret_cast<std::uint32_t>(ivt.table));

    // enable gpio clock
    Mcu::RCC::IOPENR::IOPAEN::write(1);

    // set gpio mode and turn on led
    Mcu::GPIOA::MODER::MODE5::write(2);
    Mcu::GPIOA::BSRR::BS5::write(1);

    Mcu::RCC::IOPENR::IOPAEN::write(0);

    // the usual infinite loop
    while (true) {
    }
}
