#!/bin/bash

SOURCE_DIR=$1
BUILD_DIR=$2
FILE_NAME=en.stm32cubel0.zip
UNPACKED_NAME=STM32Cube_FW_L0_V1.11.0
URL=http://mknight.io/files/${FILE_NAME}
STM32_CUBE=stm32-cube

if [ ! -d $STM32_CUBE ]; then
	if [ ! -f $FILE_NAME ]; then
		echo "-- Fetching STM32 CUBE L0"
		wget --no-verbose $URL
	fi

	echo "-- Extracting STM32 CUBE L0"
	unzip -q $FILE_NAME
	mv $UNPACKED_NAME $STM32_CUBE
fi
