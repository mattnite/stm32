// Generic Register class
//
// Author: Matthew Knight
// File Name: register.hpp
// Date: 2019-05-03

#pragma once

#include <array>

template <uint32_t Width>
class Register {
	const uint32_t address;

public:
	Register(uint32_t address) : address(address) {}

	template <uint32_t Bit>
	void set() {
		*reinterpret_cast<uint32_t*>(address) |= 1 << Bit;
		static_assert(Bit < Width);
	}

	void set(const uint32_t bit) {
		if (bit <Width)
			*reinterpret_cast<uint32_t*>(address) |= 1 << bit;
	}
};
