# Maybe Monads in C++

Some workable experimentation with Maybe Monads in C++

# Learnings

Lambdas as arguments are hard to disambiguate by the compiler, even when return types are explicit.  `std::function` is quite generous with copy-constructor conversion for up-casting, so separate methods were required for the interface: `bind`, `predicate`, and `maybe`

# Usage

See `test.cpp` for usage examples.
