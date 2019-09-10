#pragma once

#include "clock.hpp"

#include "register-alias/st-micro/STM32L0x3.hpp"

using Base = STM32L0x3;

struct Mcu : public Base {
    using Clk = Clock<Base>;
    using SysClk = Clk::SysClk<Clk::Hsi16<>>;
    using Ahb = Clk::Ahb<SysClk, 1>;
    using Apb1 = Clk::Apb1<Ahb, 1>;
    using Apb2 = Clk::Apb2<Ahb, 1>;

    template <typename... Sources>
    static void clocks_init() {
        (Sources::init(), ...);
    }

    static void clock_init() { clocks_init<SysClk, Ahb, Apb1, Apb2>(); }
};
