// GPIO Class
//
// Author: Matthew Knight
// File Name: gpio.hpp
// Date: 2019-05-03

#pragma once

#include "register.hpp"

#include <cstdint>

using Register32_t = Register<32>;

namespace {
	volatile uint32_t& memory(const uint32_t location) {
		return *reinterpret_cast<uint32_t*>(location);
	}
}

class Gpio {
	const uint32_t base;

public:
	Gpio(uint32_t address) : base(address) {}

	Register32_t mode() { return Register32_t(base); }
	volatile uint32_t& type() { return memory(base + 0x04); }
	volatile uint32_t& speed() { return memory(base + 0x08); }
	volatile uint32_t& pupd() { return memory(base + 0x0c); }
	volatile uint32_t& input() { return memory(base + 0x10); }
	volatile uint32_t& output() { return memory(base + 0x14); }
	volatile uint32_t& bsrr() { return memory(base + 0x18); }
	volatile uint32_t& lock() { return memory(base + 0x1C); }
	volatile uint32_t& altLow() { return memory(base + 0x20); }
	volatile uint32_t& altHigh() { return memory(base + 0x24); }
};
