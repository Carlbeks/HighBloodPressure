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
	explicit NullPointerException(String&& msg) : Exception(std::move(msg), &type) {}
	explicit NullPointerException(const String& msg) : Exception(msg, &type) {}
};

class ArrayIndexOutOfBoundException final : public Exception {
	inline static const String type = L"ArrayIndexOutOfBoundException";

public:
	explicit ArrayIndexOutOfBoundException(String&& msg) : Exception(std::move(msg), &type) {}
	explicit ArrayIndexOutOfBoundException(const String& msg) : Exception(msg, &type) {}
};

class InvalidOperationException final : public Exception {
	inline static const String type = L"InvalidOperationException";

public:
	explicit InvalidOperationException(String&& msg) : Exception(std::move(msg), &type) {}
	explicit InvalidOperationException(const String& msg) : Exception(msg, &type) {}
};

interface ILogger {
	virtual ~ILogger() = default;
	virtual ILogger& trace(const String&) noexcept = 0;
	virtual ILogger& trace(String&&) noexcept = 0;
	virtual ILogger& debug(const String&) noexcept = 0;
	virtual ILogger& debug(String&&) noexcept = 0;
	virtual ILogger& log(const String&) noexcept = 0;
	virtual ILogger& log(String&&) noexcept = 0;
	virtual ILogger& info(const String&) noexcept = 0;
	virtual ILogger& info(String&&) noexcept = 0;
	virtual ILogger& warn(const String&) noexcept = 0;
	virtual ILogger& warn(String&&) noexcept = 0;
	virtual ILogger& error(const String&) noexcept = 0;
	virtual ILogger& error(String&&) noexcept = 0;
};

struct PublicLogger final : ILogger {
	PublicLogger() { std::wcout << L"PublicLogger created\n"; }

	PublicLogger& put(const String& msg) noexcept {
		std::wcout << L"[] [Root] " + msg + L"\n";
		return * this;
	}

	PublicLogger& put(String&& msg) noexcept {
		std::wcout << L"[] [Root] " + msg + L"\n";
		return *this;
	}

	ILogger& trace(const String&) noexcept override;
	ILogger& trace(String&&) noexcept override;
	ILogger& debug(const String&) noexcept override;
	ILogger& debug(String&&) noexcept override;
	ILogger& log(const String&) noexcept override;
	ILogger& log(String&&) noexcept override;
	ILogger& info(const String&) noexcept override;
	ILogger& info(String&&) noexcept override;
	ILogger& warn(const String&) noexcept override;
	ILogger& warn(String&&) noexcept override;
	ILogger& error(const String&) noexcept override;
	ILogger& error(String&&) noexcept override;
};

extern PublicLogger Logger;

