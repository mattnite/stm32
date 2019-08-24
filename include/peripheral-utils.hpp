// peripheral utility types and functions
//
// Author: Matthew Knight
// File Name: peripheral-utils.hpp
// Date: 2019-08-13

#pragma once

template <typename...>
using Reset = void;

template <typename Mcu, typename Peripheral,
			typename = std::enable_if_t<
				std::is_same_v<Mcu, typename Peripheral::Mcu>>>
struct ClockEnable;

template <typename Mcu, typename Peripheral,
			typename = std::enable_if_t<
				std::is_same_v<Mcu, typename Peripheral::Mcu>>>
struct SleepEnable;

template <typename Mcu, typename Peripheral,
			typename = std::enable_if_t<
				std::is_same_v<Mcu, typename Peripheral::Mcu>>>
struct ClockSource;
