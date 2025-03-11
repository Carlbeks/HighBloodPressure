//
// Created by EmsiaetKadosh on 25-1-20.
//

#pragma once

inline void test() {}

inline void rsa() {
	int pk, sk;
	int p = 0, q = 0; // = random prime;
	int n = p * q;
	int phi = (p - 1) * (q - 1);
	int d = 0, e = 0;
	// assert gcd(e, phi) == 1;
	// assert d * e === 1 mod phi;
	// encode:
	int message = 0, package;
	package = static_cast<int>(pow(message, e)) % n;
	// decode:
	message = static_cast<int>(pow(package, d)) % n;
}
