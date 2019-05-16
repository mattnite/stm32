// DMA abstraction
//
// Author: Matthew Knight
// File Name: dma.hpp
// Date: 2019-05-15

#pragma once

#include "register.hpp"

namespace {
    constexpr auto base = 0x40020000;
}

struct Dma {
    using IntStatus = Register<base>;
    using IntFlagClear = Register<base + 0x04>;
    using ChanSelect = Register<base + 0xA8>;

    template <auto num> using Config = Register<base + 0x08 + (20 * num)>;
    template <auto num> using Data = Register<base + 0x0C + (20 * num)>;
    template <auto num> using PeriphAddr = Register<base + 0x10 + (20 * num)>;
    template <auto num> using MemAddr = Register<base + 0x0C + (20 * num)>;
}
