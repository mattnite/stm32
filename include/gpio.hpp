#pragma once

#include "constants.hpp"

#include <bitset>

#include <cstdint>

template <typename Port, auto pin>
struct Pulse {
	Pulse() {
		Port::BSRR::clear(pin);
		Port::BSRR::set(pin);
	}

	~Pulse() {
		Port::BSRR::clear(pin);
	}
};

template<typename Port, auto pin>
struct GpioPin {
	void set() {
		Port::BSRR::set(pin);
	}

	void clear() {
		Port::BSRR::clear(pin);
	}
};

template <typename Port>
struct GpioPort {
	constexpr GpioPort() {
		Mcu::RCC::IOPENR::IOPAEN::write(1);
	}

	~GpioPort() {
		Mcu::RCC::IOPENR::IOPAEN::write(0);
	}

	template <auto pin>
	GpioPin<Port, pin> get() {
		return {};
	}
};
