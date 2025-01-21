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
	[[nodiscard]] const char* what() const override { return "Use Exception::getMessage() instead.";}
};

class NullPointerException final : public Exception {
	inline static const String type = L"NullPointerException";

public:
	explicit NullPointerException(String&& msg) : Exception(std::move(msg), &type) {}
	explicit NullPointerException(const String& msg) : Exception(msg, &type) {}
};

class ArrayIndexOutOfBoundException final : public Exception {
	inline static const String type = L"ArrayIndexOutOfBoundException";

public:
	explicit ArrayIndexOutOfBoundException(String&& msg) : Exception(std::move(msg), &type) {}
	explicit ArrayIndexOutOfBoundException(const String& msg) : Exception(msg, &type) {}
};
