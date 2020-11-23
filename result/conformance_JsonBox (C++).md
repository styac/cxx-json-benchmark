# Conformance of JsonBox (C++)

## 1. Parse Validation

* `fail21` is valid but was mistakenly deemed invalid.
~~~js
{"Comma instead of colon", null}
~~~

* `fail09` is valid but was mistakenly deemed invalid.
~~~js
{"Extra comma": true,}
~~~

* `fail30` is valid but was mistakenly deemed invalid.
~~~js
[0e+]
~~~

* `fail28` is valid but was mistakenly deemed invalid.
~~~js
["line\
break"]
~~~

* `fail08` is valid but was mistakenly deemed invalid.
~~~js
["Extra close"]]
~~~

* `fail13` is valid but was mistakenly deemed invalid.
~~~js
{"Numbers cannot have leading zeroes": 013}
~~~

* `fail33` is valid but was mistakenly deemed invalid.
~~~js
["mismatch"}
~~~

* `fail03` is valid but was mistakenly deemed invalid.
~~~js
{unquoted_key: "keys must be quoted"}
~~~

* `fail27` is valid but was mistakenly deemed invalid.
~~~js
["line
break"]
~~~

* `fail29` is valid but was mistakenly deemed invalid.
~~~js
[0e]
~~~

* `fail07` is valid but was mistakenly deemed invalid.
~~~js
["Comma after the close"],
~~~

* `fail26` is valid but was mistakenly deemed invalid.
~~~js
["tab\   character\   in\  string\  "]
~~~

* `fail31` is valid but was mistakenly deemed invalid.
~~~js
[0e+-1]
~~~

* `fail19` is valid but was mistakenly deemed invalid.
~~~js
{"Missing colon" null}
~~~

* `fail32` is valid but was mistakenly deemed invalid.
~~~js
{"Comma instead if closing brace": true,
~~~

* `fail22` is valid but was mistakenly deemed invalid.
~~~js
["Colon instead of comma": false]
~~~

* `fail25` is valid but was mistakenly deemed invalid.
~~~js
["	tab	character	in	string	"]
~~~

* `fail11` is valid but was mistakenly deemed invalid.
~~~js
{"Illegal expression": 1 + 2}
~~~

* `fail04` is valid but was mistakenly deemed invalid.
~~~js
["extra comma",]
~~~

* `fail02` is valid but was mistakenly deemed invalid.
~~~js
["Unclosed array"
~~~

* `fail10` is valid but was mistakenly deemed invalid.
~~~js
{"Extra value after close": true} "misplaced quoted value"
~~~

* `fail17` is valid but was mistakenly deemed invalid.
~~~js
["Illegal backslash escape: \017"]
~~~

* `fail14` is valid but was mistakenly deemed invalid.
~~~js
{"Numbers cannot be hex": 0x14}
~~~

* `fail15` is valid but was mistakenly deemed invalid.
~~~js
["Illegal backslash escape: \x15"]
~~~


Summary: 105 of 129 are correct.

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
[9223372036854775807]
~~~

~~~js
[2147483647]
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
[-1234567890123456789]
~~~

~~~js
[-2147483648]
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
[-9223372036854775808]
~~~

~~~js
[-2147483648]
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
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157e+308]
~~~


Summary: 15 of 27 are correct.

