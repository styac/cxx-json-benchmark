# Conformance of JsonCpp (C++)

## 1. Parse Validation

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail13.json` is invalid but was mistakenly deemed valid.
~~~js
{"Numbers cannot have leading zeroes": 013}
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

* `y_structure_lonely_true` is valid but was mistakenly deemed invalid.
~~~js
true
~~~

* `y_object_duplicated_key` is valid but was mistakenly deemed invalid.
~~~js
{"a":"b","a":"c"}
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


Summary: 133 of 151 are correct.

## 2. Parse Double


Summary: 66 of 66 are correct.

## 3. Parse String


Summary: 9 of 9 are correct.

## 4. Roundtrip

* Fail:
~~~js
[1.2345]
~~~

~~~js
[1.2344999999999999]
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
[5e-324]
~~~

~~~js
[4.9406564584124654e-324]
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
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157e+308]
~~~


Summary: 22 of 27 are correct.

