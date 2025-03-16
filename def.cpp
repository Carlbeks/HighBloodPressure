//
// Created by EmsiaetKadosh on 25-1-18.
//

#include "def.h"

#include "Chars.h"
#include "exception.h"
#include "Game.h"

template<TypeName Base> template<NewCopyable T> requires std::is_base_of_v<Base, T> && TypeName<T>
void ObjectHolder<Base>::set(const T& value) {
	if (!value) this->value = allocatedFor(new T(value));
	else {
		if (hasValue) delete deallocating(this->value);
		this->value = allocatedFor(new T(value));
	}
}

template<TypeName Base> template<NewMoveable T> requires std::is_base_of_v<Base, T> && TypeName<T>
void ObjectHolder<Base>::set(T&& value) {
	if (!value) this->value = allocatedFor(new T(value));
	else {
		if (hasValue) delete deallocating(this->value);
		this->value = allocatedFor(new T(value));
	}
}

void requireNonnull(const void* value) noexcept(false) { if (!value) throw NullPointerException(L"value is null"); }
void checkAllocation(const void* value) noexcept(false) { if (!value) throw BadAllocationException(L"bad allocation"); }

void printAllocate(void* value, const size_t size, const String& msg) {
	const String str = L"alloc   " + ptrtow(reinterpret_cast<QWORD>(value)) + L" " + std::to_wstring(size) + String(L"B ") + msg;
	Logger.log(str);
#if __CARLBEKS_MEMORY__ > 2
	MainLogFile << str << std::endl;
#endif
}

void printDeallocate(void* value, const size_t size, const String& msg) {
	const String str = L"dealloc " + ptrtow(reinterpret_cast<QWORD>(value)) + L" " + std::to_wstring(size) + String(L"B ") + msg;
	Logger.log(str);
#if __CARLBEKS_MEMORY__ > 2
	MainLogFile << str << std::endl;
#endif
}

String ptrtow(const QWORD value) { return qwtowb16(value, 16); }
