// GPIO Class
//
// Author: Matthew Knight
// File Name: gpio.hpp
// Date: 2019-05-03

#pragma once

#include <cstdint>

volatile uint32_t& memory(const uint32_t location) {
	return *reinterpret_cast<uint32_t*>(location);
}

template <auto base>
class Gpio {

public:
	volatile static uint32_t& mode() { return memory(base); }
	volatile static uint32_t& type() { return memory(base + 0x04); }
	volatile static uint32_t& speed() { return memory(base + 0x08); }
	volatile static uint32_t& pupd() { return memory(base + 0x0c); }
	volatile static uint32_t& input() { return memory(base + 0x10); }
	volatile static uint32_t& output() { return memory(base + 0x14); }
	volatile static uint32_t& bsrr() { return memory(base + 0x18); }
	volatile static uint32_t& lock() { return memory(base + 0x1C); }
	volatile static uint32_t& altLow() { return memory(base + 0x20); }
	volatile static uint32_t& altHigh() { return memory(base + 0x24); }
};
