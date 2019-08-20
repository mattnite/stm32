#pragma once

#include "clock.hpp"

#include "svd-alias/st-micro/STM32L0x3.hpp"

using Mcu = STM32L0x3;
using Clk = Clock<Mcu>;
using SysClk = Clk::SysClk<Clk::Pll<Clk::Hsi16<true>, 4, 2>>;
