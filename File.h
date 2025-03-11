//
// Created by EmsiaetKadosh on 25-3-7.
//

#pragma once


class File {
public:
	std::wfstream file;
	explicit File(const std::wstring& path) : file(path, std::ios::in | std::ios::out | std::ios::binary) {}
	~File() { if (file.is_open()) file.close(); }
};

class Data {
public:
	enum class DataType : int {
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
			if (c == L'\\') {
				c = file.file.get();
			}
		}
		errorInfo =	L"Error: String never ends. EOF comes before a '\"'";
		Failed();
	}

	int loadUntil(int& line, wchar at = 0) {
		String name = {};
		String val = {};
		enum { identifier, eq, value, end } status = identifier;
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
	explicit DataLoader(const String& path) : file(path) {}
	DataLoader(const DataLoader&) = delete;
	DataLoader(DataLoader&&) = delete;

	int load() {
		int line = 1;
		return loadUntil(line);
	}
};
