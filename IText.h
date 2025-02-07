//
// Created by EmsiaetKadosh on 25-1-16.
//

#pragma once
#include "Chars.h"
#include "def.h"

class LiteralText;
class Translator;
// class Font;
class FontManager;

using FontID = int;

class RenderableString {
	friend class LiteralText;
	friend class Font;

	struct StringConfig {
		String text;
		unsigned int color = -1;
		unsigned int background = -1;
		/**
		 * 位意义
		 * 1 - bold
		 * 2 - italic
		 * 4 - underline
		 * 8 - strikeThrough
		 * 16 - default bg
		 * 32 - default clr
		 */
		int style = 0;
		FontID idFont = 0;

		explicit StringConfig() = default;

		void reset() noexcept {
			idFont = 0;
			color = -1;
			background = -1;
			style = 0;
		}

		[[nodiscard]] bool isBold() const noexcept { return style & 1; }
		[[nodiscard]] bool isItalic() const noexcept { return style & 2; }
		[[nodiscard]] bool isUnderline() const noexcept { return style & 4; }
		[[nodiscard]] bool isStrikeThrough() const noexcept { return style & 8; }
		[[nodiscard]] bool isDefaultBackground() const noexcept { return style & 16; }
		[[nodiscard]] bool isDefaultColor() const noexcept { return style & 32; }
		void setBold(const bool value) noexcept { style = value ? style | 1 : style & ~1; }
		void setItalic(const bool value) noexcept { style = value ? style | 2 : style & ~2; }
		void setUnderline(const bool value) noexcept { style = value ? style | 4 : style & ~4; }
		void setStrikeThrough(const bool value) noexcept { style = value ? style | 8 : style & ~8; }
		void useDefaultBackground(const bool value) noexcept { style = value ? style | 16 : style & ~16; }
		void useDefaultColor(const bool value) noexcept { style = value ? style | 32 : style & ~32; }

		[[nodiscard]] StringConfig copyConfig() const noexcept {
			StringConfig ret;
			ret.idFont = idFont;
			ret.color = color;
			ret.background = background;
			ret.style = style;
			return ret;
		}

		[[nodiscard]] StringConfig copy() const noexcept {
			StringConfig ret;
			ret.idFont = idFont;
			ret.text = text;
			ret.idFont = idFont;
			ret.color = color;
			ret.background = background;
			ret.style = style;
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
			if (isBold()) ret.append(L"b");
			if (isItalic()) ret.append(L"i");
			if (isUnderline()) ret.append(L"u");
			if (isStrikeThrough()) ret.append(L"s");
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

	RenderableString& append(const RenderableString& other) {
		auto iterator = other.configs.cbegin();
		int flags = 0;
		bool clr = true, bg = true, font = true;
		const unsigned int color = configs.back().color;
		const unsigned int background = configs.back().background;
		const FontID idFont = configs.back().idFont;
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
					config.useDefaultColor(false);
					string += 8;
					continue;
				}
				case L'.': {
					unsigned long long i = 0;
					while (i < 9) if (string[i++] == L'\0') goto end;
					config.background = wtouib16(++string, 8);
					config.useDefaultBackground(false);
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
					config.setStrikeThrough(true);
					break;
				case L'_':
				case L'u':
				case L'U':
					config.setUnderline(true);
					break;
				case L'/':
				case L'i':
				case L'I':
					config.setItalic(true);
					break;
				case L'=':
				case L'b':
				case L'B':
					config.setBold(true);
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
					config.useDefaultColor(false);
					string += 8;
					continue;
				}
				case L'.': {
					if (end - string < 9) break;
					config.background = wtouib16(++string, 8);
					config.useDefaultBackground(false);
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
					config.setStrikeThrough(true);
					break;
				case L'_':
				case L'u':
				case L'U':
					config.setUnderline(true);
					break;
				case L'/':
				case L'i':
				case L'I':
					config.setItalic(true);
					break;
				case L'=':
				case L'b':
				case L'B':
					config.setBold(true);
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
};

inline RenderableString operator""_renderable(const wchar* const text, const QWORD len) noexcept { return RenderableString(text); }

class Font {
	friend class FontManager;
	Map<int, HFONT> fonts;
	long height;
	long escapement;
	long orientation;
	const FontID id;
	bool adaptAllSize = false;
	explicit Font(const FontID id, const long height, const long escapement, const long orientation, const bool adaptAllSize) : height(height), escapement(escapement), orientation(orientation), id(id), adaptAllSize(adaptAllSize) {}

	HFONT tryCreate(const RenderableString::StringConfig& config) {
		if (const auto iter = fonts.find(config.style); iter != fonts.end()) return iter->second;
		LOGFONTW f{
			.lfHeight = height,
			.lfWidth = 0,
			.lfEscapement = escapement,
			.lfOrientation = orientation,
			.lfWeight = config.isBold() ? FW_BOLD : FW_NORMAL,
			.lfItalic = config.isItalic(),
			.lfUnderline = config.isUnderline(),
			.lfStrikeOut = config.isStrikeThrough(),
			.lfCharSet = DEFAULT_CHARSET,
			.lfOutPrecision = OUT_DEFAULT_PRECIS,
			.lfClipPrecision = CLIP_DEFAULT_PRECIS,
			.lfQuality = static_cast<unsigned char>(adaptAllSize ? DEFAULT_QUALITY : PROOF_QUALITY),
			.lfPitchAndFamily = FF_DONTCARE,
			.lfFaceName{}
		};
		memcpy(f.lfFaceName, f.lfFaceName, sizeof(f.lfFaceName));
		HFONT fnt = CreateFontIndirectW(&f);
		fonts.emplace(std::make_pair(config.style, fnt));
		return fnt;
	}

public:
	void draw(const RenderableString& text, int x, int y, unsigned int color);
};

class FontManager {
	Map<FontID, Font*> fonts;
	Font* defaultFont = new Font(1, 30, 0, 0, true);

public:
	FontManager() { fonts.emplace(1, defaultFont); }

	Font& get(const FontID id) {
		if (!fonts.contains(id)) return *defaultFont;
		return *fonts[id];
	}

	void resize(int width, int height) {}
};

interface IText {
	virtual ~IText() = default;
	[[nodiscard]] virtual const String& getText() const noexcept = 0;
	[[nodiscard]] virtual const RenderableString& getRenderableString() const noexcept = 0;
};

typedef class LiteralText final : public IText {
	const String string;
	mutable RenderableString renderableString;

public:
	explicit LiteralText(const String& string): string(string), renderableString(string) {}
	explicit LiteralText(String&& string): string(std::move(string)), renderableString(string) {}

	const String& getText() const noexcept override { return string; }

	const RenderableString& getRenderableString() const noexcept override { return renderableString; }
} TranslatedText;

class TranslatableText final : public IText {
	const String idSrc;
	mutable const LiteralText* target = nullptr;
	mutable QWORD langConfig = 0;

public:
	explicit TranslatableText(const String& id) : idSrc(id) {}
	explicit TranslatableText(String&& id) : idSrc(std::move(id)) {}
	const String& getText() const noexcept override;
	const RenderableString& getRenderableString() const noexcept override;
	void refreshText() const noexcept;
};

struct Language {
	Map<String, TranslatedText> translateTable;
	int id = 1;
};

class Translator {
	Map<String, int> langMap{};
	List<Language> langList{};
	TranslatedText nullText{ L"\\#FF""EE0000<translator-null>" };
	String lang = L"zh-cn";
	int langConfig = 1;
	int idLangMax = 1;
	using IterID = Map<String, TranslatedText>::const_iterator;
	using IterLang = List<Language>::const_iterator;

public:
	explicit Translator() { langMap.insert(std::make_pair(String(L"zh-cn"), 1)); }
	void addLang(const String& lang) noexcept { langMap.insert(std::make_pair(lang, ++idLangMax)); }
	void addLang(String&& lang) noexcept { langMap.insert(std::make_pair(lang, ++idLangMax)); }
	void loadLang();

	[[nodiscard]] const TranslatedText* getText(const String& id) const noexcept {
		for (const auto& [translateTable, _] : langList) {
			IterID iterator = translateTable.find(id);
			if (iterator != translateTable.cend()) return &iterator->second;
		}
		return &nullText;
	}

	[[nodiscard]] int getConfigVersion() const noexcept { return langConfig; }
};

extern Translator translator;
extern FontManager fontManager;
