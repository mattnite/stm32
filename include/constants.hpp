#pragma once

#include "clock.hpp"

#include "svd-alias/st-micro/STM32L0x3.hpp"

using Mcu = STM32L0x3;
using SysClk =
    Clock<Mcu>::Control<Clock<Mcu>::Pll<Clock<Mcu>::Hsi16<true>, 4, 2>>;
