# Conformance of yyjson (C)

## 1. Parse Validation


Summary: 129 of 129 are correct.

## 2. Parse Double


Summary: 66 of 66 are correct.

## 3. Parse String

* `["Hello\u0000World"]`
  * expect: `"Hello\0World"` (length: 11)
  * actual: `"Hello"` (length: 5)


Summary: 8 of 9 are correct.

## 4. Roundtrip


Summary: 27 of 27 are correct.

