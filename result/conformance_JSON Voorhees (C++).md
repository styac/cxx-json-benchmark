# Conformance of JSON Voorhees (C++)

## 1. Parse Validation

* `y_object_duplicated_key_and_value` is valid but was mistakenly deemed invalid.
~~~js
{"a":"b","a":"b"}
~~~

* `y_object_duplicated_key` is valid but was mistakenly deemed invalid.
~~~js
{"a":"b","a":"c"}
~~~

* `fail09` is valid but was mistakenly deemed invalid.
~~~js
{"Extra comma": true,}
~~~

* `fail13` is valid but was mistakenly deemed invalid.
~~~js
{"Numbers cannot have leading zeroes": 013}
~~~

* `fail27` is valid but was mistakenly deemed invalid.
~~~js
["line
break"]
~~~

* `fail25` is valid but was mistakenly deemed invalid.
~~~js
["	tab	character	in	string	"]
~~~

* `fail04` is valid but was mistakenly deemed invalid.
~~~js
["extra comma",]
~~~


Summary: 122 of 129 are correct.

## 2. Parse Double

* `[18446744073709551616]`
  * expect: `1.8446744073709552e+19 (0x01643F0000000000000)`
  * actual: `-1 (0x016BFF0000000000000)`

* `[-9223372036854775809]`
  * expect: `-9.2233720368547758e+18 (0x016C3E0000000000000)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[10141204801825834086073718800384]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `-1 (0x016BFF0000000000000)`

* `[10141204801825835211973625643008]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `-1 (0x016BFF0000000000000)`

* `[10141204801825834649023672221696]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `-1 (0x016BFF0000000000000)`

* `[5708990770823838890407843763683279797179383808]`
  * expect: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`
  * actual: `-1 (0x016BFF0000000000000)`

* `[5708990770823839524233143877797980545530986496]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `-1 (0x016BFF0000000000000)`

* `[5708990770823839207320493820740630171355185152]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `-1 (0x016BFF0000000000000)`

* `[100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000]`
  * expect: `1e+308 (0x0167FE1CCF385EBC8A0)`
  * actual: `-1 (0x016BFF0000000000000)`


Summary: 57 of 66 are correct.

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
[2.225073858507201e-308]
~~~

~~~js
[2.22507e-308]
~~~

* Fail:
~~~js
[2.2250738585072014e-308]
~~~

~~~js
[2.22507e-308]
~~~

* Fail:
~~~js
[5e-324]
~~~

~~~js
[4.94066e-324]
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
[1.7976931348623157e308]
~~~

~~~js
[1.79769e+308]
~~~


Summary: 21 of 27 are correct.

