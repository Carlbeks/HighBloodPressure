//
// Created by EmsiaetKadosh on 25-1-16.
//

#pragma once
#include "Chars.h"
#include "def.h"

class IdentifiedText {};

/**
 * 注意一些默认值。
 * FontID=0，表示继承使用默认字体；
 * color和background=0xffffffff，表示继承使用默认颜色；
 */
class RenderableString {
	struct StringConfig {
		String text;
		/**
		 * 注意。-1也即0xffffffff被用于标记使用传入的默认颜色。
		 * 其余，以0xAARRGGBB的形式标记颜色。
		 */
		unsigned int color = -1;
		/**
		 * 注意。-1也即0xffffffff被用于标记使用传入的默认颜色。
		 * 其余，以0xAARRGGBB的形式标记颜色。
		 */
		unsigned int background = -1;
		wchar idFont = 0;
		bool bold = false;
		bool italic = false;
		bool underline = false;
		bool strikeThrough = false;

		explicit StringConfig() = default;

		void reset() noexcept {
			idFont = 0;
			color = -1;
			background = -1;
			bold = false;
			italic = false;
			underline = false;
			strikeThrough = false;
		}

		[[nodiscard]] StringConfig copyConfig() const noexcept {
			StringConfig ret;
			ret.idFont = idFont;
			ret.color = color;
			ret.background = background;
			ret.bold = bold;
			ret.italic = italic;
			ret.underline = underline;
			ret.strikeThrough = strikeThrough;
			return ret;
		}

		[[nodiscard]] StringConfig copy() const noexcept {
			StringConfig ret;
			ret.idFont = idFont;
			ret.text = text;
			ret.idFont = idFont;
			ret.color = color;
			ret.background = background;
			ret.bold = bold;
			ret.italic = italic;
			ret.underline = underline;
			ret.strikeThrough = strikeThrough;
			return ret;
		}

		[[nodiscard]] String toString() const noexcept {
			String ret;
			ret.append(L"#");
			ret.append(uitowb16(color));
			ret.append(L".");
			ret.append(uitowb16(background));
			ret.append(L",F");
			ret.append(std::to_wstring(idFont));
			ret.append(L",");
			if (bold) ret.append(L"=");
			if (italic) ret.append(L"/");
			if (underline) ret.append(L"_");
			if (strikeThrough) ret.append(L"-");
			ret.append(L":");
			ret.append(text);
			return ret;
		}
	};

	List<StringConfig> configs;
	using Iterator = List<StringConfig>::iterator;
	using ConstIterator = List<StringConfig>::const_iterator;

public:
	explicit RenderableString(const String& string): RenderableString(string.c_str(), string.length()) {}
	explicit RenderableString(String&& string) : RenderableString(string.c_str(), string.length()) {}

	explicit RenderableString(const wchar* const string, const QWORD length = static_cast<QWORD>(-1)) {
		if (length == -1) parseAppend(string);
		else parseAppend(string, length);
	}

	[[nodiscard]] String toString() const noexcept {
		String ret;
		for (const auto& config : configs) {
			ret.append(config.toString());
			ret.append(L";\n");
		}
		return ret;
	}

private:
	void parseAppend(const wchar* string) noexcept {
		StringConfig config;
		if (!configs.empty()) {
			config = configs.back();
			configs.pop_back();
		}
		while (*string != L'\0') {
			const wchar* start = string;
			if (!config.text.empty()) {
				configs.push_back(std::move(config));
				config = config.copyConfig();
			}
			while (*string != L'\0' && *string != '\\') ++string;
			if (*string == L'\0') {
				if (*start != L'\0') config.text.append(start, string);
				configs.push_back(std::move(config));
				return;
			}
			if (start != string) config.text.append(start, string);
			if (*++string == L'\0') {
				config.text.append(start, string);
				configs.push_back(std::move(config));
				return;
			}
			if (!config.text.empty()) {
				configs.push_back(std::move(config));
				config = config.copyConfig();
			}
			switch (*string) {
				case L'\\':
					config.text.append(1, L'\\');
					++string;
					continue;
				case L'#': {
					unsigned long long i = 0;
					while (i < 9) if (string[i++] == L'\0') goto end;
					config.color = wtouib16(++string, 8);
					string += 8;
					continue;
				}
				case L'.': {
					unsigned long long i = 0;
					while (i < 9) if (string[i++] == L'\0') goto end;
					config.background = wtouib16(++string, 8);
					string += 8;
					continue;
				}
				case L'F':
				case L'f':
					if (*++string == L'\0') goto end;
					config.idFont = *string;
					break;
				case L'-':
				case L's':
				case L'S':
					config.strikeThrough = true;
					break;
				case L'_':
				case L'u':
				case L'U':
					config.underline = true;
					break;
				case L'/':
				case L'i':
				case L'I':
					config.italic = true;
					break;
				case L'=':
				case L'b':
				case L'B':
					config.bold = true;
					break;
				case L'r':
				case L'R':
					if (!config.text.empty()) configs.push_back(std::move(config));
					config = StringConfig();
					break;
				default:
					config.text.append(string - 1, 2);
					break;
			}
			++string;
		}
	end:
		configs.push_back(std::move(config));
	}

	void parseAppend(const wchar* string, const QWORD length) noexcept {
		StringConfig config = StringConfig();
		if (!configs.empty()) {
			config = configs.back();
			configs.pop_back();
		}
		const wchar* const end = string + length;
		while (string < end) {
			const wchar* start = string;
			if (!config.text.empty()) {
				configs.push_back(std::move(config));
				config = config.copyConfig();
			}
			while (string < end && *string != '\\') ++string;
			if (string >= end) {
				if (start != end) config.text.append(start, end);
				configs.push_back(std::move(config));
				return;
			}
			if (start != string) config.text.append(start, string);
			if (++string == end) {
				config.text.append(start, end);
				configs.push_back(std::move(config));
				return;
			}
			if (!config.text.empty()) {
				configs.push_back(std::move(config));
				config = config.copyConfig();
			}
			switch (*string) {
				case L'\\':
					config.text.append(1, L'\\');
					++string;
					continue;
				case L'#': {
					if (end - string < 9) break;
					config.color = wtouib16(++string, 8);
					string += 8;
					continue;
				}
				case L'.': {
					if (end - string < 9) break;
					config.background = wtouib16(++string, 8);
					string += 8;
					continue;
				}
				case L'F':
				case L'f':
					if (++string == end) break;
					config.idFont = *string;
					break;
				case L'-':
				case L's':
				case L'S':
					config.strikeThrough = true;
					break;
				case L'_':
				case L'u':
				case L'U':
					config.underline = true;
					break;
				case L'/':
				case L'i':
				case L'I':
					config.italic = true;
					break;
				case L'=':
				case L'b':
				case L'B':
					config.bold = true;
					break;
				case L'r':
				case L'R':
					if (!config.text.empty()) configs.push_back(std::move(config));
					config = StringConfig();
					break;
				default:
					config.text.append(string - 1, 2);
					break;
			}
			++string;
		}
		configs.push_back(std::move(config));
	}

	RenderableString& append(const RenderableString& other) {
		auto iterator = other.configs.cbegin();
		int flags = 0;
		bool clr = true, bg = true, font = true;
		unsigned int color = configs.back().color;
		unsigned int background = configs.back().background;
		wchar idFont = configs.back().idFont;
		while (iterator != other.configs.cend()) {
			if (flags >= 3) break;
			StringConfig config = iterator->copy();
			if (font) {
				if (config.idFont == idFont || config.idFont != 0) {
					font = false;
					++flags;
				} else config.idFont = idFont;
			}
			if (clr) {
				if (config.color == color || config.color != 0xffffffff) {
					clr = false;
					++flags;
				} else config.color = color;
			}
			if (bg) {
				if (config.background == background || config.background != 0xffffffff) {
					bg = false;
					++flags;
				} else config.background = background;
			}
			configs.push_back(std::move(config));
			++iterator;
		}
		while (iterator != other.configs.cend()) configs.push_back(iterator->copy());
		return *this;
	}

	RenderableString& append(const String& other) {
		parseAppend(other.c_str(), other.length());
		return *this;
	}
};

interface Text {
	virtual ~Text() = default;
	virtual const String& getText() const noexcept = 0;
};

class Translator;

class TranslatableText final : public Text {
	const String idSrc;
	mutable const String* target = nullptr;
	mutable QWORD langConfig = 0;

public:
	explicit TranslatableText(const String& id) : idSrc(id) {}
	explicit TranslatableText(String&& id) : idSrc(std::move(id)) {}
	const String& getText() const noexcept override;
};

struct Language {
	Map<String, String> translateTable;
	int id = 1;
};

class Translator {
	Map<String, int> langMap{};
	List<Language> langList{};
	String lang = L"zh-cn";
	String nullText = L"<translator-null>";
	QWORD langConfig = 1;
	int idLangMax = 1;
	using IterID = Map<String, String>::const_iterator;
	using IterLang = List<Language>::const_iterator;

public:
	explicit Translator() { langMap.insert(std::make_pair(String(L"zh-cn"), 1)); }
	void addLang(const String& lang) noexcept(this->langMap.insert(std::make_pair(lang, this->idLangMax))) { langMap.insert(std::make_pair(lang, ++idLangMax)); }
	void addLang(String&& lang) noexcept(this->langMap.insert(std::make_pair(lang, this->idLangMax))) { langMap.insert(std::make_pair(lang, ++idLangMax)); }
	void loadLang();

	const String* getText(const String& id) const noexcept {
		for (const auto& [translateTable, _] : langList) {
			IterID iterator = translateTable.find(id);
			if (iterator != translateTable.cend()) return &iterator->second;
		}
		return &nullText;
	}

	int getConfigVersion() const noexcept { return langConfig; }
};

inline static Translator translator = Translator();
