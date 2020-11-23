# Conformance of Vinenthz libjson (C)

## 1. Parse Validation

* `y_structure_lonely_true` is valid but was mistakenly deemed invalid.
~~~js
true
~~~

* `y_number_0e1` is valid but was mistakenly deemed invalid.
~~~js
[0e1]
~~~

* `y_string_space` is valid but was mistakenly deemed invalid.
~~~js
" "
~~~

* `y_structure_lonely_false` is valid but was mistakenly deemed invalid.
~~~js
false
~~~

* `y_structure_lonely_string` is valid but was mistakenly deemed invalid.
~~~js
"asd"
~~~

* `y_structure_lonely_negative_real` is valid but was mistakenly deemed invalid.
~~~js
-0.1
~~~

* `y_number_0e+1` is valid but was mistakenly deemed invalid.
~~~js
[0e+1]
~~~

* `y_structure_lonely_int` is valid but was mistakenly deemed invalid.
~~~js
42
~~~

* `y_structure_lonely_null` is valid but was mistakenly deemed invalid.
~~~js
null
~~~

* `y_structure_string_empty` is valid but was mistakenly deemed invalid.
~~~js
""
~~~


Summary: 119 of 129 are correct.

## 2. Parse Double

* `[18446744073709551616]`
  * expect: `1.8446744073709552e+19 (0x01643F0000000000000)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[10141204801825834086073718800384]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[10141204801825835211973625643008]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[10141204801825834649023672221696]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[5708990770823838890407843763683279797179383808]`
  * expect: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[5708990770823839524233143877797980545530986496]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[5708990770823839207320493820740630171355185152]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000]`
  * expect: `1e+308 (0x0167FE1CCF385EBC8A0)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`


Summary: 58 of 66 are correct.

## 3. Parse String


Summary: 9 of 9 are correct.

## 4. Roundtrip

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
{"a":null,"foo":"bar"}
~~~

~~~js
{"a":"null","foo":"bar"}
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
[-1.2345]
~~~

~~~js
[-1.2344999999999999]
~~~

* Fail:
~~~js
[false]
~~~

~~~js
["false"]
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
[0.0]
~~~

~~~js
[0]
~~~

* Fail:
~~~js
[null]
~~~

~~~js
["null"]
~~~

* Fail:
~~~js
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157e+308]
~~~


Summary: 17 of 27 are correct.

