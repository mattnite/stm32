// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "svd-alias/svd-alias.hpp"

#include <array>

template <auto size, std::size_t alignment>
struct InterruptVectorTableBase {
    __attribute__((aligned(alignment)))
    std::array<int, size> arr;
};

template <typename Mcu>
struct InterruptVectorTable : InterruptVectorTableBase<4, 2 << Mcu::SCB::VTOR::TBLOFF::offset> {

};

using Mcu = STM32L0x3;

int main() {
    volatile constexpr InterruptVectorTable<Mcu> ivt{0, 1, 2, 3};
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
