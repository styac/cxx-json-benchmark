#!/bin/bash

prog=$(pwd)/json-benchmark

cd ../cxx-json-benchmark/data/jsonchecker_aborts/

for i in *.json
do
    $prog --aborts "$1" ${i%.json}
done
