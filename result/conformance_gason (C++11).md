# Conformance of gason (C++11)

## 1. Parse Validation

* `fail09` is valid but was mistakenly deemed invalid.
~~~js
{"Extra comma": true,}
~~~

* `fail30` is valid but was mistakenly deemed invalid.
~~~js
[0e+]
~~~

* `fail08` is valid but was mistakenly deemed invalid.
~~~js
["Extra close"]]
~~~

* `fail13` is valid but was mistakenly deemed invalid.
~~~js
{"Numbers cannot have leading zeroes": 013}
~~~

* `fail29` is valid but was mistakenly deemed invalid.
~~~js
[0e]
~~~

* `fail07` is valid but was mistakenly deemed invalid.
~~~js
["Comma after the close"],
~~~

* `fail19` is valid but was mistakenly deemed invalid.
~~~js
{"Missing colon" null}
~~~

* `fail04` is valid but was mistakenly deemed invalid.
~~~js
["extra comma",]
~~~

* `fail10` is valid but was mistakenly deemed invalid.
~~~js
{"Extra value after close": true} "misplaced quoted value"
~~~

* `y_string_with_del_character` is valid but was mistakenly deemed invalid.
~~~js
["aa"]
~~~

* `y_string_unescaped_char_delete` is valid but was mistakenly deemed invalid.
~~~js
[""]
~~~


Summary: 140 of 151 are correct.

## 2. Parse Double

* `[1E-10]`
  * expect: `1e-10 (0x0163DDB7CDFD9D7BDBB)`
  * actual: `1.0000000000000011e-10 (0x0163DDB7CDFD9D7BDC3)`

* `[-1E-10]`
  * expect: `-1e-10 (0x016BDDB7CDFD9D7BDBB)`
  * actual: `-1.0000000000000011e-10 (0x016BDDB7CDFD9D7BDC3)`

* `[1.234E-10]`
  * expect: `1.2340000000000001e-10 (0x0163DE0F5C0635643A8)`
  * actual: `1.2340000000000014e-10 (0x0163DE0F5C0635643AD)`

* `[1.79769e+308]`
  * expect: `1.7976900000000001e+308 (0x0167FEFFFFC57CA82AE)`
  * actual: `1.7976900000000009e+308 (0x0167FEFFFFC57CA82B2)`

* `[2.22507e-308]`
  * expect: `2.2250699999999998e-308 (0x016FFFFE2E8159D0)`
  * actual: `2.2250700000000734e-308 (0x016FFFFE2E815A65)`

* `[-1.79769e+308]`
  * expect: `-1.7976900000000001e+308 (0x016FFEFFFFC57CA82AE)`
  * actual: `-1.7976900000000009e+308 (0x016FFEFFFFC57CA82B2)`

* `[-2.22507e-308]`
  * expect: `-2.2250699999999998e-308 (0x016800FFFFE2E8159D0)`
  * actual: `-2.2250700000000734e-308 (0x016800FFFFE2E815A65)`

* `[4.9406564584124654e-324]`
  * expect: `4.9406564584124654e-324 (0x0161)`
  * actual: `0 (0x0160)`

* `[2.2250738585072009e-308]`
  * expect: `2.2250738585072009e-308 (0x016FFFFFFFFFFFFF)`
  * actual: `2.225073858507275e-308 (0x01610000000000095)`

* `[2.2250738585072014e-308]`
  * expect: `2.2250738585072014e-308 (0x01610000000000000)`
  * actual: `2.2250738585072755e-308 (0x01610000000000096)`

* `[1.7976931348623157e+308]`
  * expect: `1.7976931348623157e+308 (0x0167FEFFFFFFFFFFFFF)`
  * actual: `inf (0x0167FF0000000000000)`

* `[-9223372036854775809]`
  * expect: `-9.2233720368547758e+18 (0x016C3E0000000000000)`
  * actual: `-9.2233720368547779e+18 (0x016C3E0000000000001)`

* `[123e34]`
  * expect: `1.23e+36 (0x016476D9C75D3AC072B)`
  * actual: `1.2300000000000001e+36 (0x016476D9C75D3AC072C)`

* `[2.2250738585072011e-308]`
  * expect: `2.2250738585072009e-308 (0x016FFFFFFFFFFFFF)`
  * actual: `2.225073858507275e-308 (0x01610000000000095)`

* `[0.017976931348623157e+310]`
  * expect: `1.7976931348623157e+308 (0x0167FEFFFFFFFFFFFFF)`
  * actual: `inf (0x0167FF0000000000000)`

* `[2.2250738585072012e-308]`
  * expect: `2.2250738585072014e-308 (0x01610000000000000)`
  * actual: `2.225073858507275e-308 (0x01610000000000095)`

* `[2.22507385850720113605740979670913197593481954635164564e-308]`
  * expect: `2.2250738585072009e-308 (0x016FFFFFFFFFFFFF)`
  * actual: `2.225073858507275e-308 (0x01610000000000095)`

* `[2.22507385850720113605740979670913197593481954635164565e-308]`
  * expect: `2.2250738585072014e-308 (0x01610000000000000)`
  * actual: `2.225073858507275e-308 (0x01610000000000095)`

* `[0.999999999999999944488848768742172978818416595458984375]`
  * expect: `1 (0x0163FF0000000000000)`
  * actual: `0.99999999999999989 (0x0163FEFFFFFFFFFFFFF)`

* `[0.999999999999999944488848768742172978818416595458984376]`
  * expect: `1 (0x0163FF0000000000000)`
  * actual: `0.99999999999999989 (0x0163FEFFFFFFFFFFFFF)`

* `[1.00000000000000011102230246251565404236316680908203126]`
  * expect: `1.0000000000000002 (0x0163FF0000000000001)`
  * actual: `1 (0x0163FF0000000000000)`

* `[7205759403792793199999e-5]`
  * expect: `72057594037927928 (0x016436FFFFFFFFFFFFF)`
  * actual: `72057594037927968 (0x0164370000000000002)`

* `[7205759403792793200001e-5]`
  * expect: `72057594037927936 (0x0164370000000000000)`
  * actual: `72057594037927984 (0x0164370000000000003)`

* `[9223372036854775808.0]`
  * expect: `9.2233720368547758e+18 (0x01643E0000000000000)`
  * actual: `9.2233720368547779e+18 (0x01643E0000000000001)`

* `[922337203685477529599999e-5]`
  * expect: `9.2233720368547748e+18 (0x01643DFFFFFFFFFFFFF)`
  * actual: `9.223372036854782e+18 (0x01643E0000000000003)`

* `[922337203685477529600001e-5]`
  * expect: `9.2233720368547758e+18 (0x01643E0000000000000)`
  * actual: `9.223372036854782e+18 (0x01643E0000000000003)`

* `[10141204801825835211973625643008]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `1.0141204801825842e+31 (0x0164660000000000003)`

* `[10141204801825834649023672221696]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`

* `[1014120480182583464902367222169599999e-5]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `1.014120480182584e+31 (0x0164660000000000002)`

* `[1014120480182583464902367222169600001e-5]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `1.014120480182584e+31 (0x0164660000000000002)`

* `[5708990770823838890407843763683279797179383808]`
  * expect: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`
  * actual: `5.7089907708238433e+45 (0x0164970000000000003)`

* `[5708990770823839524233143877797980545530986496]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `5.7089907708238433e+45 (0x0164970000000000003)`

* `[5708990770823839207320493820740630171355185152]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `5.7089907708238433e+45 (0x0164970000000000003)`

* `[5708990770823839207320493820740630171355185151999e-3]`
  * expect: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`
  * actual: `5.7089907708238446e+45 (0x0164970000000000004)`

* `[5708990770823839207320493820740630171355185152001e-3]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `5.7089907708238446e+45 (0x0164970000000000004)`

* `[100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000]`
  * expect: `1e+308 (0x0167FE1CCF385EBC8A0)`
  * actual: `9.9999999999999981e+307 (0x0167FE1CCF385EBC89F)`

* `[2.225073858507201136057409796709131975934819546351645648023426109724822222021076945516529523908135087914149158913039621106870086438694594645527657207407820621743379988141063267329253552286881372149012981122451451889849057222307285255133155755015914397476397983411801999323962548289017107081850690630666655994938275772572015763062690663332647565300009245888316433037779791869612049497390377829704905051080609940730262937128958950003583799967207254304360284078895771796150945516748243471030702609144621572289880258182545180325707018860872113128079512233426288368622321503775666622503982534335974568884423900265498198385487948292206894721689831099698365846814022854243330660339850886445804001034933970427567186443383770486037861622771738545623065874679014086723327636718751234567890123456789012345678901e-308]`
  * expect: `2.2250738585072014e-308 (0x01610000000000000)`
  * actual: `2.225073858507275e-308 (0x01610000000000095)`


Summary: 29 of 66 are correct.

## 3. Parse String

* `["Hello\u0000World"]`
  * expect: `"Hello\0World"` (length: 11)
  * actual: `"Hello"` (length: 5)

* `["\uD834\uDD1E"]`
  * expect: `"𝄞"` (length: 4)
  * actual: `"������"` (length: 6)


Summary: 7 of 9 are correct.

## 4. Roundtrip

* Fail:
~~~js
[-0.0]
~~~

~~~js
[-0.000000]
~~~

* Fail:
~~~js
[1.2345]
~~~

~~~js
[1.234500]
~~~

* Fail:
~~~js
[0]
~~~

~~~js
[0.000000]
~~~

* Fail:
~~~js
[9223372036854775807]
~~~

~~~js
[9223372036854777856.000000]
~~~

* Fail:
~~~js
[0,1]
~~~

~~~js
[0.000000,1.000000]
~~~

* Fail:
~~~js
[2.225073858507201e-308]
~~~

~~~js
[0.000000]
~~~

* Fail:
~~~js
[-1.2345]
~~~

~~~js
[-1.234500]
~~~

* Fail:
~~~js
[-1234567890123456789]
~~~

~~~js
[-1234567890123457024.000000]
~~~

* Fail:
~~~js
[-1]
~~~

~~~js
[-1.000000]
~~~

* Fail:
~~~js
[-2147483648]
~~~

~~~js
[-2147483648.000000]
~~~

* Fail:
~~~js
[2.2250738585072014e-308]
~~~

~~~js
[0.000000]
~~~

* Fail:
~~~js
[5e-324]
~~~

~~~js
[0.000000]
~~~

* Fail:
~~~js
[-9223372036854775808]
~~~

~~~js
[-9223372036854777856.000000]
~~~

* Fail:
~~~js
[0.0]
~~~

~~~js
[0.000000]
~~~

* Fail:
~~~js
[2147483647]
~~~

~~~js
[2147483647.000000]
~~~

* Fail:
~~~js
[4294967295]
~~~

~~~js
[4294967295.000000]
~~~

* Fail:
~~~js
[1]
~~~

~~~js
[1.000000]
~~~

* Fail:
~~~js
[1234567890123456789]
~~~

~~~js
[1234567890123457024.000000]
~~~

* Fail:
~~~js
[1.7976931348623157e308]
~~~

~~~js
[inf]
~~~


Summary: 8 of 27 are correct.
