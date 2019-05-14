// Generic Register class
//
// Author: Matthew Knight
// File Name: register.hpp
// Date: 2019-05-03

#pragma once

#include <cstdint>

template <typename T = std::uint32_t>
class Register1 {
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
