// Interrupt Vector Table
//
// Author: Matthew Knight
// File Name: interrupt-table.hpp
// Date: 2019-08-02

#pragma once

#include <tuple>

using Isr = void (*)();

std::uint32_t const irqOffset = 16;

template <auto size, std::size_t alignment> struct InterruptVectorTableBase {
    __attribute__((aligned(alignment))) volatile Isr table[size];
};

template <typename Mcu>
using InterruptVectorTableCortexM =
    InterruptVectorTableBase<Mcu::numInterrupts + irqOffset,
                             1 << Mcu::SCB::VTOR::TBLOFF::offset>;

template <typename Mcu> struct SystemExceptions;

template <typename Mcu>
struct InterruptVectorTable : public InterruptVectorTableCortexM<Mcu> {
    using Base = InterruptVectorTableCortexM<Mcu>;
    std::uint32_t const vtor;

    template <typename Pair> void loadPair(Pair pair) {
        Base::table[static_cast<std::uint32_t>(pair.first) + irqOffset] =
            pair.second;
    }

    template <typename... Pairs>
    InterruptVectorTable(Isr const &sysTick, Pairs &&... pairs)
        : vtor(Mcu::SCB::VTOR::read()) {
        Base::table[15] = sysTick;
        (loadPair(pairs), ...);
        Mcu::SCB::VTOR::write(reinterpret_cast<std::uint32_t>(Base::table));
    }

    void setSysTick(Isr const &isr) { Base::table[15] = isr; }

    ~InterruptVectorTable() { Mcu::SCB::VTOR::write(vtor); }
};
