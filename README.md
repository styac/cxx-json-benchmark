# C,C++ Json benchmarks, conformance check

## based on "Native JSON Benchmark" of Milo Yip (miloyip@gmail.com)

https://github.com/miloyip/nativejson-benchmark

## The results always reflect the latest states

1. clone

git clone --recursive https://github.com/styac/cxx-json-benchmark

./pull_thirdparty.sh

2. out of source build (fasttrack)

./compile.sh

3. cd ../cxx-json-benchmark_build

./json-benchmark

4. cd ../cxx-json-benchmark/result

5. make

platform:
- opensuse 15.2
- gcc 9.3

hardware:
- AMD Bulldozer 8 core not too fast settings in BIOS


![Performance](result/performance.png)

![Conformance](result/conformance.png)

------------------------------------------------------------------------




