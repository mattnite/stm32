// STM32L0x3 Clock Control driver
//
// Author: Matthew Knight
// File Name: clock-control.hpp
// Date: 2019-08-18

#pragma once

#include "svd-alias/util.hpp"

#include <ratio>

namespace {
    template <auto... nums>
    constexpr bool is_one_of(std::uint32_t val) {
        return ((nums == val) | ...);
    }

    template <typename Field, auto val>
    struct WriteField {
        static void execute() { Field::write(val); }
    };

    template <typename Field, auto val>
    struct FieldEquals {
        static void execute() {
            while (Field::read() != val)
                ;
        }
    };

    template <typename ModifyAction, typename WaitFor>
    static void modify_wait() {
        ModifyAction::execute();
        WaitFor::execute();
    }
} // namespace

template <typename Mcu>
struct Clock {
    using MaxSysClkFrequency = std::ratio<32000000, 1>;

    enum class SysClkSource { Msi, Hsi16, Hse, Pll };

    template <typename Source, auto mul, auto div>
    struct Pll {
        using Frequency = std::ratio_multiply<typename Source::Frequency,
                                              std::ratio<mul, div>>;

        static_assert(is_one_of<3, 4, 6, 8, 12, 16, 24, 32, 48>(mul),
                      "invalid PLL multiplier");
        static_assert(is_one_of<2, 3, 4>(div), "invalid PLL divider");

        static void init() {
            if (static_cast<SysClkSource>(Mcu::RCC::CFGR::SW::read()) ==
                SysClkSource::Pll) {
                modify_wait<
                    WriteField<typename Mcu::RCC::CFGR::SW,
                               static_cast<std::uint32_t>(SysClkSource::Hsi16)>,
                    FieldEquals<typename Mcu::RCC::CFGR::SWS,
                                static_cast<std::uint32_t>(
                                    SysClkSource::Hsi16)>>();
            }

            modify_wait<WriteField<typename Mcu::RCC::CR::PLLON, 0>,
                        FieldEquals<typename Mcu::RCC::CR::PLLRDY, 0>>();

            // TODO: map mul/div values to the enumerated values needed
            Mcu::Flash::ACR::LATENCY::write(1);
            Mcu::RCC::CFGR::PLLMUL::write(mul);
            Mcu::RCC::CFGR::PLLDIV::write(div);
            // TODO: set pll source

            Source::init();

            // Pll enable
            modify_wait<WriteField<typename Mcu::RCC::CR::PLLON, 1>,
                        FieldEquals<typename Mcu::RCC::CR::PLLRDY, 1>>();

            // set Pll as sysclk
            modify_wait<
                WriteField<typename Mcu::RCC::CFGR::SW,
                           static_cast<std::uint32_t>(SysClkSource::Pll)>,
                FieldEquals<typename Mcu::RCC::CFGR::SWS,
                            static_cast<std::uint32_t>(SysClkSource::Pll)>>();
        }
    };

    template <bool div_four = false>
    struct Hsi16 {
        using Frequency = std::conditional_t<div_four, std::ratio<4000000, 1>,
                                             std::ratio<16000000, 1>>;

        static void init() {
            Mcu::RCC::CR::HSI16DIVEN::write(div_four);

            modify_wait<WriteField<typename Mcu::RCC::CR::HSI16ON, 1>,
                        FieldEquals<typename Mcu::RCC::CR::HSI16RDYF, 1>>();
        }
    };

    template <typename Config>
    struct Control {
        using Frequency = typename Config::Frequency;

        static_assert(std::ratio_less_equal_v<Frequency, MaxSysClkFrequency>,
                      "SYSCLK is set too high (max is 32MHz)");

        static void init() { Config::init(); }
    };
};
