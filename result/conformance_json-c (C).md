# Conformance of json-c (C)

## 1. Parse Validation

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail04.json` is invalid but was mistakenly deemed valid.
~~~js
["extra comma",]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail07.json` is invalid but was mistakenly deemed valid.
~~~js
["Comma after the close"],
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail08.json` is invalid but was mistakenly deemed valid.
~~~js
["Extra close"]]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail09.json` is invalid but was mistakenly deemed valid.
~~~js
{"Extra comma": true,}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail10.json` is invalid but was mistakenly deemed valid.
~~~js
{"Extra value after close": true} "misplaced quoted value"
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail13.json` is invalid but was mistakenly deemed valid.
~~~js
{"Numbers cannot have leading zeroes": 013}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail24.json` is invalid but was mistakenly deemed valid.
~~~js
['single quote']
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail25.json` is invalid but was mistakenly deemed valid.
~~~js
["	tab	character	in	string	"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail27.json` is invalid but was mistakenly deemed valid.
~~~js
["line
break"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail29.json` is invalid but was mistakenly deemed valid.
~~~js
[0e]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail30.json` is invalid but was mistakenly deemed valid.
~~~js
[0e+]
~~~

* `y_structure_lonely_null` is valid but was mistakenly deemed invalid.
~~~js
null
~~~


Summary: 139 of 151 are correct.

## 2. Parse Double

* `[18446744073709551616]`
  * expect: `1.8446744073709552e+19 (0x01643F0000000000000)`
  * actual: `0 (0x0160)`

* `[-9223372036854775809]`
  * expect: `-9.2233720368547758e+18 (0x016C3E0000000000000)`
  * actual: `0 (0x0160)`

* `[10141204801825834086073718800384]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `0 (0x0160)`

* `[10141204801825835211973625643008]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `0 (0x0160)`

* `[10141204801825834649023672221696]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `0 (0x0160)`

* `[5708990770823838890407843763683279797179383808]`
  * expect: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`
  * actual: `0 (0x0160)`

* `[5708990770823839524233143877797980545530986496]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `0 (0x0160)`

* `[5708990770823839207320493820740630171355185152]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `0 (0x0160)`

* `[100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000]`
  * expect: `1e+308 (0x0167FE1CCF385EBC8A0)`
  * actual: `0 (0x0160)`


Summary: 57 of 66 are correct.

## 3. Parse String


Summary: 9 of 9 are correct.

## 4. Roundtrip


Summary: 27 of 27 are correct.

