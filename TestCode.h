//
// Created by EmsiaetKadosh on 25-1-20.
//

#pragma once

struct B {
	int a = 1;
	B() { std::wcout << L"common constructor\n"; }
	B(B&) { std::wcout << L"copy constructor\n"; }
	// B(B&& other) noexcept { std::wcout << L"move constructor\n"; }
};

struct A {
	B b;

	explicit A(B&& other) : b(other) {}
};

inline void test() {
	B b;
	A a{ B() };
	a = A(std::move(a.b));
}
