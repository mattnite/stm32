// peripheral utility types and functions
//
// Author: Matthew Knight
// File Name: peripheral-utils.hpp
// Date: 2019-08-13

#pragma once

template <typename...>
using Reset = void;

template <typename Peripheral, template <typename, typename> typename Impl>
using Glue = Impl<typename Peripheral::Mcu, Peripheral>;

template <typename Mcu, typename Peripheral>
using McuMatch =
    std::enable_if_t<std::is_same_v<Mcu, typename Peripheral::Mcu>>;

// Peripheral attribute declarations
template <typename Mcu, typename Peripheral,
          typename = McuMatch<Mcu, Peripheral>>
struct ClockEnableImpl;

template <typename Mcu, typename Peripheral,
          typename = McuMatch<Mcu, Peripheral>>
struct SleepEnableImpl;

template <typename Mcu, typename Peripheral,
          typename = McuMatch<Mcu, Peripheral>>
struct ClockSourceImpl;

// Peripheral attribute
template <typename Peripheral>
using ClockEnable = Glue<Peripheral, ClockEnableImpl>;

template <typename Peripheral>
using SleepEnable = Glue<Peripheral, SleepEnableImpl>;

template <typename Peripheral>
using ClockSource = Glue<Peripheral, ClockSourceImpl>;
