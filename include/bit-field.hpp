// Bit field Abstraction
//
// Author: Matthew Knight
// File Name: bit-field.hpp
// Date: 2019-05-15

#pragma once

#include <cstdint>

template <auto address, auto offset, auto width, typename T = std::uint32_t>
struct BitField {
    constexpr static auto max = (1 << width) - 1;
    constexpr static auto mask = max << offset;

    volatile static void set(const T &val) {
        auto ptr = reinterpret_cast<T *>(address);
        *ptr = (*ptr & ~mask) | (mask & (val << offset));
    }

    volatile static T get() {
        return (*reinterpret_cast<T *>(address) & mask) >> offset;
    }
};
