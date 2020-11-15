# Conformance of Configuru (C++11)

## 1. Parse Validation

* `string_2_escaped_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["\uD800\uD800"]
~~~

* `object_same_key_unclear_values` is valid but was mistakenly deemed invalid.
~~~js
{"a":0, "a":-0}

~~~

* `string_3_escaped_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["\uD800\uD800\uD800"]
~~~

* `object_same_key_different_values` is valid but was mistakenly deemed invalid.
~~~js
{"a":1,"a":2}
~~~

* `object_same_key_same_value` is valid but was mistakenly deemed invalid.
~~~js
{"a":1,"a":1}
~~~

* `string_1_escaped_invalid_codepoint` is valid but was mistakenly deemed invalid.
~~~js
["\uD800"]
~~~

* `y_object_duplicated_key_and_value` is valid but was mistakenly deemed invalid.
~~~js
{"a":"b","a":"b"}
~~~

* `y_object_duplicated_key` is valid but was mistakenly deemed invalid.
~~~js
{"a":"b","a":"c"}
~~~


Summary: 143 of 151 are correct.

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

