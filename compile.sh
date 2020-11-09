#!/bin/bash

thd=$(pwd)
thd_build=${thd}_build_gcc
mkdir $thd_build
cd $thd_build
cmake $thd
make
