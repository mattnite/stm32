//#include "stm32l0xx.h"
#include "svd-alias/svd-alias.hpp"

#include <cstdint>

#define FLASH_BASE (0x08000000UL)
#if !defined(HSE_VALUE)
#define HSE_VALUE                                                              \
    ((uint32_t)8000000U) /*!< Value of the External oscillator in Hz */
#endif

#if !defined(MSI_VALUE)
#define MSI_VALUE                                                              \
    ((uint32_t)2097152U) /*!< Value of the Internal oscillator in Hz*/
#endif                   /* MSI_VALUE */

#if !defined(HSI_VALUE)
#define HSI_VALUE                                                              \
    ((uint32_t)16000000U) /*!< Value of the Internal \ oscillator \ in Hz*/
#endif                    /* HSI_VALUE */

/************************* Miscellaneous Configuration ************************/

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET                                                        \
    0x00U /*!< Vector Table base offset field.                                 \
              This value must be a multiple of 0x100. */
/******************************************************************************/

/* This variable is updated in three ways:
    1) by calling CMSIS function SystemCoreClockUpdate() 2) by calling HAL API
    function HAL_RCC_GetHCLKFreq() 3) each time HAL_RCC_ClockConfig() is called
    to configure the system clock frequency Note: If you use this function to
    configure the system clock; then there is no need to call the 2 first
    functions listed above, since SystemCoreClock variable is updated
    automatically.
*/
uint32_t SystemCoreClock = 2097152U; /* 32.768 kHz * 2^6 */
const uint8_t AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
                                   1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
const uint8_t APBPrescTable[8] = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};
const uint8_t PLLMulTable[9] = {3U, 4U, 6U, 8U, 12U, 16U, 24U, 32U, 48U};

/**
 * @brief  Setup the microcontroller system.
 * @param  None
 * @retval None
 */
using Mcu = STM32L0x3;

extern "C" {
void SystemInit(void) {
    Mcu::RCC::CR::MSION::write(1);

    // Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], MCOSEL[2:0], MCOPRE[2:0] bits
    Mcu::RCC::CFGR::reg() &= 0x88ff400c;

    // Reset HSION, HSIDIVEN, HSEON, CSSON and PLLON bits
    Mcu::RCC::CR::reg() &= 0xfef6fff6;

    Mcu::RCC::CRRCR::HSI48ON::write(0);
    Mcu::RCC::CR::HSEBYP::write(0);

    // Reset PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits
    Mcu::RCC::CFGR::reg() &= 0xff02ffff;

    // Disable all interrupts
    Mcu::RCC::CIER::reg() = 0;

    // Configure the Vector Table location add offset address
    Mcu::SCB::VTOR::reg() = FLASH_BASE | VECT_TAB_OFFSET;
}
}

/**
 * @brief  Update SystemCoreClock according to Clock Register Values The
 * SystemCoreClock variable contains the core clock (HCLK), it can be used by
 * the user application to setup the SysTick timer or configure other
 * parameters.
 *
 * @note   Each time the core clock (HCLK) changes, this function must be
 * called to update SystemCoreClock variable value. Otherwise, any
 * configuration based on this variable will be incorrect.
 *
 * @note   - The system frequency computed by this function is not the real
 * frequency in the chip. It is calculated based on the predefined constant and
 * the selected clock source:
 *
 *           - If SYSCLK source is MSI, SystemCoreClock will contain the MSI
 *           value as defined by the MSI range.
 *
 *           - If SYSCLK source is HSI, SystemCoreClock will contain the
 *           HSI_VALUE(*)
 *
 *           - If SYSCLK source is HSE, SystemCoreClock will contain the
 *           HSE_VALUE(**)
 *
 *           - If SYSCLK source is PLL, SystemCoreClock will contain the
 *           HSE_VALUE(**) or HSI_VALUE(*) multiplied/divided by the PLL
 *           factors.
 *
 *         (*) HSI_VALUE is a constant defined in stm32l0xx_hal.h file (default
 *         value 16 MHz) but the real value may vary depending on the
 *         variations in voltage and temperature.
 *
 *         (**) HSE_VALUE is a constant defined in stm32l0xx_hal.h file
 *         (default value 8 MHz), user has to ensure that HSE_VALUE is same as
 *         the real frequency of the crystal used. Otherwise, this function may
 *         have wrong result.
 *
 *         - The result of this function could be not correct when using
 *         fractional value for HSE crystal.  @param  None @retval None
 */
void SystemCoreClockUpdate(void) {
    /*
    uint32_t tmp = 0U, pllmul = 0U, plldiv = 0U, pllsource = 0U, msirange = 0U;

    * Get SYSCLK source
     * -------------------------------------------------------*
    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp) {
    case 0x00U: /* MSI used as system clock *
        msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> RCC_ICSCR_MSIRANGE_Pos;
        SystemCoreClock = (32768U * (1U << (msirange + 1U)));
        break;
    case 0x04U: /* HSI used as system clock *
        if ((RCC->CR & RCC_CR_HSIDIVF) != 0U) {
            SystemCoreClock = HSI_VALUE / 4U;
        } else {
            SystemCoreClock = HSI_VALUE;
        }
        break;
    case 0x08U: /* HSE used as system clock *
        SystemCoreClock = HSE_VALUE;
        break;
    default: /* PLL used as system clock *
        /* Get PLL clock source and multiplication factor
         * ----------------------*
        pllmul = RCC->CFGR & RCC_CFGR_PLLMUL;
        plldiv = RCC->CFGR & RCC_CFGR_PLLDIV;
        pllmul = PLLMulTable[(pllmul >> RCC_CFGR_PLLMUL_Pos)];
        plldiv = (plldiv >> RCC_CFGR_PLLDIV_Pos) + 1U;

        pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

        if (pllsource == 0x00U) {
            /* HSI oscillator clock selected as PLL clock entry *
            if ((RCC->CR & RCC_CR_HSIDIVF) != 0U) {
                SystemCoreClock = (((HSI_VALUE / 4U) * pllmul) / plldiv);
            } else {
                SystemCoreClock = (((HSI_VALUE)*pllmul) / plldiv);
            }
        } else {
            /* HSE selected as PLL clock entry *
            SystemCoreClock = (((HSE_VALUE)*pllmul) / plldiv);
        }
        break;
    }

    /* Compute HCLK clock frequency
     * --------------------------------------------*/
    /* Get HCLK prescaler */
    //tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos)];
    /* HCLK clock frequency */
   // SystemCoreClock >>= tmp;

}
