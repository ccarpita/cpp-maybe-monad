.DEFAULT_GOAL := build

build:
	clang++ -std=c++11 -stdlib=libc++ test.cpp -o test_monads

test: build
	./test_monads
