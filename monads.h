#pragma once

#include <functional>
#include <memory>
#include <stdexcept>

/**
 * A generic Maybe container for any value, using similar operational semantics to std::unique_ptr,
 * which is incidentally how the optional data is implemented.
 */
template<typename T>
class Maybe {
 public:
  Maybe() {}
  Maybe(const Maybe<T> &init) : _rep(init ? new T(*init) : nullptr) {}
  Maybe(const T &init) : _rep(new T(init)) {}

  Maybe& operator=(const Maybe<T> &init) {
    _rep = init ? new T(*init) : nullptr;
  }

  /**
   * Get the raw value of the Maybe container. Throws a std::exception if
   * the value is empty.
   *
   * @Example
   *   auto maybe = Maybe(5);
   *   assert(maybe);
   *   assert(*maybe == 5);
   */
  T operator*() const {
    if (!_rep) {
      throw std::runtime_error("Attempted to get value of empty Maybe container");
    }
    return *_rep;
  }

  /**
   * Returns true in a boolean context if the container has a value.
   *
   * @Example
   *   auto maybe = Maybe<int>();
   *   assert(!maybe);
   *   auto maybe_val = Maybe<std::string>("");
   *   assert(maybe_val);
   */
  operator bool() const {
    return !!_rep;
  }

  /**
   * Check Maybe containers for equality, which will evaluate to true
   * if both are empty, or both are non-empty and their values are
   * also equal.
   */
  bool operator==(const Maybe<T> &v) const {
    return (!_rep && !v) ? true : ((!_rep || !v) ? false : *_rep == *v);
  }

 private:
  std::unique_ptr<T> _rep;
};

/**
 * An implementation of Maybe Monad pattern implementing cast and bind.
 */
template<typename T>
class MaybeMonad {
 public:

  /**
   * When constructed with an empty value, the MaybeMonad will already be
   * in an empty/failed state and no bound functions will execute.
   */
  MaybeMonad() {}

  /**
   * Construct a MaybeMonad with an initial non-empty value.
   */
  explicit MaybeMonad(const T &init) : _rep(init) {}

  /**
   * Construct a MaybeMonad from an existing Maybe, which will copy-construct
   * the internal representation.
   */
  explicit MaybeMonad(const Maybe<T> &init) : _rep(init) {}

  /**
   * Calls a function (T -> Maybe<T>) if there is a non-empty value and returns
   * a MaybeMonad<T> for the result of that function or the empty non-result
   */
  MaybeMonad<T> bind(const std::function<Maybe<T>(const T &rep)>& func) {
    return _rep ? MaybeMonad<T>(func(*_rep)) : MaybeMonad<T>();
  }

  /**
   * Convenience method calling a function (T -> bool) if the contained
   * Monad<T> is non-empty, and returns a MaybeMonad<T> which will be empty
   * if the Monad<T> was already empty, or the passed function returns false.
   */
  MaybeMonad<T> predicate(const std::function<bool(const T &rep)>& func) {
    return _rep && func(*_rep) ? MaybeMonad<T>(_rep) : MaybeMonad<T>();
  }

  /**
   * Returns the Monadic result of the current operation.  This is generally
   * called at the end of a chain of binds and predicates.
   */
  Maybe<T> extract() { return _rep; }

 private:
  Maybe<T> _rep;
};
