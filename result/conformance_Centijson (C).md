# Conformance of Centijson (C)

## 1. Parse Validation

* `y_object_duplicated_key_and_value` is valid but was mistakenly deemed invalid.
~~~js
{"a":"b","a":"b"}
~~~

* `y_string_accepted_surrogate_pairs` is valid but was mistakenly deemed invalid.
~~~js
["\ud83d\ude39\ud83d\udc8d"]
~~~

* `y_object_duplicated_key` is valid but was mistakenly deemed invalid.
~~~js
{"a":"b","a":"c"}
~~~


Summary: 126 of 129 are correct.

## 2. Parse Double


Summary: 66 of 66 are correct.

## 3. Parse String


Summary: 9 of 9 are correct.

## 4. Roundtrip

* Fail:
~~~js
[2.2250738585072014e-308]
~~~

~~~js
[2.225073858507201e-308]
~~~

* Fail:
~~~js
[5e-324]
~~~

~~~js
[4.940656458412465e-324]
~~~

* Fail:
~~~js
[1.7976931348623157e308]
~~~

~~~js
[1.797693134862316e+308]
~~~


Summary: 24 of 27 are correct.

