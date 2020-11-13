#!/bin/bash


export c_compiler_name=$1
export cc_compiler_name=$1
thd=$(pwd)

if [ "x${c_compiler_name}" = "x" ]
then
    c_compiler_name="gcc"
    cc_compiler_name="g++"
elif [ "${compiler_name}" = "gcc" ]
then
    cc_compiler_name="g++"
fi

thd_build=${thd}_build_${c_compiler_name}
mkdir ${thd_build}

echo "C compiler: " c_compiler_name "C++ compiler: " ${cc_compiler_name} " build directory: " ${thd_build}

if [ -d $thd_build ]
then
    cd $thd_build
    echo "build with ${c_compiler_name} in " $thd_build
    cmake -D CMAKE_C_COMPILER=${c_compiler_name} -D CMAKE_CXX_COMPILER=${cc_compiler_name} $thd
    make
else
    echo $thd_build " cannot be created"
fi


