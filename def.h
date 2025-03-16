//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#define __CARLBEKS_DEBUG__
#define __CARLBEKS_MEMORY__ 1

#pragma warning(disable: 4819)

#include <typeinfo>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <map>
#include <atomic>
#include <thread>
#include <fstream>
#include <cmath>

using wchar = wchar_t;
using QWORD = unsigned long long int;
using String = std::wstring;
using Thread = std::thread;
template<typename K, typename V, typename Cmp = std::less<K>, typename Alloc = std::allocator<std::pair<const K, V>>> using Map = std::map<K, V, Cmp, Alloc>;
template<typename T, typename Alloc = std::allocator<T>> using List = std::list<T, Alloc>;
template<typename F> using Function = std::function<F>;

#define Success() { return 0; }
#define Failed() { return 1; }
#define Error() { return -1; }
#define Comment(PARAMS) /##/ PARAMS
#define SameAs(PARAMS) Comment(PARAMS)

//NOLINTNEXTLINE(*-reserved-identifier)
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
#include <dwmapi.h>

#define WM_APP_LBUTTONUP (WM_APP + 1)
#define WM_APP_MBUTTONDOWN (WM_APP + 2)

#pragma comment(lib, "Msimg32.lib")
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

struct MemoryManager {
	struct MemoryInfo {
		const String msg;
		std::size_t size;
	};

	Map<void*, MemoryInfo> allocated {};

	constexpr MemoryManager() noexcept = default;
} inline memoryManager;

void requireNonnull(const void* value) noexcept(false);
void checkAllocation(const void* value) noexcept(false);
inline String ptrtow(QWORD value);

#if defined __CARLBEKS_DEBUG__ || defined __CARLBEKS_MEMORY__
void printAllocate(void* value, std::size_t size, const String&);
void printDeallocate(void* value, std::size_t size, const String&);
extern String atow(const char* chars);

template<typename T> T* allocatedFor$(T* value, const String& msg = L"", std::size_t size = sizeof(T)) {
	requireNonnull(value);
	const auto& k = memoryManager.allocated.emplace(value, MemoryManager::MemoryInfo{ L"[" + atow(typeid(T).name()) + L"] " + msg , size}).first;
#if __CARLBEKS_MEMORY__ > 1
	printAllocate(value, k->second.size, k->second.msg);
#endif
	return value;
}

template<typename T> T* deallocating$(T* value) {
#if __CARLBEKS_MEMORY__ > 1
	const MemoryManager::MemoryInfo* info = nullptr;
	if (memoryManager.allocated.contains(value)) info = &memoryManager.allocated.at(value);
	printDeallocate(value, info ? info->size : 0, info ? info->msg : L"???");
#endif
	if (value) memoryManager.allocated.erase(value);
	return value;
}

#if __CARLBEKS_MEMORY__ > 3
#define allocatedFor(val, ...) allocatedFor$(val, L"\n    From " __FUNCSIG__ "\n    At   " __FILE__ ":" _STL_STRINGIZE(__LINE__) __VA_OPT__(,) __VA_ARGS__)
#else
#define allocatedFor(val, ...) allocatedFor$(val, L"" __VA_OPT__(,) __VA_ARGS__)
#endif
#define deallocating(val) deallocating$(val)
#else
#define allocatedFor(val, ...) val
#define deallocating(val) val
#endif


template<TypeName Base> class ObjectHolder {
	Base* value;
	bool hasValue;
	char padding[7]{};

public:
	/**
	 * 用于延迟初始化。
	 */
	ObjectHolder() : value(nullptr), hasValue(false) {}

	ObjectHolder(Base* value) : value(value), hasValue(false) {}

	template<NewCopyable T> requires (std::is_base_of_v<Base, T> || std::is_same_v<Base, T>) && TypeName<T>
	ObjectHolder(const T& value) : value(allocatedFor(new T(value))), hasValue(true) {}

	template<NewMoveable T> requires (std::is_base_of_v<Base, T> || std::is_same_v<Base, T>) && TypeName<T>
	ObjectHolder(T&& value) : value(allocatedFor(new T(std::forward<T>(value)))), hasValue(true) {}

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
		if (hasValue) delete deallocating(value);
		value = nullptr;
	}

	[[nodiscard]] Base* operator->() noexcept(false) {
		requireNonnull(value);
		return value;
	}

	[[nodiscard]] const Base* operator->() const noexcept(false) {
		requireNonnull(value);
		return value;
	}

	[[nodiscard]] Base& operator*() noexcept(false) {
		requireNonnull(value);
		return *value;
	}

	[[nodiscard]] const Base& operator*() const noexcept(false) {
		requireNonnull(value);
		return *value;
	}

	[[nodiscard]] Base& get() noexcept(false) {
		requireNonnull(value);
		return *value;
	}

	[[nodiscard]] const Base& get() const noexcept(false) {
		requireNonnull(value);
		return *value;
	}

	[[nodiscard]] operator Base*() const noexcept(false) { return value; }
	Base* ptr() const noexcept { return value; }
	operator bool() const noexcept { return value; }
	bool operator!() const noexcept { return value == nullptr; }
	[[nodiscard]] bool isManager() const noexcept { return hasValue; }

	template<typename T> ObjectHolder<T> referenceof(const T& other) {
		ObjectHolder ret{};
		ret.value = &other;
		return ret;
	}
};

template<TypeName Base> class SynchronizedHolder {
	mutable Base* newValue = nullptr;
	mutable Base* value = nullptr;
	mutable bool isOk = false;

public:
	SynchronizedHolder() = default;

	~SynchronizedHolder() {
		if (newValue == value) { if (value) delete deallocating(value); } else {
			if (newValue) delete deallocating(newValue);
			if (value) delete deallocating(value);
		}
		newValue = nullptr;
		value = nullptr;
	}


	template<NewCopyable T> requires std::is_base_of_v<Base, T> && TypeName<T>
	void setNew(const Base& other) noexcept {
		isOk = false;
		if (newValue && newValue != value) deleteNew();
		newValue = allocatedFor(new T(other));
	}

	template<NewMoveable T> requires std::is_base_of_v<Base, T> && TypeName<T>
	void setNew(T&& val) noexcept {
		isOk = false;
		if (newValue && newValue != value) deleteNew();
		newValue = allocatedFor(new T(std::forward<T>(val)));
	}

	void ok() const noexcept { isOk = true; }

	Base& get() const noexcept(false) {
		requireNonnull(value);
		return *value;
	}

	Base& getNew() const noexcept(false) {
		requireNonnull(newValue); // 用于抛错
		return *newValue;
	}

	Base* ptr() const noexcept { return value; }
	Base* ptrNew() const noexcept { return newValue; }
	Base* ptrs() const noexcept { return newValue ? newValue : value; }

	void async() const noexcept {
		if (newValue == value) return;
		if (!isOk) return;
		Base* nuv = newValue;
		newValue = nullptr;
		if (nuv) {
			if (value && value != nuv) deleteOld();
			value = nuv;
		}
	}

private:
	void deleteOld() const noexcept {
		delete deallocating(value);
		value = nullptr;
	}

	void deleteNew() const noexcept {
		delete deallocating(newValue);
		newValue = nullptr;
	}
};
