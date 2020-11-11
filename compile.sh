#!/bin/bash

thd=$(pwd)

function build_gcc
{
    cd
    local thd_build=${thd}_build_gcc
    mkdir $thd_build
    if [ -d $thd_build ]
    then
        cd $thd_build
        echo "build with GCC in " $thd_build
        cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ $thd
        make
    else
        echo $thd_build " cannot be created"
    fi
}

function build_clang
{
    local thd_build=${thd}_build_clang
    mkdir $thd_build
    if [ -d $thd_build ]
    then
        cd $thd_build
        echo "build with CLANG in " $thd_build
        cmake  -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang $thd
        make
    else
        echo $thd_build " cannot be created"
    fi
}

build_gcc
# build_clang # must be fixed
