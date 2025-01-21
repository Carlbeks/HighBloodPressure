//
// Created by EmsiaetKadosh on 25-1-18.
//

#include "def.h"

#include "exception.h"

template<TypeName Base> Base& ObjectHolder<Base>::operator->() noexcept(false) {
	if (value) return *value;
	throw NullPointerException(L"value is null");
}

template<TypeName Base> const Base& ObjectHolder<Base>::operator->() const noexcept(false) {
	if (value) return *value;
	throw NullPointerException(L"value is null");
}

template<TypeName Base> Base& ObjectHolder<Base>::operator*() noexcept(false) {
	if (value) return *value;
	throw NullPointerException(L"value is null");
}

template<TypeName Base> const Base& ObjectHolder<Base>::operator*() const noexcept(false) {
	if (value) return *value;
	throw NullPointerException(L"value is null");
}

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

template<TypeName T> const T& Reference<T>::getLvalue() const noexcept(false) {
	if (type == ReferenceValueType::LVALUE) return *lvalue;
	throw NullPointerException(L"reference is rvalue");
}

template<TypeName T> T&& Reference<T>::getRvalue() noexcept(false) {
	if (type == ReferenceValueType::RVALUE) return std::move(*rvalue);
	throw NullPointerException(L"reference is lvalue");
}
