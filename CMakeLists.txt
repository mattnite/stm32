cmake_minimum_required(VERSION 3.0)
project(stm32 LANGUAGES CXX ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel)
endif()

find_package(svd-alias REQUIRED)

include($ENV{SVD_MODULE_PATH}/st-micro/STM32L0x3.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=${SVD_CPU}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=${SVD_FLOAT_ABI}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthumb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m${SVD_ENDIAN}-endian")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostartfiles -nostdlib")

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32L073RZTx_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto -T ${LINKER_SCRIPT}")

add_executable(stm32
    src/startup_stm32l073xx.s
    src/init-system.cpp
    src/main.cpp)
   
target_include_directories(stm32 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(stm32 svd-alias::svd-alias gcc)

add_custom_command(
	TARGET stm32 POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} stm32 -O binary stm32.bin
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Convert ELF to binary"
)
