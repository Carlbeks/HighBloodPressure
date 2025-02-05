//
// Created by EmsiaetKadosh on 25-1-22.
//

#pragma once

#include "def.h"
#include "exception.h"
#include "Game.h"

ILogger& PublicLogger::trace(const String& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Trace] " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::trace(String&& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Trace] " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::debug(const String& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Debug] " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::debug(String&& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Debug] " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::log(const String& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Log]   " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::log(String&& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Log]   " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::info(const String& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Info]  " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::info(String&& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Info]  " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::warn(const String& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Warn]  " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::warn(String&& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Warn]  " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::error(const String& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Error] " + msg + L"\n";
	return *this;
}

ILogger& PublicLogger::error(String&& msg) noexcept {
	std::wcout << L"T-" + qwtowb10(game.getTick(), 8) + L" [Error] " + msg + L"\n";
	return *this;
}

inline PublicLogger Logger{};
