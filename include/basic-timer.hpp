// Basic Timer Driver
//
// Author: Matthew Knight
// File Name: basic-timer.hpp
// Date: 2019-08-02

#pragma once

#include <type_traits>

template <typename Peripheral,
          typename = std::enable_if_t<
              std::is_same_v<Peripheral, typename Peripheral::Mcu::TIM6> ||
              std::is_same_v<Peripheral, typename Peripheral::Mcu::TIM7>>>
struct BasicTimer {
    BasicTimer() { Peripheral::CR1::CEN::write(1); }

    ~BasicTimer() { Peripheral::CR1::CEN::write(0); }
};
