#!/bin/bash

cd Drivers/CMSIS/Include
dos2unix cmsis_gcc.h
patch <../../../patch/cmsis_gcc.patch
rm -f cmsis_gcc.h.rej
ls cmsis_gcc.*
cd ../../..
