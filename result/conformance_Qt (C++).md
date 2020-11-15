# Conformance of Qt (C++)

## 1. Parse Validation

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail15.json` is invalid but was mistakenly deemed valid.
~~~js
["Illegal backslash escape: \x15"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail17.json` is invalid but was mistakenly deemed valid.
~~~js
["Illegal backslash escape: \017"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail25.json` is invalid but was mistakenly deemed valid.
~~~js
["	tab	character	in	string	"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail26.json` is invalid but was mistakenly deemed valid.
~~~js
["tab\   character\   in\  string\  "]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail27.json` is invalid but was mistakenly deemed valid.
~~~js
["line
break"]
~~~

* `/home/collection/sw/Json/cxx-json-benchmark/data/jsonchecker/fail28.json` is invalid but was mistakenly deemed valid.
~~~js
["line\
break"]
~~~

* `string_1_invalid_codepoint` is valid but was mistakenly deemed invalid.
~~~js
["í €"]
~~~

* `string_3_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["í €í €í €"]
~~~

* `string_2_invalid_codepoints` is valid but was mistakenly deemed invalid.
~~~js
["í €í €"]
~~~

* `number_1e-999` is valid but was mistakenly deemed invalid.
~~~js
[1E-999]

~~~

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


Summary: 133 of 151 are correct.

## 2. Parse Double


Summary: 66 of 66 are correct.

## 3. Parse String


Summary: 9 of 9 are correct.

## 4. Roundtrip

* Fail:
~~~js
[-1234567890123456789]
~~~

~~~js
[-1234567890123456800]
~~~

* Fail:
~~~js
[-9223372036854775808]
~~~

~~~js
[-9223372036854776000]
~~~

* Fail:
~~~js
[1234567890123456789]
~~~

~~~js
[1234567890123456800]
~~~

* Fail:
~~~js
[9223372036854775807]
~~~

~~~js
[9223372036854776000]
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
[-0.0]
~~~

~~~js
[0]
~~~

* Fail:
~~~js
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157e+308]
~~~


Summary: 20 of 27 are correct.

