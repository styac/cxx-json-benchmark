# Conformance of Qt (C++)

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

* `fail28` is valid but was mistakenly deemed invalid.
~~~js
["line\
break"]
~~~

* `fail27` is valid but was mistakenly deemed invalid.
~~~js
["line
break"]
~~~

* `fail26` is valid but was mistakenly deemed invalid.
~~~js
["tab\   character\   in\  string\  "]
~~~

* `fail25` is valid but was mistakenly deemed invalid.
~~~js
["	tab	character	in	string	"]
~~~

* `fail17` is valid but was mistakenly deemed invalid.
~~~js
["Illegal backslash escape: \017"]
~~~

* `fail15` is valid but was mistakenly deemed invalid.
~~~js
["Illegal backslash escape: \x15"]
~~~


Summary: 115 of 129 are correct.

## 2. Parse Double


Summary: 66 of 66 are correct.

## 3. Parse String


Summary: 9 of 9 are correct.

## 4. Roundtrip

* Fail:
~~~js
[-0.0]
~~~

~~~js
[0]
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
[0.0]
~~~

~~~js
[0]
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
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157e+308]
~~~


Summary: 20 of 27 are correct.

