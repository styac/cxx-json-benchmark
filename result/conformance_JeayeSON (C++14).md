# Conformance of JeayeSON (C++14)

## 1. Parse Validation

* `y_structure_lonely_true` is valid but was mistakenly deemed invalid.
~~~js
true
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

* `fail21` is valid but was mistakenly deemed invalid.
~~~js
{"Comma instead of colon", null}
~~~

* `fail09` is valid but was mistakenly deemed invalid.
~~~js
{"Extra comma": true,}
~~~

* `fail16` is valid but was mistakenly deemed invalid.
~~~js
[\naked]
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

* `fail24` is valid but was mistakenly deemed invalid.
~~~js
['single quote']
~~~

* `fail08` is valid but was mistakenly deemed invalid.
~~~js
["Extra close"]]
~~~

* `fail12` is valid but was mistakenly deemed invalid.
~~~js
{"Illegal invocation": alert()}
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

* `fail06` is valid but was mistakenly deemed invalid.
~~~js
[   , "<-- missing value"]
~~~

* `fail31` is valid but was mistakenly deemed invalid.
~~~js
[0e+-1]
~~~

* `fail19` is valid but was mistakenly deemed invalid.
~~~js
{"Missing colon" null}
~~~

* `fail05` is valid but was mistakenly deemed invalid.
~~~js
["double extra comma",,]
~~~

* `fail32` is valid but was mistakenly deemed invalid.
~~~js
{"Comma instead if closing brace": true,
~~~

* `fail23` is valid but was mistakenly deemed invalid.
~~~js
["Bad value", truth]
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

* `fail20` is valid but was mistakenly deemed invalid.
~~~js
{"Double colon":: null}
~~~

* `fail15` is valid but was mistakenly deemed invalid.
~~~js
["Illegal backslash escape: \x15"]
~~~


Summary: 90 of 129 are correct.

## 2. Parse Double

* `[3.1416]`
  * expect: `3.1415999999999999 (0x016400921FF2E48E8A7)`
  * actual: `3.1415998935699463 (0x016400921FF20000000)`

* `[1E-10]`
  * expect: `1e-10 (0x0163DDB7CDFD9D7BDBB)`
  * actual: `1.000000013351432e-10 (0x0163DDB7CDFE0000000)`

* `[-1E-10]`
  * expect: `-1e-10 (0x016BDDB7CDFD9D7BDBB)`
  * actual: `-1.000000013351432e-10 (0x016BDDB7CDFE0000000)`

* `[1.234E+10]`
  * expect: `12340000000 (0x0164206FC2BA8000000)`
  * actual: `12339999744 (0x0164206FC2BA0000000)`

* `[1.234E-10]`
  * expect: `1.2340000000000001e-10 (0x0163DE0F5C0635643A8)`
  * actual: `1.2339999855282002e-10 (0x0163DE0F5C060000000)`

* `[1.79769e+308]`
  * expect: `1.7976900000000001e+308 (0x0167FEFFFFC57CA82AE)`
  * actual: `inf (0x0167FF0000000000000)`

* `[2.22507e-308]`
  * expect: `2.2250699999999998e-308 (0x016FFFFE2E8159D0)`
  * actual: `0 (0x0160)`

* `[-1.79769e+308]`
  * expect: `-1.7976900000000001e+308 (0x016FFEFFFFC57CA82AE)`
  * actual: `-inf (0x016FFF0000000000000)`

* `[-2.22507e-308]`
  * expect: `-2.2250699999999998e-308 (0x016800FFFFE2E8159D0)`
  * actual: `-0 (0x0168000000000000000)`

* `[4.9406564584124654e-324]`
  * expect: `4.9406564584124654e-324 (0x0161)`
  * actual: `0 (0x0160)`

* `[2.2250738585072009e-308]`
  * expect: `2.2250738585072009e-308 (0x016FFFFFFFFFFFFF)`
  * actual: `0 (0x0160)`

* `[2.2250738585072014e-308]`
  * expect: `2.2250738585072014e-308 (0x01610000000000000)`
  * actual: `0 (0x0160)`

* `[1.7976931348623157e+308]`
  * expect: `1.7976931348623157e+308 (0x0167FEFFFFFFFFFFFFF)`
  * actual: `inf (0x0167FF0000000000000)`

* `[18446744073709551616]`
  * expect: `1.8446744073709552e+19 (0x01643F0000000000000)`
  * actual: `0 (0x0160)`

* `[-9223372036854775809]`
  * expect: `-9.2233720368547758e+18 (0x016C3E0000000000000)`
  * actual: `0 (0x0160)`

* `[123e34]`
  * expect: `1.23e+36 (0x016476D9C75D3AC072B)`
  * actual: `1.2300000305226882e+36 (0x016476D9C75E0000000)`

* `[45913141877270640000.0]`
  * expect: `4.5913141877270643e+19 (0x0164403E961FA3BA6A0)`
  * actual: `4.5913142669879542e+19 (0x0164403E96200000000)`

* `[2.2250738585072011e-308]`
  * expect: `2.2250738585072009e-308 (0x016FFFFFFFFFFFFF)`
  * actual: `0 (0x0160)`

* `[0.017976931348623157e+310]`
  * expect: `1.7976931348623157e+308 (0x0167FEFFFFFFFFFFFFF)`
  * actual: `inf (0x0167FF0000000000000)`

* `[2.2250738585072012e-308]`
  * expect: `2.2250738585072014e-308 (0x01610000000000000)`
  * actual: `0 (0x0160)`

* `[2.22507385850720113605740979670913197593481954635164564e-308]`
  * expect: `2.2250738585072009e-308 (0x016FFFFFFFFFFFFF)`
  * actual: `0 (0x0160)`

* `[2.22507385850720113605740979670913197593481954635164565e-308]`
  * expect: `2.2250738585072014e-308 (0x01610000000000000)`
  * actual: `0 (0x0160)`

* `[0.999999999999999944488848768742172978818416595458984374]`
  * expect: `0.99999999999999989 (0x0163FEFFFFFFFFFFFFF)`
  * actual: `1 (0x0163FF0000000000000)`

* `[1.00000000000000011102230246251565404236316680908203126]`
  * expect: `1.0000000000000002 (0x0163FF0000000000001)`
  * actual: `1 (0x0163FF0000000000000)`

* `[72057594037927928.0]`
  * expect: `72057594037927928 (0x016436FFFFFFFFFFFFF)`
  * actual: `72057594037927936 (0x0164370000000000000)`

* `[7205759403792793199999e-5]`
  * expect: `72057594037927928 (0x016436FFFFFFFFFFFFF)`
  * actual: `72057594037927936 (0x0164370000000000000)`

* `[9223372036854774784.0]`
  * expect: `9.2233720368547748e+18 (0x01643DFFFFFFFFFFFFF)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[922337203685477529599999e-5]`
  * expect: `9.2233720368547748e+18 (0x01643DFFFFFFFFFFFFF)`
  * actual: `9.2233720368547758e+18 (0x01643E0000000000000)`

* `[10141204801825834086073718800384]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `0 (0x0160)`

* `[10141204801825835211973625643008]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `0 (0x0160)`

* `[10141204801825834649023672221696]`
  * expect: `1.0141204801825835e+31 (0x0164660000000000000)`
  * actual: `0 (0x0160)`

* `[1014120480182583464902367222169599999e-5]`
  * expect: `1.0141204801825834e+31 (0x016465FFFFFFFFFFFFF)`
  * actual: `1.0141204801825835e+31 (0x0164660000000000000)`

* `[5708990770823838890407843763683279797179383808]`
  * expect: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`
  * actual: `0 (0x0160)`

* `[5708990770823839524233143877797980545530986496]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `0 (0x0160)`

* `[5708990770823839207320493820740630171355185152]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `0 (0x0160)`

* `[5708990770823839207320493820740630171355185151999e-3]`
  * expect: `5.7089907708238389e+45 (0x016496FFFFFFFFFFFFF)`
  * actual: `inf (0x0167FF0000000000000)`

* `[5708990770823839207320493820740630171355185152001e-3]`
  * expect: `5.7089907708238395e+45 (0x0164970000000000000)`
  * actual: `inf (0x0167FF0000000000000)`

* `[100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000]`
  * expect: `1e+308 (0x0167FE1CCF385EBC8A0)`
  * actual: `0 (0x0160)`

* `[2.225073858507201136057409796709131975934819546351645648023426109724822222021076945516529523908135087914149158913039621106870086438694594645527657207407820621743379988141063267329253552286881372149012981122451451889849057222307285255133155755015914397476397983411801999323962548289017107081850690630666655994938275772572015763062690663332647565300009245888316433037779791869612049497390377829704905051080609940730262937128958950003583799967207254304360284078895771796150945516748243471030702609144621572289880258182545180325707018860872113128079512233426288368622321503775666622503982534335974568884423900265498198385487948292206894721689831099698365846814022854243330660339850886445804001034933970427567186443383770486037861622771738545623065874679014086723327636718751234567890123456789012345678901e-308]`
  * expect: `2.2250738585072014e-308 (0x01610000000000000)`
  * actual: `0 (0x0160)`


Summary: 27 of 66 are correct.

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
[9223372036854775807]
~~~

~~~js
[-1]
~~~

* Fail:
~~~js
[2.225073858507201e-308]
~~~

~~~js
[0]
~~~

* Fail:
~~~js
[-1234567890123456789]
~~~

~~~js
[-2112454933]
~~~

* Fail:
~~~js
[2.2250738585072014e-308]
~~~

~~~js
[0]
~~~

* Fail:
~~~js
[5e-324]
~~~

~~~js
[0]
~~~

* Fail:
~~~js
[-9223372036854775808]
~~~

~~~js
[0]
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
[-1]
~~~

* Fail:
~~~js
[1234567890123456789]
~~~

~~~js
[2112454933]
~~~

* Fail:
~~~js
[1.7976931348623157e308]
~~~

~~~js
[inf]
~~~


Summary: 16 of 27 are correct.

