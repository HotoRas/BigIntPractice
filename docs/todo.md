# TODO

## Refactor
- Type: `struct BigInt` -> `class bigint_t`
- `#define BI bigint_t` and `#undef BI` at the end
- Split header file (`bigint.hpp`: rework needed)
- Storage: `std::vector<int> a` -> `std::vector<int64_t> _num`
- `base` into private const internal value, make it from `(int)1e9` to `(int64_t)1e21`
  + `int base_digits` into private const internal value, from `9` to `21`

## Implement
- Using `std::vector<int64_t>` internally and messing up with `base 10` not `base 2`,
but implementing bitshift `operator<<(bigint_t&, int)` and `operator>>(bigint_t&, int)` would be nice. 
(will just return `*this *= 2` or `*this /= 2` but whatever)