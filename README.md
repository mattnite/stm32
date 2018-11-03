# STM32L073 Program Template

This repo contains the base files needed in order to compile programs to run on
an stm32l073 microcontroller. I set this up myself because I wanted to go
through all the details required for an MCU project.

## Note on CMSIS

I have only included the base header files from CMSIS that the project needs. If
you need actual drivers, then you can get them from STM32CubeMX.

## Roadmap

There are a couple things I'd like to work on for my own skills development --
yes it would mean "recreating the wheel" but the process in doing that teaches
me things.

First I want to look into using C++ to program the MCU. arm-none-eabi has a C++
compiler and I'd like to investigate the generated assembly. Notably I'd like to
use C++14 as the constexpr feature is better developed than 11. There are a lot
of considerations for this, so I'll look at them later. In addition to that, it
would be interesting to use classes for interfacing with peripherals, so there
would be a number of different drivers to write.

Secondly, I'd like to look into being able to switch STM32 MCU's.
