# Conformance of jsmn (C)

## 1. Parse Validation

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail03.json` is invalid but was mistakenly deemed valid.
~~~js
{unquoted_key: "keys must be quoted"}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail04.json` is invalid but was mistakenly deemed valid.
~~~js
["extra comma",]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail05.json` is invalid but was mistakenly deemed valid.
~~~js
["double extra comma",,]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail06.json` is invalid but was mistakenly deemed valid.
~~~js
[   , "<-- missing value"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail07.json` is invalid but was mistakenly deemed valid.
~~~js
["Comma after the close"],
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

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail12.json` is invalid but was mistakenly deemed valid.
~~~js
{"Illegal invocation": alert()}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail13.json` is invalid but was mistakenly deemed valid.
~~~js
{"Numbers cannot have leading zeroes": 013}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail14.json` is invalid but was mistakenly deemed valid.
~~~js
{"Numbers cannot be hex": 0x14}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail16.json` is invalid but was mistakenly deemed valid.
~~~js
[\naked]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail19.json` is invalid but was mistakenly deemed valid.
~~~js
{"Missing colon" null}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail20.json` is invalid but was mistakenly deemed valid.
~~~js
{"Double colon":: null}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail21.json` is invalid but was mistakenly deemed valid.
~~~js
{"Comma instead of colon", null}
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail22.json` is invalid but was mistakenly deemed valid.
~~~js
["Colon instead of comma": false]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail23.json` is invalid but was mistakenly deemed valid.
~~~js
["Bad value", truth]
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

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail31.json` is invalid but was mistakenly deemed valid.
~~~js
[0e+-1]
~~~


Summary: 128 of 151 are correct.

## 2. Parse Double


Summary: 66 of 66 are correct.

## 3. Parse String

* `["Hello\nWorld"]`
  * expect: `"Hello
World"` (length: 11)
  * actual: `"Hello\nWorld"` (length: 12)

* `["Hello\u0000World"]`
  * expect: `"Hello\0World"` (length: 11)
  * actual: `"Hello\u0000World"` (length: 16)

* `["\"\\/\b\f\n\r\t"]`
  * expect: `""\/
	"` (length: 8)
  * actual: `"\"\\/\b\f\n\r\t"` (length: 15)

* `["\u0024"]`
  * expect: `"$"` (length: 1)
  * actual: `"\u0024"` (length: 6)

* `["\u00A2"]`
  * expect: `"¢"` (length: 2)
  * actual: `"\u00A2"` (length: 6)

* `["\u20AC"]`
  * expect: `"€"` (length: 3)
  * actual: `"\u20AC"` (length: 6)

* `["\uD834\uDD1E"]`
  * expect: `"𝄞"` (length: 4)
  * actual: `"\uD834\uDD1E"` (length: 12)


Summary: 2 of 9 are correct.

## 4. Roundtrip


Summary: 0 of 0 are correct.

