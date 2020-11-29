
1. clone

git clone --recursive https://github.com/styac/cxx-json-benchmark

./checkout_thirdparty.sh

./pull_thirdparty.sh

2. out of source build

./compile.sh

3. cd ../cxx-json-benchmark_build

./json-benchmark

4. cd ../cxx-json-benchmark/result

5. cp ../reports/*csv .

6. make

7. view

conformance.html

performance_*.html



