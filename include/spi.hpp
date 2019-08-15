// SPI peripheral driver
//
// Author: Matthew Knight
// File Name: spi.hpp
// Date: 2019-08-13

#pragma once

#include "gpio.hpp"

#include "svd-alias/register.hpp"

namespace Spi {
	enum class Type {
		Master,
		Slave
	};

	template <auto address>
	struct Register {
		static const auto addr = address;
	};

	template <typename Peripheral, typename DataFrame, Type type, auto lsb_first = false, auto mode = 0, typename = std::enable_if_t<
			sizeof(DataFrame) == 1 || sizeof(DataFrame) == 2	
		>>
	struct Module {
		Module() {
			static_assert(mode >= 0 && mode < 4, "SPI mode is 0-3");

			Peripheral::CR1::write<
				Svd::FieldPair<Peripheral::CR1::BR, 0>,
				Svd::FieldPair<Peripheral::CR1::CPHA, mode & 0x1>,
				Svd::FieldPair<Peripheral::CR1::CPOL, (mode & 0x2) >> 1>,
				Svd::FieldPair<Peripheral::CR1::LSBFIRST, lsb_first>,
				Svd::FieldPair<Peripheral::CR1::MSTR, type == Type::Master>,
				Svd::FieldPair<Peripheral::CR1::DFF, sizeof(DataFrame) == 1>
			>();
			
			if constexpr (type == Type::Master) {
				Peripheral::CR2::SSOE::write(1);
			}
			// - write to dedicated DMA streams if they are required
			ClockEnable<Peripheral::Mcu, Peripheral>::Field::write(1);
			Peripheral::CR1::SPIEN::write(1);
		}
			
		~Module() {	
			// TODO: more to this
			ClockEnable<Peripheral::Mcu, Peripheral>::Field::write(0);
		}
	}
};

template <typename Mcu> struct Reset<Mcu, typename Mcu::SPI1> {
	using Field = typename Mcu::RCC::APB2RSTR::SPI1RST;
};

template <typename Mcu> struct Reset<Mcu, typename Mcu::SPI2> {
	using Field = typename Mcu::RCC::APB1RSTR::SPI2RST;
};

template <typename Mcu> struct ClockEnable<Mcu, typename Mcu::SPI1> {
	using Field = typename Mcu::RCC::APB2ENR::SPI1EN;
};

template <typename Mcu> struct ClockEnable<Mcu, typename Mcu::SPI2> {
	using Field = typename Mcu::RCC::APB1ENR::SPI2EN;
};

template <typename Mcu> struct SleepEnable<Mcu, typename Mcu::SPI1> {
	using Field = typename Mcu::RCC::APB2SMENR::SPI1SMEN;
};

template <typename Mcu> struct SleepEnable<Mcu, typename Mcu::SPI2> {
	using Field = typename Mcu::RCC::APB2SMENR::SPI2SMEN;
};

struct SPI1 {
	using Mcu = STM32L0x3;

	// control register 1
	struct CR1 : public Register<0x40013000> {
		using BIDIMODE = Field<15, 1>;    // Bidirectional data mode enable
		using BIDIOE = Field<14, 1>;    // Output enable in bidirectional mode
		using CRCEN = Field<13, 1>;    // Hardware CRC calculation enable
		using CRCNEXT = Field<12, 1>;    // CRC transfer next
		using DFF = Field<11, 1>;    // Data frame format
		using RXONLY = Field<10, 1>;    // Receive only
		using SSM = Field<9, 1>;    // Software slave management
		using SSI = Field<8, 1>;    // Internal slave select
		using LSBFIRST = Field<7, 1>;    // Frame format
		using SPE = Field<6, 1>;    // SPI enable
		using BR = Field<3, 3>;    // Baud rate control
		using MSTR = Field<2, 1>;    // Master selection
		using CPOL = Field<1, 1>;    // Clock polarity
		using CPHA = Field<0, 1>;    // Clock phase
	};

	// control register 2
	struct CR2 : public Register<0x40013004> {
		using RXDMAEN = Field<0, 1>;    // Rx buffer DMA enable
		using TXDMAEN = Field<1, 1>;    // Tx buffer DMA enable
		using SSOE = Field<2, 1>;    // SS output enable
		using FRF = Field<4, 1>;    // Frame format
		using ERRIE = Field<5, 1>;    // Error interrupt enable
		using RXNEIE = Field<6, 1>;    // RX buffer not empty interrupt enable
		using TXEIE = Field<7, 1>;    // Tx buffer empty interrupt enable
	};

	// status register
	struct SR : public Register<0x40013008> {
		using RXNE = Field<0, 1>;    // Receive buffer not empty
		using TXE = Field<1, 1>;    // Transmit buffer empty
		using CHSIDE = Field<2, 1>;    // Channel side
		using UDR = Field<3, 1>;    // Underrun flag
		using CRCERR = Field<4, 1>;    // CRC error flag
		using MODF = Field<5, 1>;    // Mode fault
		using OVR = Field<6, 1>;    // Overrun flag
		using BSY = Field<7, 1>;    // Busy flag
		using TIFRFE = Field<8, 1>;    // TI frame format error
	};

	// data register
	struct DR : public Register<0x4001300c> {
		using Field = Field<0, 16>;    // Data register
	};

	// CRC polynomial register
	struct CRCPR : public Register<0x40013010> {
		using CRCPOLY = Field<0, 16>;    // CRC polynomial register
	};

	// RX CRC register
	struct RXCRCR : public RegisterReadOnly<0x40013014> {
		using RxCRC = Field<0, 16>;    // Rx CRC register
	};

	// TX CRC register
	struct TXCRCR : public RegisterReadOnly<0x40013018> {
		using TxCRC = Field<0, 16>;    // Tx CRC register
	};

	// I2S configuration register
	struct I2SCFGR : public Register<0x4001301c> {
		using I2SMOD = Field<11, 1>;    // I2S mode selection
		using I2SE = Field<10, 1>;    // I2S Enable
		using I2SCFG = Field<8, 2>;    // I2S configuration mode
		using PCMSYNC = Field<7, 1>;    // PCM frame synchronization
		using I2SSTD = Field<4, 2>;    // I2S standard selection
		using CKPOL = Field<3, 1>;    // Steady state clock polarity
		using DATLEN = Field<1, 2>;    // Data length to be transferred
		using CHLEN = Field<0, 1>;    // Channel length (number of bits per audio channel)
	};

	// I2S prescaler register
	struct I2SPR : public Register<0x40013020> {
		using MCKOE = Field<9, 1>;    // Master clock output enable
		using ODD = Field<8, 1>;    // Odd factor for the prescaler
		using I2SDIV = Field<0, 8>;    // I2S Linear prescaler
	};

};
