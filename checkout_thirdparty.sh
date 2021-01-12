#!/bin/bash

this_folder=$(pwd)

for i in ${this_folder}/thirdparty/*
do
    if [ -d $i ]
    then
        echo $i
        cd $i
        if [ $i = "${this_folder}/thirdparty/ArduinoJson" ]
        then
            git checkout 6.x
        elif [ $i = "${this_folder}/thirdparty/json-voorhees" ]
        then
            git checkout trunk
        else
            git checkout master
        fi
        if [ $i = "${this_folder}/thirdparty/taocppjson" ]
        then
            # taocppjson/external/PEGTL
            git submodule init
            git submodule update
        fi
    fi
done

