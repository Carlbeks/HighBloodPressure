//
// Created by EmsiaetKadosh on 25-3-4.
//
#pragma once

template <typename T, typename L = void>
class AnywhereEditable;
template <typename T, typename L = void>
class AnywhereEditableList;
template <typename T, typename L = void>
class AnywhereIterator;
class AnywhereIteratorEnd;

/**
 * 此处无法进行代码编译层面的直接约束
 * @tparam T 满足T extends AnywhereEditable<T, L>
 * @tparam L 满足L extends AnywhereEditableList<T, L>
 */
template <typename T, typename L>
class AnywhereEditable {
public:
	using Lst = std::conditional_t<std::is_same_v<L, void>, AnywhereEditableList<T, L>, L>;

private:
	friend class AnywhereIterator<T, L>;
	friend class AnywhereEditableList<T, L>;
	friend class AnywhereIteratorEnd;
	friend L;
	AnywhereEditable* prev = nullptr;
	AnywhereEditable* next = nullptr;
	AnywhereEditableList<T, L>* list = nullptr; // 指示自身属于某个列表
protected:
	bool managedByList = false; // 指示是否在列表内管理内存，而非列表外管理内存
	byte reserved[7]{}; // reserved[0]: Task::schedulePop

public:
	AnywhereEditable() = default;
	AnywhereEditable(const AnywhereEditable& other) {}

	AnywhereEditable(AnywhereEditable&& other) noexcept : prev(other->prev), next(other->next), list(other->list) {
		other->prev = nullptr;
		other->next = nullptr;
		other->list = nullptr;
	}

	virtual ~AnywhereEditable() {
		if (list) {
			managedByList = false;
			auto lst = list;
			list = nullptr;
			lst->pop(static_cast<T*>(this));
			Logger.warn(L"~AnywhereEditable() : 析构时没有清除list，导致了析构时pop");
		}
	}

	int pushCopy(AnywhereEditableList<T, L>& list) noexcept;
	int pushThis(AnywhereEditableList<T, L>& list) noexcept;
	virtual int pop() noexcept;
	AnywhereEditableList<T, L>* getContainer() const noexcept { return list; }
};

template <typename T, typename L>
class AnywhereIterator {
	friend class AnywhereIteratorEnd;
	AnywhereEditable<T, L>* current;

public:
	explicit AnywhereIterator(AnywhereEditable<T, L>* current) : current(current) {}
	AnywhereIterator(const AnywhereIterator& other) = default;

	T& operator*() noexcept(false) {
		if (current->next) return static_cast<T&>(*current);
		throw RuntimeException(L"AnywhereIterator::operator*() : next is null, end of list");
	}

	T* operator->() noexcept(false) {
		if (current->next) return &static_cast<T&>(*current);
		throw RuntimeException(L"AnywhereIterator::operator*() : next is null, end of list");
	}

	bool operator!=(const AnywhereIterator& other) const noexcept { return current != other.current; }
	bool operator==(const AnywhereIterator& other) const noexcept { return current == other.current; }
	bool operator!=(const AnywhereIteratorEnd& other) const noexcept;
	bool operator==(const AnywhereIteratorEnd& other) const noexcept;

	AnywhereIterator& operator++() noexcept(false) {
		if (current->next) current = current->next;
		else throw InvalidOperationException(L"AnywhereIterator::operator++() : end of list");
		return *this;
	}

	AnywhereIterator operator++(int) noexcept(false) {
		if (current->next) {
			AnywhereIterator ret = *this;
			current = current->next;
			return ret;
		}
		throw InvalidOperationException(L"AnywhereIterator::operator++() : end of list");
	}

	AnywhereIterator& operator--() noexcept(false) {
		if (current->prev) current = current->prev;
		else throw InvalidOperationException(L"AnywhereIterator::operator--() : begin of list");
		return *this;
	}

	AnywhereIterator operator--(int) noexcept(false) {
		if (current->prev) {
			AnywhereIterator ret = *this;
			current = current->prev;
			return ret;
		}
		throw InvalidOperationException(L"AnywhereIterator::operator--() : begin of list");
	}
};

class AnywhereIteratorEnd {
public:
	AnywhereIteratorEnd() {}
	AnywhereIteratorEnd(const AnywhereIteratorEnd& other) = delete;
	AnywhereIteratorEnd(AnywhereIteratorEnd&& other) = delete;

	template <typename T, typename L>
	bool operator!=(const AnywhereIterator<T, L>& other) const noexcept { return other.current && other.current->next; }

	template <typename T, typename L>
	bool operator==(const AnywhereIterator<T, L>& other) const noexcept { return !operator!=(other); }
};

template <typename T, typename L>
class AnywhereEditableList {
protected:
	AnywhereEditable<T, L> head;
	AnywhereEditable<T, L> tail;

public:
	AnywhereEditableList() {
		head.next = &tail;
		tail.prev = &head;
	}

	virtual ~AnywhereEditableList() { for (AnywhereIterator<T, L> it = begin(); it != end(); ++it) {} }

	int pushCopy(T* value) noexcept;
	int pushThis(T* value) noexcept;
	int pushNewed(T* value) noexcept;
	virtual int pop(T* value) noexcept;
	AnywhereIterator<T, L> begin() noexcept { return AnywhereIterator<T, L>(head.next); }
	AnywhereIteratorEnd end() noexcept { return AnywhereIteratorEnd(); }
	AnywhereIterator<T, L> begin() const noexcept { return AnywhereIterator<T, L>(head.next); }
	AnywhereIteratorEnd end() const noexcept { return AnywhereIteratorEnd(); }
	AnywhereEditable<T, L>* front() const noexcept { return head.next == &tail ? nullptr : head.next; }
	AnywhereEditable<T, L>* back() const noexcept { return tail.prev == &head ? nullptr : tail.prev; }
};

template <typename T, typename L>
int AnywhereEditable<T, L>::pushCopy(AnywhereEditableList<T, L>& list) noexcept { return list.pushCopy(static_cast<T*>(this)); }

template <typename T, typename L>
int AnywhereEditable<T, L>::pushThis(AnywhereEditableList<T, L>& list) noexcept { return list.pushThis(static_cast<T*>(this)); }

template <typename T, typename L>
int AnywhereEditable<T, L>::pop() noexcept { return list->pop(static_cast<T*>(this)); }

template <typename T, typename L>
bool AnywhereIterator<T, L>::operator!=(const AnywhereIteratorEnd& other) const noexcept { return other != *this; }

template <typename T, typename L>
bool AnywhereIterator<T, L>::operator==(const AnywhereIteratorEnd& other) const noexcept { return other == *this; }

template <typename T, typename L>
int AnywhereEditableList<T, L>::pushCopy(T* value) noexcept {
	T* nv = allocatedFor(new T(*value));
	return pushNewed(nv);
}

template <typename T, typename L>
int AnywhereEditableList<T, L>::pushThis(T* value) noexcept {
	if (value->list) {
		Logger.error(L"AnywhereEditableList::pushThis() : value is already in a list");
		Failed();
	}
	value->list = this;
	tail.prev->next = value;
	value->prev = tail.prev;
	value->next = &tail;
	tail.prev = value;
	Success();
}

template <typename T, typename L>
int AnywhereEditableList<T, L>::pushNewed(T* value) noexcept {
	const int ret = pushThis(value);
	if (ret) delete deallocating(value);
	else value->managedByList = true;
	return ret;
}

template <typename T, typename L>
int AnywhereEditableList<T, L>::pop(T* value) noexcept {
	if (value->list != this) {
		Logger.error(L"AnywhereEditableList::pop() : value is not in this list");
		Failed();
	}
	value->list = nullptr;
	value->next->prev = value->prev;
	value->prev->next = value->next;
	if (value->managedByList) delete deallocating(value);
	Success();
}
