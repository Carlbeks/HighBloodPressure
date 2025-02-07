//
// Created by EmsiaetKadosh on 25-1-17.
//

#pragma once

#include "def.h"

[[nodiscard]] inline unsigned int wtouib16(const wchar* string) noexcept {
	unsigned int ret = 0;
	while (*string != L'\0') {
		if (ret & 0xf0000000) return 0xffffffff;
		if (*string >= L'0' && *string <= L'9') {
			ret <<= 4;
			ret += *string - L'0';
		} else if (*string >= L'A' && *string <= L'F') {
			ret <<= 4;
			ret += *string - 0x41;// 'A' - 10
		} else if (*string >= L'a' && *string <= L'f') {
			ret <<= 4;
			ret += *string - 0x61;// 'a' - 10
		} else return 0xffffffff;
		++string;
	}
	return ret;
}

[[nodiscard]] inline unsigned int wtouib16(const wchar* const string, const unsigned long long length) noexcept {
	unsigned int ret = 0;
	unsigned long long i = 0;
	while (i < length) {
		if (ret & 0xf0000000) return 0xffffffff;
		if (string[i] >= L'0' && string[i] <= L'9') {
			ret <<= 4;
			ret += string[i] - L'0';
		} else if (string[i] >= L'A' && string[i] <= L'F') {
			ret <<= 4;
			ret += string[i] - 0x41;// L'A' - 10
		} else if (string[i] >= L'a' && string[i] <= L'f') {
			ret <<= 4;
			ret += string[i] - 0x61;// L'a' - 10
		} else return 0xffffffff;
		++i;
	}
	return ret;
}

/**
 * 将数字转换为字符串
 * @param value 要转换的数字
 * @param fills 填充位数。返回的字符串长度一定不小于该值
 * @return String类型
 */
[[nodiscard]] inline String uitowb16(unsigned int value, const unsigned int fills = 1) noexcept {
	static constexpr wchar Table16[17] = L"0123456789ABCDEF";
	String ret;
	if (value < static_cast<unsigned int>(1) << fills - 1) {
		ret.assign(fills, L'0');
		for (unsigned int i = fills - 1; i != 0 && value; --i) {
			ret[i] = Table16[value & 0xf];
			value >>= 4;
		}
	} else {
		unsigned int i = 0;
		while (i < 8) {
			if ((value >> i) & 0xf) break;
			++i;
		}
		while (i < 8) {
			ret.push_back(Table16[(value >> i) & 0xf]);
			++i;
		}
		if (ret.empty()) ret = L"0";
	}
	return ret;
}

[[nodiscard]] inline String qwtowb10(QWORD value, const unsigned int fills = 1) noexcept {
	static constexpr wchar Table10[11] = L"0123456789";
	static constexpr QWORD Compare10[20] = {
		0, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
		100000000, 1000000000, 10000000000, 100000000000,
		1000000000000, 10000000000000, 100000000000000,
		1000000000000000, 10000000000000000, 100000000000000000,
		1000000000000000000, 10000000000000000000
	};
	String ret;
	if (value < Compare10[fills]) {
		ret.assign(fills, L'0');
		for (unsigned int i = fills - 1; i != 0 && value; --i) {
			ret[i] = Table10[value % 10];
			value /= 10;
		}
	} else {
		unsigned int i = 0;
		while (i < 19) {
			if (value >= Compare10[i]) break;
			++i;
		}
		while (i < 19) {
			ret.push_back(Table10[value / Compare10[i]]);
			value %= Compare10[i];
			++i;
		}
		if (ret.empty()) ret = L"0";
	}
	return ret;
}
