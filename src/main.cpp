// Main Application
//
// Author: Matthew Knight
// File Name: main.c
// Date: 2018-10-03
//
// This file holds the main application

#include "constants.hpp"
#include "gpio.hpp"

#include "svd-alias/svd-alias.hpp"

#include <array>
#include <tuple>

void foo() {}

using Isr = void (*)();

template <auto size, std::size_t alignment> struct InterruptVectorTableBase {
    __attribute__((aligned(alignment))) volatile Isr table[size];
};

template <typename Mcu> struct IrqOffset;

template <> struct IrqOffset<STM32L0x3> {
    const static std::uint32_t value = 16;
};

template <typename Mcu>
using InterruptVectorTable =
    InterruptVectorTableBase<Mcu::numInterrupts + IrqOffset<Mcu>::value,
                             1 << Mcu::SCB::VTOR::TBLOFF::offset>;

template <typename Mcu>
struct ManagedInterruptVectorTable : public InterruptVectorTable<Mcu> {
    using Base = InterruptVectorTable<Mcu>;
    std::uint32_t const vtor;

    template <typename Pair> void loadPair(Pair pair) {
        Base::table[static_cast<std::uint32_t>(pair.first) +
                    IrqOffset<Mcu>::value] = pair.second;
    }

    template <typename... Pairs>
    ManagedInterruptVectorTable(Pairs &&... pairs)
        : vtor(Mcu::SCB::VTOR::read()) {
        (loadPair(pairs), ...);
        Mcu::SCB::VTOR::write(reinterpret_cast<std::uint32_t>(Base::table));
    }

    ~ManagedInterruptVectorTable() { Mcu::SCB::VTOR::write(vtor); }
};

int main() {
        Pulse<Mcu::GPIOA, 6> ivtPulse;
        const ManagedInterruptVectorTable<Mcu> ivt{
            std::make_pair(Mcu::Interrupts::USB, foo),
            std::make_pair(Mcu::Interrupts::RCC,
                           []() {
                               Mcu::GPIOA::BSRR::BS5::write(1);
                               Mcu::GPIOA::BSRR::BS5::write(0);
                           }),
            std::make_pair(2, []() { Pulse<Mcu::GPIOA, 5> pulse; }),
            std::make_pair(3, []() {
                *reinterpret_cast<volatile std::uint32_t *>(0x50000018) |= 1
                                                                           << 5;
                *reinterpret_cast<volatile std::uint32_t *>(0x50000018) &=
                    ~(1 << 5);
            })};

    Mcu::RCC::IOPENR::IOPAEN::write(1);

    Mcu::GPIOA::MODER::MODE5::write(1);

    // the usual infinite loop
    while (true) {
        for (auto i = 0; i < 100000; i++)
            ;

        Mcu::GPIOA::ODR::toggle(5);
    }
}
