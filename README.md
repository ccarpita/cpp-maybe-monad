# Monads in C++

A small discrete implementation of a Maybe Monad in C++.  Other monads such as Promises to come!

# Usage

Copy `monads.h` to somewhere in your local path or library search path.  Note that it currently doesn't namespace its types (`Maybe` and `MaybeMonad`) so you'll need to use a forwarding header if you need them to be namespaced.

Example:

```cpp

#include <string>

#include 'monads.h'

std::string getAnswer(const std::string &input) {
  int len;
  Maybe<std::string> maybe_val =
    MaybeMonad<std::string>(input)
      .bind([&len](const std::string &str) { len = str.length(); return str; })
      .predicate([](const std::string &str) { return str.length() > 12; })
      .bind([](const std::string &str) { return str + "\nYour Answer is: "; })
      .bind([](const std::string &str) { return str + "42"; })
      .extract();
  return maybe_val.value_or("There is No Answer");
}
```

See `test.cpp` for more usage examples.

# Learnings

Lambdas as arguments are hard to disambiguate by the compiler, even when return types are explicit.  `std::function` is generous with up-casting given non-explicit copy-constructor conversion, so separate methods were required for the interface (`bind` and `predicate`) rather than relying simply on the return type.  Given that booleans could be used as a contained type, having separate methods was likely desirable anyhow.

# License

MIT
