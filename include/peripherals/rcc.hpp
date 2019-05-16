// RCC peripheral abstraction
//
// Author: Matthew Knight
// File Name: rcc.hpp
// Date: 2019-05-15

#pragma once

#include "register.hpp"

namespace {
    constexpr auto base = 0x40021000;
}

struct Rcc {
    using Control = Register<base>;
    using Calibration = Register<base + 0x04>;
    using Recovery = Register<base + 0x08>;
    using Config = Register<base + 0x0c>;
    using IntEnable = Register<base + 0x10>;
    using IntFlag = Register<base + 0x14>;
    using IntClear = Register<base + 0x18>;
    using GpioReset = Register<base + 0x1c>;
    using AhbReset = Register<base + 0x20>;
    using Apb2Reset = Register<base + 0x24>;
    using Apb1Reset = Register<base + 0x28>;
    using GpioClkEn = Register<base + 0x2c>;
    using AhbClkEn = Register<base + 0x30>;
    using Apb2ClkEn = Register<base + 0x34>;
    using Apb1ClkEn = Register<base + 0x38>;
    using GpioClkEnSlp = Register<base + 0x3c>;
    using AhbClkEnSlp = Register<base + 0x40>;
    using Apb2ClkEnSlp = Register<base + 0x44>;
    using Apb1ClkEnSlp = Register<base + 0x48>;
    using Ccipr = Register<base + 0x4c>;
    using ControlStatus = Register<base + 0x50>;
};
