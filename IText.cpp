//
// Created by EmsiaetKadosh on 25-1-18.
//

#include "IText.h"

#include "def.h"
#include "Renderer.h"

void Font::draw(const RenderableString& text, int x, int y, unsigned int color) {
	int lastStyle = -1;
	HFONT lastFont = nullptr;
	COLORREF defaultColor = Renderer::changeColorFormat(color);
	for (const RenderableString::StringConfig& config : text.configs) {
		if (config.style != lastStyle) {
			lastStyle = config.style;
			lastFont = tryCreate(config);
		}
		if (lastFont == nullptr) lastFont = tryCreate(config);
		SelectObject(renderer.MainDC, lastFont);
		RECT rect {
			.left = x,
			.top = y,
			.right = 0,
			.bottom = 0
		};
		DrawTextW(renderer.MainDC, config.text.c_str(), static_cast<int>(config.text.length()), &rect, DT_SINGLELINE | DT_NOCLIP);
		SetTextColor(renderer.MainDC, config.isDefaultColor() ?  defaultColor : Renderer::changeColorFormat(config.color));
		Logger.log(L"rect.right = " + std::to_wstring(rect.right));
	}
}

const String& TranslatableText::getText() const noexcept {
	refreshText();
	return target->getText();
}

const RenderableString& TranslatableText::getRenderableString() const noexcept {
	refreshText();
	return target->getRenderableString();
}

void TranslatableText::refreshText() const noexcept {
	if (target == nullptr || langConfig == translator.getConfigVersion()) {
		target = translator.getText(idSrc);
		langConfig = translator.getConfigVersion();
	}
}

inline Translator translator = Translator();
inline FontManager fontManager = FontManager();
