// Usart driver
//
// Author: Matthew Knight
// File Name: uart.hpp
// Date: 2019-08-14

#pragma once

#include "peripheral-utils.hpp"

/**
 * Note: this driver only supports 16 values for oversampling
 */
template <typename Peripheral, auto baud>
struct Usart {
    using Clock = typename ClockSource<Peripheral>::Type;

    Usart() {
        Peripheral::BRR::write(
            typename Clock::Frequency::num / typename Clock::Frequemcy::den / baud;
        );
        // enable tx, rx, interrupts, and global enable in CR1
    }

    ~Usart() {

    }

    // mostly going to be used for debugging
    operator<<(char const* str) {
        char* ptr = str;
        return *this;
    }
};

template <typename Mcu>
struct ClockSource<Mcu, Mcu::USART1> {
    using Type = Mcu::Apb2;
};
