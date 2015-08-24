#include "monads.h"
#include "assert.h"

#include <string>
#include <iostream>

static std::string CURRENT_TEST = "";
#define RUN_TEST(TESTNAME) CURRENT_TEST = #TESTNAME; std::cout << "Running: " << #TESTNAME << "\n"; TESTNAME();
#define ASSERT(VAL, MESG) if (!(VAL)) { std::cout << "[" << CURRENT_TEST << "] " << "Assertion failed: " << #VAL << " -- " << MESG << "\n"; assert(VAL); }

void testMaybe() {
  Maybe<int> result =
    MaybeMonad<int>(5)
      .bind([](int n){ return n + 2; })
      .bind([](int n){ return Maybe<int>(n + 1); })
      .predicate([](int n) { return n > 0; })
      .extract();

  ASSERT(result, "Result should be non-empty");
  ASSERT(*result == 8, "Result should be 8");
}

void testIdentity() {
  Maybe<int> result = MaybeMonad<int>(5).extract();
  ASSERT(result, "Result should be non-empty");
  ASSERT(*result == 5, "Result should equal 5");
}

void testFailedPred() {
  Maybe<int> res(
    MaybeMonad<int>(1)
      .bind([](int n) { return n + 1; })
      .predicate([](int n) { return n > 10; })
      .extract());
  ASSERT(!res, "Result should be empty");
}

void testEmptyMonad() {
  Maybe<int> res = MaybeMonad<int>().extract();
  ASSERT(!res, "Result should be empty");
}

void testBindReturnsEmpty() {
  int c = 0;
  Maybe<int> res = MaybeMonad<int>(5)
    .bind([&c](int n) { c++; return n + 2; })
    .bind([&c](int n) { c++; return Maybe<int>(); })
    .bind([&c](int n) { c++; return n + 1; })
    .extract();

  ASSERT(!res, "Result should be empty");
  ASSERT(c == 2, "Only two of the bindings should be called");
}

int main() {
  RUN_TEST(testMaybe);
  RUN_TEST(testIdentity);
  RUN_TEST(testFailedPred);
  RUN_TEST(testEmptyMonad);
  RUN_TEST(testBindReturnsEmpty);
}
