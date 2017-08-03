#!/bin/sh

BUILD_DIR=build

COL_GREEN='\033[0;32m'
COL_RED='\033[0;31m'
COL_NORM='\033[0m'

echo "Creating build directory..."

mkdir ${BUILD_DIR}
if [ $? -eq 0 ]; then
	echo -e "${COL_GREEN}Done${COL_NORM}"
else
	echo -e "${COL_RED}Failed to create build directory!${COL_NORM}"
	exit
fi

echo "Copying configuration file..."

cp config/tup.config.conf ${BUILD_DIR}/tup.config
if [ $? -eq 0 ]; then
	echo -e "${COL_GREEN}Done${COL_NORM}"
else
	echo -e "${COL_RED}Failed to copy configuration file!${COL_NORM}"
	exit
fi

echo "Now run 'tup init' to initialise the tup tree and then 'tup ${BUILD_DIR}' to build the project"
