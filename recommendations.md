# Recommendations

Below are some preferences regarding coding practices. They are opinionated but supported with practical experience. Not only in C++.

## Work environment
- use package manager
- use precompiled headers
- use sanitizers when errors are difficult to spot
- develop and test in debug mode
- in the editor enable show white spaces option
- formalize expected code formatting [style](.clang-format)
- backup settings and config files in the cloud

## Coding practices
- const qualify everything you can
- prefer <, <= to >, >= 
- prefer signed types to unsigned
- prefer references to pointers
- use constinit const and not macros
- index everything starting from 0 and not 1
- prefer half open ranges in algorithm implementation [..)
- do not shadow variable names
- keep the scope of local variables minimal
- carefully choose names you declare, make them descriptive
- prefer STL algorithms to raw loops
- know the limit of integral exact representation in your float type
- if you catch exception, do not just silence it
- use almost always auto, but remember: almost
- prefer std:: containers to GUI framework provided ones
- prefer `std::span` or `std::string_view` to raw pointer to buffers

## Performance
- believe no one, measure!
- use exceptions sparingly
- prefer preincrement ++i to postincrement i++
- prefer stack to heap – whenever reasonable
- reuse containers to reduce dynamic allocation
- have CPU pipelining in mind: ++i, --z will execute almost at one cycle
- precompute constant primes, powers, e.g. `{1, 10, 100, 1000, ..}`
- prefer explicit computation over `<cmath>` for integrals
>- `(1 << 7)` better then `std::pow(2, 7)`
>- `a / b` better then `std::floor(float(a) / b)`
>- `(a + b – 1) / b` better then `std::ceil(float(a) / b)`
- avoid overflows: prefer `a + (b – a) / 2` to `(a + b) / 2`
- use streams for in-loop std::strings manipulations
- allocate memory to reduce cache misses (row major vs column major)
- use modules or pimpl for large enough class implementations 










