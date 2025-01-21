//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <map>
#include <atomic>

using wchar = wchar_t;
using QWORD = unsigned long long int;
using String = std::wstring;
template<typename K, typename V, typename Cmp = std::less<K>, typename Alloc = std::allocator<std::pair<const K, V>>> using Map = std::map<K, V, Cmp, Alloc>;
template<typename T, typename Alloc = std::allocator<T>> using List = std::list<T, Alloc>;
template<typename F> using Function = std::function<F>;

#define Success() { return 0; }
#define Failed() { return 1; }
#define Error() { return -1; }

#if false
#ifdef _MSC_VER
#define __FUNCSIGW__ L"" __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
#define __FUNCSIGW__ L"" __PRETTY_FUNCTION__
#else
#define __FUNCSIGW__ L"" __func__
#endif
#endif

#define _WINSOCKAPI_ /* 防止winsock.h被引入。winsock.h和winsock2.h冲突。 */
#if false
#include <WinSock2.h>
#endif

#define NOMINMAX
#include <Windows.h>
#include <Windowsx.h>
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <WinUser.h>
#include <Uxtheme.h>
#include <iostream>
#include <dwmapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Uxtheme.lib")
#pragma comment(lib, "winmm.lib")

template<typename T>
concept Copyable = requires(const T& t) { T(t); };
template<typename T>
concept NewCopyable = requires(const T& t) { delete new T(t); };
template<typename T>
concept Moveable = requires(T&& t) { T(std::move(t)); };
template<typename T>
concept NewMoveable = requires(T&& t) { delete new T(std::move(t)); };
template<typename T>
concept NonreferenceType = !std::is_reference_v<T>;
template<typename T>
concept NonpointerType = !std::is_pointer_v<T>;
template<typename T>
concept ReferenceType = std::is_reference_v<T>;
template<typename T>
concept PointerType = std::is_pointer_v<T>;
template<typename T>
concept TypeName = NonreferenceType<T> && NonpointerType<T>;

template<TypeName Base> class ObjectHolder {
	Base* value;
	bool hasValue;
	char padding[7]{};

public:
	/**
	 * 用于延迟初始化。
	 */
	ObjectHolder() : value(nullptr), hasValue(false) {}

	ObjectHolder(Base* const value) : value(value), hasValue(false) {}

	template<NewCopyable T> requires std::is_base_of_v<Base, T> && TypeName<T>
	ObjectHolder(const T& value) : value(new T(value)), hasValue(true) {}

	template<NewMoveable T> requires std::is_base_of_v<Base, T> && TypeName<T>
	ObjectHolder(T&& value) : value(new T(std::forward<T>(value))), hasValue(true) {}

	ObjectHolder(const ObjectHolder& other) noexcept: value(other.value), hasValue(false) {}

	ObjectHolder(ObjectHolder&& other) noexcept: value(other.value), hasValue(other.hasValue) {
		other.value = nullptr;
		other.hasValue = false;
	}

	template<NewCopyable T> requires std::is_base_of_v<Base, T> && TypeName<T>
	void set(const T& value);

	template<NewMoveable T> requires std::is_base_of_v<Base, T> && TypeName<T>
	void set(T&& value);


	~ObjectHolder() {
		if (hasValue) delete value;
		value = nullptr;
	}

	Base& operator->() noexcept(false);
	const Base& operator->() const noexcept(false);
	Base& operator*() noexcept(false);
	const Base& operator*() const noexcept(false);
	operator bool() const noexcept { return value; }
};

enum class ReferenceValueType {
	LVALUE,
	RVALUE
};

template<TypeName T> class Reference {
	const T* lvalue = nullptr;
	T* rvalue = nullptr;

public:
	const ReferenceValueType type;
	Reference(const T& lvalue) : lvalue(&lvalue), type(ReferenceValueType::LVALUE) {}
	Reference(T&& rvalue) : rvalue(&rvalue), type(ReferenceValueType::RVALUE) {}
	Reference& operator=(const Reference&) = delete;
	Reference& operator=(Reference&&) = delete;

	Reference& operator=(const T& lvalue) {
		this->lvalue = &lvalue;
		this->type = ReferenceValueType::LVALUE;
		return *this;
	}

	Reference& operator=(T&& rvalue) {
		this->rvalue = &rvalue;
		this->type = ReferenceValueType::RVALUE;
		return *this;
	}

	const T& getLvalue() const noexcept(false);
	T&& getRvalue() noexcept(false);
};
