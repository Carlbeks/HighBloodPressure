//
// Created by EmsiaetKadosh on 25-1-18.
//

#include "def.h"

#include "exception.h"

template<TypeName Base> template<NewCopyable T> requires std::is_base_of_v<Base, T> && TypeName<T>
void ObjectHolder<Base>::set(const T& value) {
	if (!value) this->value = new T(value);
	else {
		if (hasValue) delete this->value;
		this->value = new T(value);
	}
}

template<TypeName Base> template<NewMoveable T> requires std::is_base_of_v<Base, T> && TypeName<T>
void ObjectHolder<Base>::set(T&& value) {
	if (!value) this->value = new T(value);
	else {
		if (hasValue) delete this->value;
		this->value = new T(value);
	}
}

void requireNonnull(const void* value) noexcept(false) { if (!value) throw NullPointerException(L"value is null"); }
