# Conformance of boostjson (C++11)

## 1. Parse Validation

* `string_2_escaped_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["\uD800\uD800"]
~~~

* `string_1_invalid_codepoint` is valid but was mistakenly deemed invalid.
~~~js
["í €"]
~~~

* `string_3_escaped_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["\uD800\uD800\uD800"]
~~~

* `string_3_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["í €í €í €"]
~~~

* `string_2_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["í €í €"]
~~~

* `string_1_escaped_invalid_codepoint` is valid but was mistakenly deemed invalid.
~~~js
["\uD800"]
~~~


Summary: 145 of 151 are correct.

## 2. Parse Double

* `[2.22507e-308]`
  * expect: `2.2250699999999998e-308 (0x016FFFFE2E8159D0)`
  * actual: `2.2250700000000003e-308 (0x016FFFFE2E8159D1)`

* `[-2.22507e-308]`
  * expect: `-2.2250699999999998e-308 (0x016800FFFFE2E8159D0)`
  * actual: `-2.2250700000000003e-308 (0x016800FFFFE2E8159D1)`

* `[0.9868011474609375]`
  * expect: `0.9868011474609375 (0x0163FEF93E000000000)`
  * actual: `0.98680114746093761 (0x0163FEF93E000000001)`

* `[2.2250738585072011e-308]`
  * expect: `2.2250738585072009e-308 (0x016FFFFFFFFFFFFF)`
  * actual: `2.2250738585072014e-308 (0x01610000000000000)`

* `[2.22507385850720113605740979670913197593481954635164564e-308]`
  * expect: `2.2250738585072009e-308 (0x016FFFFFFFFFFFFF)`
  * actual: `2.2250738585072014e-308 (0x01610000000000000)`

* `[0.999999999999999944488848768742172978818416595458984374]`
  * expect: `0.99999999999999989 (0x0163FEFFFFFFFFFFFFF)`
  * actual: `1 (0x0163FF0000000000000)`

* `[1.00000000000000011102230246251565404236316680908203126]`
  * expect: `1.0000000000000002 (0x0163FF0000000000001)`
  * actual: `1 (0x0163FF0000000000000)`

* `[7205759403792793199999e-5]`
  * expect: `72057594037927928 (0x016436FFFFFFFFFFFFF)`
  * actual: `72057594037927936 (0x0164370000000000000)`

* `[10141204801825834086073718800384]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `1.0141204801825835e+31 (0x0164660000000000000)`

* `[1014120480182583464902367222169599999e-5]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `1.0141204801825835e+31 (0x0164660000000000000)`

* `[5708990770823839207320493820740630171355185152]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`

* `[5708990770823839207320493820740630171355185152001e-3]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`


Summary: 54 of 66 are correct.

## 3. Parse String


Summary: 9 of 9 are correct.

## 4. Roundtrip

* Fail:
~~~js
[0.0]
~~~

~~~js
[0E0]
~~~

* Fail:
~~~js
[-0.0]
~~~

~~~js
[-0E0]
~~~

* Fail:
~~~js
[1.2345]
~~~

~~~js
[1.2345000000000002E0]
~~~

* Fail:
~~~js
[-1.2345]
~~~

~~~js
[-1.2345000000000002E0]
~~~

* Fail:
~~~js
[5e-324]
~~~

~~~js
[5E-324]
~~~

* Fail:
~~~js
[2.225073858507201e-308]
~~~

~~~js
[2.2250738585072014E-308]
~~~

* Fail:
~~~js
[2.2250738585072014e-308]
~~~

~~~js
[2.2250738585072014E-308]
~~~

* Fail:
~~~js
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157E308]
~~~


Summary: 19 of 27 are correct.

