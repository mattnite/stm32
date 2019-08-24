// RFM95W driver
//
// Author: Matthew Knight
// File Name: rfm95.hpp
// Date: 2019-08-14

#pragma once

#include "spi.hpp"

// Max SPI Frequency: 10MHz
template <typename Peripheral>
struct LoRa : public Spi<Peripheral, Master, 0> {
    using MaxFrequency = std::ratio<10000000, 1>;

    static_assert(std::ratio_less_equal_v<ClockSource<Peripheral>::Frequency,
                                          MaxFrequency>,
                  "SPI Clock is too high");

    struct Fifo : Spi::Register<0x00> {};
    struct OpMode : Spi::Register<0x01> {};
    struct FrfMsb : Spi::Register<0x06> {};
    struct FrfMid : Spi::Register<0x07> {};
    struct FrfLsb : Spi::Register<0x08> {};
    struct PaConfig : Spi::Register<0x09> {};
    struct PaRamp : Spi::Register<0x0a> {};
    struct Ocp : Spi::Register<0x0b> {};
    struct Lna : Spi::Register<0x0c> {};
    struct FifoAddrPtr : Spi::Register<0x0d> {};
    struct FifoTxBaseAddr : Spi::Register<0x0e> {};
    struct FifoRxBaseAddr : Spi::Register<0x0f> {};
    struct IrqFlags : Spi::Register<0x10> {};
    struct IrqFlagsMask : Spi::Register<0x11> {};
    struct FreqIfMsb : Spi::Register<0x12> {};
    struct FreqIfLsb : Spi::Register<0x13> {};
    struct SymbTimeoutMsb : Spi::Register<0x14> {};
    struct SymbTimeoutLsb : Spi::Register<0x15> {};
    struct TxCfg : Spi::Register<0x16> {};
    struct PayloadLength : Spi::Register<0x17> {};
    struct PreambleMsb : Spi::Register<0x18> {};
    struct PreambleLsb : Spi::Register<0x19> {};
    struct ModulationCfg : Spi::Register<0x1a> {};
    struct RfMode : Spi::Register<0x1b> {};
    struct HopPeriod : Spi::Register<0x1c> {};
    struct NbRxBytes : Spi::Register<0x1d> {};
    struct RxHeaderInfo : Spi::Register<0x1e> {};
    struct RxHeaderCntValue : Spi::Register<0x1f> {};
    struct RxPacketCntValue : Spi::Register<0x20> {};
    struct ModemStat : Spi::Register<0x21> {};
    struct PktSnrValue : Spi::Register<0x22> {};
    struct RssiValue : Spi::Register<0x23> {};
    struct PktRssiValue : Spi::Register<0x24> {};
    struct HopChannel : Spi::Register<0x25> {};
    struct RxDataAddr : Spi::Register<0x26> {};
    struct DioMapping1 : Spi::Register<0x40> {};
    struct DioMapping2 : Spi::Register<0x41> {};
    struct Version : Spi::Register<0x42> {};
    struct Tcxo : Spi::Register<0x4B> {};
    struct PaDac : Spi::Register<0x4D> {};
    struct FormerTemp : Spi::Register<0x5b> {};
    struct AgcRef : Spi::Register<0x61> {};
    struct AgcThresh1 : Spi::Register<0x62> {};
    struct AgcThresh2 : Spi::Register<0x63> {};
    struct AgcThresh3 : Spi::Register<0x64> {};
};
