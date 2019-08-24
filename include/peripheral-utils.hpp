// peripheral utility types and functions
//
// Author: Matthew Knight
// File Name: peripheral-utils.hpp
// Date: 2019-08-13

#pragma once

template <typename...>
using Reset = void;

template <typename Peripheral, typename Impl>
using Glue = Impl<typename Peripheral::Mcu, Peripheral>;

template <typename Mcu, typename Peripheral,
			typename = std::enable_if_t<
				std::is_same_v<Mcu, typename Peripheral::Mcu>>>
struct ClockEnableImpl;

template <typename Mcu, typename Peripheral,
			typename = std::enable_if_t<
				std::is_same_v<Mcu, typename Peripheral::Mcu>>>
struct SleepEnableImpl;

template <typename Mcu, typename Peripheral,
			typename = std::enable_if_t<
				std::is_same_v<Mcu, typename Peripheral::Mcu>>>
struct ClockSourceImpl;

template <typename Peripheral>
using ClockEnable = Glue<Peripheral, ClockEnableImpl>;

template <typename Peripheral>
using SleepEnable = Glue<Peripheral, SleepEnableImpl>;

template <typename Peripheral>
using ClockSource = Glue<Peripheral, ClockSourceImpl>;
