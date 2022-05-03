#!/bin/bash

clear

FILE_NAME="ssdfs"

if [[ $1 == "" ]]; then
    ARG1="./simdiskfilled.bin"

else
    ARG1=$1

fi

echo -e "\033[33;1mMake clean\033[0m"
make clean

echo -e "\033[33;1mMake all\033[0m"
make all

echo -e "\033[33;1mHaciendo ejecutable\033[0m"
chmod +x "./$FILE_NAME"

echo -e "\033[33;1mCorriendo con valgrind\033[0m"
valgrind --leak-check=full --show-leak-kinds=all "./$FILE_NAME" "$ARG1"
echo -e "\033[33;1mFin\033[0m"
