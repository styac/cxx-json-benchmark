# Conformance of JsonBox (C++)

## 1. Parse Validation

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail02.json` is invalid but was mistakenly deemed valid.
~~~js
["Unclosed array"
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail03.json` is invalid but was mistakenly deemed valid.
~~~js
{unquoted_key: "keys must be quoted"}
~~~

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

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail11.json` is invalid but was mistakenly deemed valid.
~~~js
{"Illegal expression": 1 + 2}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail13.json` is invalid but was mistakenly deemed valid.
~~~js
{"Numbers cannot have leading zeroes": 013}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail14.json` is invalid but was mistakenly deemed valid.
~~~js
{"Numbers cannot be hex": 0x14}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail15.json` is invalid but was mistakenly deemed valid.
~~~js
["Illegal backslash escape: \x15"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail17.json` is invalid but was mistakenly deemed valid.
~~~js
["Illegal backslash escape: \017"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail19.json` is invalid but was mistakenly deemed valid.
~~~js
{"Missing colon" null}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail21.json` is invalid but was mistakenly deemed valid.
~~~js
{"Comma instead of colon", null}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail22.json` is invalid but was mistakenly deemed valid.
~~~js
["Colon instead of comma": false]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail25.json` is invalid but was mistakenly deemed valid.
~~~js
["	tab	character	in	string	"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail26.json` is invalid but was mistakenly deemed valid.
~~~js
["tab\   character\   in\  string\  "]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail27.json` is invalid but was mistakenly deemed valid.
~~~js
["line
break"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail28.json` is invalid but was mistakenly deemed valid.
~~~js
["line\
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

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail31.json` is invalid but was mistakenly deemed valid.
~~~js
[0e+-1]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail32.json` is invalid but was mistakenly deemed valid.
~~~js
{"Comma instead if closing brace": true,
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail33.json` is invalid but was mistakenly deemed valid.
~~~js
["mismatch"}
~~~


Summary: 127 of 151 are correct.

## 2. Parse Double

* `[18446744073709551616]`
  * expect: `1.8446744073709552e+19 (0x01643F0000000000000)`
  * actual: `2147483647 (0x01641DFFFFFFFC00000)`

* `[-9223372036854775809]`
  * expect: `-9.2233720368547758e+18 (0x016C3E0000000000000)`
  * actual: `-2147483648 (0x016C1E0000000000000)`

* `[10141204801825834086073718800384]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `2147483647 (0x01641DFFFFFFFC00000)`

* `[10141204801825835211973625643008]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `2147483647 (0x01641DFFFFFFFC00000)`

* `[10141204801825834649023672221696]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `2147483647 (0x01641DFFFFFFFC00000)`

* `[5708990770823838890407843763683279797179383808]`
  * expect: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`
  * actual: `2147483647 (0x01641DFFFFFFFC00000)`

* `[5708990770823839524233143877797980545530986496]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `2147483647 (0x01641DFFFFFFFC00000)`

* `[5708990770823839207320493820740630171355185152]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `2147483647 (0x01641DFFFFFFFC00000)`

* `[100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000]`
  * expect: `1e+308 (0x0167FE1CCF385EBC8A0)`
  * actual: `2147483647 (0x01641DFFFFFFFC00000)`


Summary: 57 of 66 are correct.

## 3. Parse String

* `["\uD834\uDD1E"]`
  * expect: `"ùÑû"` (length: 4)
  * actual: `"Ì†¥Ì¥û"` (length: 6)


Summary: 8 of 9 are correct.

## 4. Roundtrip

* Fail:
~~~js
[-1234567890123456789]
~~~

~~~js
[-2147483648]
~~~

* Fail:
~~~js
[-9223372036854775808]
~~~

~~~js
[-2147483648]
~~~

* Fail:
~~~js
[4294967295]
~~~

~~~js
[2147483647]
~~~

* Fail:
~~~js
[1234567890123456789]
~~~

~~~js
[2147483647]
~~~

* Fail:
~~~js
[9223372036854775807]
~~~

~~~js
[2147483647]
~~~

* Fail:
~~~js
[0.0]
~~~

~~~js
[0]
~~~

* Fail:
~~~js
[-0.0]
~~~

~~~js
[-0]
~~~

* Fail:
~~~js
[1.2345]
~~~

~~~js
[1.2344999999999999]
~~~

* Fail:
~~~js
[-1.2345]
~~~

~~~js
[-1.2344999999999999]
~~~

* Fail:
~~~js
[5e-324]
~~~

~~~js
[4.9406564584124654e-324]
~~~

* Fail:
~~~js
[2.225073858507201e-308]
~~~

~~~js
[2.2250738585072009e-308]
~~~

* Fail:
~~~js
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157e+308]
~~~


Summary: 15 of 27 are correct.

