/**
  ******************************************************************************
  * @file      startup_stm32l073xx.s
  * @author    MCD Application Team
  * @brief     STM32L073xx Devices vector table for GCC toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == reset_handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M0+ processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * 
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb

//.global  interrupt_vector
.global  default_handler

/*
// start address for the initialization values of the .data section.
//defined in linker script
.word  _sidata
// start address for the .data section. defined in linker script
.word  _sdata
// end address for the .data section. defined in linker script
.word  _edata
// start address for the .bss section. defined in linker script
.word  _sbss
// end address for the .bss section. defined in linker script
.word  _ebss
*/

.section  .text.reset_handler
  .weak  reset_handler
  .type  reset_handler, %function
reset_handler:  
    ldr     r0, =_estack
    mov     sp, r0          /* set stack pointer */
    bl      system_init
    movs    r1, #0
    b       loop_copy_data_init

copy_data_init:
    ldr     r3, =_sidata
    ldr     r3, [r3, r1]
    str     r3, [r0, r1]
    adds    r1, r1, #4

loop_copy_data_init:
    ldr     r0, =_sdata
    ldr     r3, =_edata
    adds    r2, r0, r1
    cmp     r2, r3
    bcc     copy_data_init
    ldr     r2, =_sbss
    b       loop_fill_zero_bss

fill_zero_bss:
    movs    r3, #0
    str     r3, [r2]
    adds    r2, r2, #4

loop_fill_zero_bss:
    ldr     r3, = _ebss
    cmp     r2, r3
    bcc     fill_zero_bss
  
    bl      main

loop_forever:
    b       loop_forever

.size  reset_handler, .-reset_handler

.section  .text.default_handler,"ax",%progbits
default_handler:
    b       loop_forever
  .size  default_handler, .-default_handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  interrupt_vector, %object
  .size  interrupt_vector, .-interrupt_vector


interrupt_vector:
  .word  _estack
  .word  reset_handler
  .word  nmi_handler
/*  
  .word  HardFault_Handler

  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  0
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  .word     WWDG_IRQHandler                   
  .word     PVD_IRQHandler                    
  .word     RTC_IRQHandler                    
  .word     FLASH_IRQHandler                  
  .word     RCC_CRS_IRQHandler                
  .word     EXTI0_1_IRQHandler                
  .word     EXTI2_3_IRQHandler                
  .word     EXTI4_15_IRQHandler               
  .word     TSC_IRQHandler                    
  .word     DMA1_Channel1_IRQHandler          
  .word     DMA1_Channel2_3_IRQHandler        
  .word     DMA1_Channel4_5_6_7_IRQHandler  
  .word     ADC1_COMP_IRQHandler              
  .word     LPTIM1_IRQHandler                 
  .word     USART4_5_IRQHandler               
  .word     TIM2_IRQHandler                   
  .word     TIM3_IRQHandler                   
  .word     TIM6_DAC_IRQHandler               
  .word     TIM7_IRQHandler 				      
  .word     0              					     
  .word     TIM21_IRQHandler                  
  .word     I2C3_IRQHandler                   
  .word     TIM22_IRQHandler                  
  .word     I2C1_IRQHandler                   
  .word     I2C2_IRQHandler                   
  .word     SPI1_IRQHandler                   
  .word     SPI2_IRQHandler                   
  .word     USART1_IRQHandler                 
  .word     USART2_IRQHandler                 
  .word     RNG_LPUART1_IRQHandler            
  .word     LCD_IRQHandler                    
  .word     USB_IRQHandler                    
*/
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the default_handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/
   .weak      nmi_handler
   .thumb_set nmi_handler,default_handler
/*
   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,default_handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,default_handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,default_handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,default_handler

   .weak      WWDG_IRQHandler
   .thumb_set WWDG_IRQHandler,default_handler

   .weak      PVD_IRQHandler
   .thumb_set PVD_IRQHandler,default_handler

   .weak      RTC_IRQHandler
   .thumb_set RTC_IRQHandler,default_handler

   .weak      FLASH_IRQHandler
   .thumb_set FLASH_IRQHandler,default_handler

   .weak      RCC_CRS_IRQHandler
   .thumb_set RCC_CRS_IRQHandler,default_handler

   .weak      EXTI0_1_IRQHandler
   .thumb_set EXTI0_1_IRQHandler,default_handler

   .weak      EXTI2_3_IRQHandler
   .thumb_set EXTI2_3_IRQHandler,default_handler

   .weak      EXTI4_15_IRQHandler
   .thumb_set EXTI4_15_IRQHandler,default_handler

   .weak      TSC_IRQHandler
   .thumb_set TSC_IRQHandler,default_handler

   .weak      DMA1_Channel1_IRQHandler
   .thumb_set DMA1_Channel1_IRQHandler,default_handler

   .weak      DMA1_Channel2_3_IRQHandler
   .thumb_set DMA1_Channel2_3_IRQHandler,default_handler

   .weak      DMA1_Channel4_5_6_7_IRQHandler
   .thumb_set DMA1_Channel4_5_6_7_IRQHandler,default_handler

   .weak      ADC1_COMP_IRQHandler
   .thumb_set ADC1_COMP_IRQHandler,default_handler

   .weak      LPTIM1_IRQHandler
   .thumb_set LPTIM1_IRQHandler,default_handler

   .weak      USART4_5_IRQHandler
   .thumb_set USART4_5_IRQHandler,default_handler

   .weak      TIM2_IRQHandler
   .thumb_set TIM2_IRQHandler,default_handler

   .weak      TIM3_IRQHandler
   .thumb_set TIM3_IRQHandler,default_handler

   .weak      TIM6_DAC_IRQHandler
   .thumb_set TIM6_DAC_IRQHandler,default_handler

   .weak      TIM7_IRQHandler
   .thumb_set TIM7_IRQHandler,default_handler

   .weak      TIM21_IRQHandler
   .thumb_set TIM21_IRQHandler,default_handler

   .weak      I2C3_IRQHandler
   .thumb_set I2C3_IRQHandler,default_handler

   .weak      TIM22_IRQHandler
   .thumb_set TIM22_IRQHandler,default_handler

   .weak      I2C1_IRQHandler
   .thumb_set I2C1_IRQHandler,default_handler

   .weak      I2C2_IRQHandler
   .thumb_set I2C2_IRQHandler,default_handler

   .weak      SPI1_IRQHandler
   .thumb_set SPI1_IRQHandler,default_handler

   .weak      SPI2_IRQHandler
   .thumb_set SPI2_IRQHandler,default_handler

   .weak      USART1_IRQHandler
   .thumb_set USART1_IRQHandler,default_handler

   .weak      USART2_IRQHandler
   .thumb_set USART2_IRQHandler,default_handler

   .weak      RNG_LPUART1_IRQHandler
   .thumb_set RNG_LPUART1_IRQHandler,default_handler

   .weak      LCD_IRQHandler
   .thumb_set LCD_IRQHandler,default_handler

   .weak      USB_IRQHandler
   .thumb_set USB_IRQHandler,default_handler
*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

