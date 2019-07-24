cmake_minimum_required(VERSION 3.0)
project(stm32 LANGUAGES CXX ASM)

find_package(svd-alias REQUIRED)

set(STM32_CUBE "${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeL0")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# cpu settings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=${CPU}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=${FLOAT_ABI}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m${ENDIAN}-endian")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=${CPU}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=${FLOAT_ABI}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthumb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m${ENDIAN}-endian")

set(LINKER_SCRIPT
	"${STM32_CUBE}/Projects/NUCLEO-L073RZ/Templates/SW4STM32/STM32L073RZ_NUCLEO/STM32L073RZTx_FLASH.ld")

set(WARNING_FLAGS "${WARNING_FLAGS} -Wall")
set(SHARED_FLAGS "${SHARED_FLAGS} -g -Os -flto -nostartfiles")
set(SHARED_FLAGS "${SHARED_FLAGS} -MMD -MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SHARED_FLAGS} ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SHARED_FLAGS} ${WARNING_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS 
	"${CMAKE_EXE_LINKER_FLAGS} -flto -T ${LINKER_SCRIPT}")
	
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(stm32 
	"${STM32_CUBE}/Drivers/CMSIS/Device/ST/STM32L0xx/Source/Templates/gcc/startup_stm32l073xx.s"
	${SOURCES}
)

target_link_libraries(stm32 svd-alias::svd-alias)

add_custom_command(
	TARGET stm32 POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} stm32 -O binary stm32.bin
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Convert ELF to binary"
)
