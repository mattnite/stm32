set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PATH $ENV{ARM_GCC_BIN})

if(NOT TOOLCHAIN_PATH)
	message(FATAL_ERROR "The environment variable 'ARM_GCC_BIN' is not set")
endif()

set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/arm-none-eabi-g++")

