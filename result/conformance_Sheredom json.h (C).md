# Conformance of Sheredom json.h (C)

## 1. Parse Validation

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail25.json` is invalid but was mistakenly deemed valid.
~~~js
["	tab	character	in	string	"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail29.json` is invalid but was mistakenly deemed valid.
~~~js
[0e]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail30.json` is invalid but was mistakenly deemed valid.
~~~js
[0e+]
~~~

* `string_2_escaped_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["\uD800\uD800"]
~~~

* `string_3_escaped_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["\uD800\uD800\uD800"]
~~~

* `string_1_escaped_invalid_codepoint` is valid but was mistakenly deemed invalid.
~~~js
["\uD800"]
~~~


Summary: 145 of 151 are correct.

## 2. Parse Double


Summary: 66 of 66 are correct.

## 3. Parse String

* `["Hello\u0000World"]`
  * expect: `"Hello\0World"` (length: 11)
  * actual: `"Hello"` (length: 5)


Summary: 8 of 9 are correct.

## 4. Roundtrip


Summary: 27 of 27 are correct.

