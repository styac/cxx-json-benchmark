# Conformance of C++ REST SDK (C++11)

## 1. Parse Validation

* `fail28` is valid but was mistakenly deemed invalid.
~~~js
["line\
break"]
~~~

* `fail13` is valid but was mistakenly deemed invalid.
~~~js
{"Numbers cannot have leading zeroes": 013}
~~~

* `fail26` is valid but was mistakenly deemed invalid.
~~~js
["tab\   character\   in\  string\  "]
~~~

* `fail17` is valid but was mistakenly deemed invalid.
~~~js
["Illegal backslash escape: \017"]
~~~

* `fail15` is valid but was mistakenly deemed invalid.
~~~js
["Illegal backslash escape: \x15"]
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


Summary: 143 of 151 are correct.

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
[5e-324]
~~~

~~~js
[4.9406564584124654e-324]
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
[1.7976931348623157e308]
~~~

~~~js
[1.7976931348623157e+308]
~~~


Summary: 20 of 27 are correct.

