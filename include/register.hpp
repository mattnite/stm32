// Register class
//
// Author: Matthew Knight
// File Name: register.hpp
// Date: 2019-05-14

#pragma once

#include "bit-field.hpp"

#include <cstdint>

/** Register access class
 *
 * This class provides a zero overhead method for managing special function
 * registers.
 */
template <auto address, typename T = std::uint32_t> struct Register {
    template <auto offset, auto width>
    using Field = BitField<address, offset, width, T>;

    volatile static void set(T val) {
        *reinterpret_cast<T *>(address) |= 1 << val;
    }

    volatile static void clear(T val) {
        *reinterpret_cast<T *>(address) &= ~(1 << val);
    }

    template <T... bits> volatile static void set() {
        *reinterpret_cast<T *>(address) |= ((1 << bits) | ...);
    }

    template <T... bits> volatile static void clear() {
        *reinterpret_cast<T *>(address) &= (~(1 << bits) & ...);
    }

    volatile static T &reg() { return *reinterpret_cast<T *>(address); }
};
