//
// Created by EmsiaetKadosh on 25-1-21.
//

#pragma once

#include "def.h"
#include "exception.h"

class ITexture {};

class Font {
	String name;
	HFONT fonts[16]{};

	static int idOf(const bool italic, const bool bold, const bool underline, const bool strike) {
		int ret = 0;
		if (italic) ret |= 8;
		if (bold) ret |= 4;
		if (underline) ret |= 2;
		if (strike) ret |= 1;
		return ret;
	}

public:
	explicit Font(const String& name) : name(name) {}

	void load(const int size) noexcept(false) {
		LOGFONTW font{
			.lfHeight = size,
			.lfWidth = 0,
			.lfEscapement = 0,
			.lfOrientation = 0,
			.lfCharSet = ANSI_CHARSET,
			.lfOutPrecision = OUT_DEFAULT_PRECIS,
			.lfClipPrecision = CLIP_DEFAULT_PRECIS,
			.lfQuality = DEFAULT_QUALITY,
			.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
		};
		if (name.size() < LF_FACESIZE) memcpy(font.lfFaceName, name.c_str(), name.size());
		else throw ArrayIndexOutOfBoundException(L"Font name is too long: " + name);
		for (int i = 0; i < 16; i++) {
			font.lfWeight = i & 4 ? 700 : 400;
			font.lfItalic = i & 8;
			font.lfUnderline = i & 2;
			font.lfStrikeOut = i & 1;
			fonts[i] = CreateFontIndirectW(&font);
		}
	}

	void unload() noexcept(false) {
		for (int i = 0; i < 16; ++i) {
			DeleteObject(fonts[i]);
			fonts[i] = nullptr;
		}
	}
};

class TextureManager {
	Map<int, Font> fonts;
	Map<String, ObjectHolder<ITexture>> textures;
	Font* defaultFont = nullptr;
	using IterFont = Map<int, Font>::const_iterator;
	using IterTexture = Map<String, ObjectHolder<ITexture>>::const_iterator;
public:
	TextureManager() {
		try {
			fonts.insert(std::make_pair(0, Font(L"Arial")));
			defaultFont = &fonts.at(0);
		}
		catch (...) {

		}
	}
	Font& getFont(const int id) const noexcept {
		IterFont iterator = fonts.find(id);
		if (iterator == fonts.cend()) return
	}
};

inline static TextureManager textureManager = TextureManager();
