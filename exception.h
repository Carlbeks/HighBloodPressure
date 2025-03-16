//
// Created by EmsiaetKadosh on 25-1-18.
//

#pragma once

class Exception : public std::exception {
	const String* type;

protected:
	String msg;

	Exception(String&& msg, const String* type) : type(type), msg(std::move(msg)) {}
	Exception(const String& msg, const String* type) : type(type), msg(msg) {}

public:
	[[nodiscard]] String getMessage() const noexcept { return msg; }
	[[nodiscard]] const String* getType() const noexcept { return type; }
	[[nodiscard]] const char* what() const override { return "Use Exception::getMessage() instead."; }
};

class NullPointerException final : public Exception {
	inline static const String type = L"NullPointerException";

public:
	NullPointerException(String&& msg) : Exception(std::move(msg), &type) {}
	NullPointerException(const String& msg) : Exception(msg, &type) {}
};

class BadAllocationException final : public Exception {
	inline static const String type = L"BadAllocationException";

public:
	BadAllocationException(String&& msg) : Exception(std::move(msg), &type) {}
	BadAllocationException(const String& msg) : Exception(msg, &type) {}
};

class ArrayIndexOutOfBoundException final : public Exception {
	inline static const String type = L"ArrayIndexOutOfBoundException";

public:
	ArrayIndexOutOfBoundException(String&& msg) : Exception(std::move(msg), &type) {}
	ArrayIndexOutOfBoundException(const String& msg) : Exception(msg, &type) {}
};

class InvalidOperationException final : public Exception {
	inline static const String type = L"InvalidOperationException";

public:
	InvalidOperationException(String&& msg) : Exception(std::move(msg), &type) {}
	InvalidOperationException(const String& msg) : Exception(msg, &type) {}
};

class RuntimeException final : public Exception {
	inline static const String type = L"RuntimeException";
public:
	RuntimeException(String&& msg) : Exception(std::move(msg), &type) {}
	RuntimeException(const String& msg) : Exception(msg, &type) {}
};


class PublicLogger final {
	[[nodiscard]] String build(const String& msg, const String& type) const;

	template<typename T, typename... Ts> requires requires(T t, Ts... ts) {
		std::wcout << t;
		(std::wcout << ... << ts);
	}
	static void prints(T&& t, Ts&&... ts) {
		std::wcout << L" " << t;
		prints(ts...);
	}

	template<typename T> requires requires(T t) { std::wcout << t; }
	static void prints(T&& t) { std::wcout << L" " << t; }

public:
	const String name;
	PublicLogger(const String& name): name(L" [" + name + L"] ") { std::wcout << L"PublicLogger created\n"; }

	PublicLogger& put(const String& msg) noexcept {
		std::wcout << L"[] [Root]" + name + msg + L"\n";
		return *this;
	}

	PublicLogger& trace(const String& msg) noexcept {
		std::wcout << build(msg, L"[Trace] ");
		return *this;
	}

	PublicLogger& debug(const String& msg) noexcept {
		std::wcout << build(msg, L"[Debug] ");
		return *this;
	}

	PublicLogger& log(const String& msg) noexcept {
		std::wcout << build(msg, L"[Log]   ");
		return *this;
	}

	PublicLogger& info(const String& msg) noexcept {
		std::wcout << build(msg, L"[Info]  ");
		return *this;
	}

	PublicLogger& warn(const String& msg) noexcept {
		std::wcout << build(msg, L"[Warn]  ");
		return *this;
	}

	PublicLogger& error(const String& msg) noexcept {
		std::wcout << build(msg, L"[Error] ");
		return *this;
	}

	template<typename T> requires requires(T t) { std::wcout << t; }
	PublicLogger& print(T&& msg) noexcept {
		std::wcout << L"[] [Root]" + name << msg << std::endl;
		return *this;
	}

	template<typename T, typename... Ts> requires requires(T t, Ts... ts) {
		std::wcout << t;
		std::wcout << (ts, ...);
	}
	PublicLogger& print(T&& msg, Ts&&... other) noexcept {
		std::wcout << L"[] [Root]" + name << msg;
		prints(other...);
		std::wcout << std::endl;
		return *this;
	}
};

inline PublicLogger Logger(L"Main");

