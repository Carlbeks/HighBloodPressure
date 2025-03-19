//
// Created by EmsiaetKadosh on 25-3-7.
//

#pragma once

class File final {
public:
	String path{};
	std::wfstream file{};
	int flags = std::ios::in | std::ios::out | std::ios::binary;

	File(const String& path) : path(path) {}
	File(const File&) = delete;
	File(File&&) = delete;
	~File() = default;
	File& operator=(const File&) = delete;
	File& operator=(File&&) = delete;

	File& open() {
		if (file.is_open()) file.close();
		file.open(path, flags);
		return *this;
	}

	File& close() {
		if (file.is_open()) file.close();
		return *this;
	}

	File& clearContent() {
		file.clear();
		if (file.is_open()) file.close();
		file.open(path, flags | std::ios::trunc);
		return *this;
	}

	File& inputs(const bool value = true) {
		if (value) flags |= std::ios::in;
		else flags &= ~std::ios::in;
		return *this;
	}

	File& outputs(const bool value = true) {
		if (value) flags |= std::ios::out;
		else flags &= ~std::ios::out;
		return *this;
	}

	File& binary(const bool value = true) {
		if (value) flags |= std::ios::binary;
		else flags &= ~std::ios::binary;
		return *this;
	}

	File& append(const bool value = true) {
		if (value) flags |= std::ios::app;
		else flags &= ~std::ios::app;
		return *this;
	}

	File& truncate(const bool value = true) {
		if (value) flags |= std::ios::trunc;
		else flags &= ~std::ios::trunc;
		return *this;
	}

	template <typename T>
	File& operator<<(T&& value) {
		if (file.is_open()) file << std::forward<T>(value);
		return *this;
	}

	File& operator<<(decltype(std::endl<wchar, std::char_traits<wchar>>)& value) {
		if (file.is_open()) file << value;
		return *this;
	}

	template <typename T>
	File& operator>>(T&& value) {
		if (file.is_open()) file >> std::forward<T>(value);
		return *this;
	}
};

class Data {
public:
	enum class DataType : unsigned char {
		Integer, Double, String, Boolean, Null, List, Object
	};

private:
	String name;
	String value;
	DataType type;

public:
	Data(String&& name, String&& value, const DataType type): name(std::move(name)), value(std::move(value)), type(type) {}
};

class DataLoader {
	File file;

	int parseString(int& line, String& string) {
		while (!file.file.eof()) {
			wchar c = file.file.get();
			if (c == L'\"') Success();
			if (c == L'\\') { c = file.file.get(); }
		}
		errorInfo = L"Error: String never ends. EOF comes before a '\"'";
		Failed();
	}

	int loadUntil(int& line, wchar at = 0) {
		String name = {};
		String val = {};
		enum : unsigned char { identifier, eq, value, end } status = identifier;
		while (!file.file.eof()) {
			const wchar c = file.file.get();
			if (c == L'\n') ++line;
			switch (status) {
				case identifier:
					if (!isspace(c)) name.append(1, c);
					else if (!name.empty()) status = eq;
					break;
				case eq:
					if (c == L'=') status = value;
					else if (!isspace(c)) {
						errorInfo = L"Error: expected '=' after identifier at line " + std::to_wstring(line);
						Failed();
					}
					break;
				case value:
					if (!isspace(c)) { if (c == L'\"') {} }
					break;
				case end:
					break;
			}
		}
	}

public:
	String errorInfo = {};
	DataLoader(const String& path) : file(path) {}
	DataLoader(const DataLoader&) = delete;
	DataLoader(DataLoader&&) = delete;
	DataLoader& operator=(const DataLoader&) = delete;
	DataLoader& operator=(DataLoader&&) = delete;
	~DataLoader() = default;

	int load() {
		int line = 1;
		return loadUntil(line);
	}
};
