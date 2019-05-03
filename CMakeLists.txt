cmake_minimum_required(VERSION 3.0)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/arm-gcc-toolchain.cmake")
project(stm32 LANGUAGES CXX ASM)

execute_process(
	COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/fetch_dependencies.sh"
	${CMAKE_CURRENT_SOURCE_DIR} 
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set(STM32_CUBE "${CMAKE_CURRENT_BINARY_DIR}/stm32-cube")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(CMAKE_CXX_FLAGS "\
 -std=c++17 \
 -DSTM32L073xx \
 -mthumb \
 -mthumb-interwork \
 -mcpu=cortex-m0plus \
 -mlittle-endian \
 -mfloat-abi=soft \
 -Wall \
 -nostartfiles \
 -fmessage-length=0 \
 -ffunction-sections \
 -MMD \
 -MP \
 -T ${STM32_CUBE}/Projects/NUCLEO-L073RZ/Templates/SW4STM32/STM32L073RZ_NUCLEO/STM32L073RZTx_FLASH.ld \
 -Os \
")

set(CMAKE_EXE_LINKER_FLAGS "--specs=nano.specs")

include_directories(
	"${STM32_CUBE}/Drivers/CMSIS/Device/ST/STM32L0xx/Include"
	"${STM32_CUBE}/Drivers/CMSIS/Include"
)
add_executable(stm32 
	"${STM32_CUBE}/Drivers/CMSIS/Device/ST/STM32L0xx/Source/Templates/gcc/startup_stm32l073xx.s"
	${SOURCES}
)
