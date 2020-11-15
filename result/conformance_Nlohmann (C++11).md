# Conformance of Nlohmann (C++11)

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


Summary: 66 of 66 are correct.

## 3. Parse String


Summary: 9 of 9 are correct.

## 4. Roundtrip

* Fail:
~~~js
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157e+308]
~~~


Summary: 26 of 27 are correct.

