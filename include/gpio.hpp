// GPIO Class
//
// Author: Matthew Knight
// File Name: gpio.hpp
// Date: 2019-05-03

#pragma once

#include "register.hpp"

#include <cstdint>

template <auto base>
struct Gpio {
	using Mode = Register<base>;
	using Type = Register<base + 0x04>;
	using Speed = Register<base + 0x08>;
	using Pupd = Register<base + 0x0c>;
	using Input = Register<base + 0x10>;
	using Output = Register<base + 0x14>;
	using Bsrr = Register<base + 0x18>;
	using Lock = Register<base + 0x1c>;
	using AltLow = Register<base + 0x20>;
	using AltHigh = Register<base + 0x24>;
};
