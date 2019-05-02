cmake_minimum_required(VERSION 3.0)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/arm-gcc-toolchain.cmake")
project(stm32 LANGUAGES CXX)

execute_process(
	COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/fetch_dependencies.sh"
	${CMAKE_CURRENT_SOURCE_DIR} 
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set(STM32_CUBE "${CMAKE_CURRENT_BINARY_DIR}/stm32-cube")

set(CMAKE_CXX_FLAGS "\
 -DSTM32L073xx \
 -mthumb \
 -mcpu=cortex-m0plus \
 -mlittle-endian \
 -mfloat-abi=soft \
 -nostartfiles \
 -Wall \
 -fmessage-length=0 \
 -ffunction-sections \
 -MMD \
 -MP \
 -T ${STM32_CUBE}/Projects/NUCLEO-L073RZ/Templates/SW4STM32/STM32L073RZ_NUCLEO/STM32L073RZTx_FLASH.ld \
")

add_executable(stm32 
	"${STM32_CUBE}/Projects/NUCLEO-L073RZ/Templates/SW4STM32/startup_stm32l073xx.s"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)
