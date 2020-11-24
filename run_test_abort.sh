#!/bin/bash

prog=$(pwd)/json-benchmark
testlist=$(pwd)/../cxx-json-benchmark/reports/active_test_names.md

cd ../cxx-json-benchmark/data/jsonchecker_aborts/

while IFS= read -r line
do
    if [[ "${line:0:1}" == [0-9]  ]]
    then
        tf0=${line#*\"}
        tf1=${tf0%\"}
        for i in *.json
        do
            if [[ "$tf1" != "SimpleJSON (C++)" ]] # hangs
            then
                $prog --aborts "$tf1" ${i%.json}
            fi
        done
    fi
done < $testlist
