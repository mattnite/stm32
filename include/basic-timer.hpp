// Basic Timer Driver
//
// Author: Matthew Knight
// File Name: basic-timer.hpp
// Date: 2019-08-02

#pragma once

#include "svd-alias/svd-alias.hpp"

template <typename Peripheral>
struct BasicTimer {
	BasicTimer() {
		Peripheral::CR1::CEN::write(1);
	}

	~BasicTimer() {
		Peripheral::CR1::CEN::write(0);
	}
};
