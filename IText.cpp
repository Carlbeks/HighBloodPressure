//
// Created by EmsiaetKadosh on 25-1-18.
//

#include "IText.h"

#include "def.h"
#include "Renderer.h"

int RenderableString::getHeight() const noexcept {
	int height = 0;
	for (const StringConfig& config : configs) if (const Font& font = fontManager.get(config.idFont); font.getHeight() > height) height = font.getHeight();
	return height;
}

int RenderableString::getWidth(const FontID defaultID) const noexcept {
	int width = 0;
	for (const StringConfig& config : configs) width += fontManager.get(config.idFont ? config.idFont : defaultID).getWidth(config);
	return width;
}

int RenderableString::getWidth(RenderConfig* renderConfigs, const FontID defaultID) const noexcept {
	int width = 0;
	int i = 0;
	for (const StringConfig& config : configs) {
		renderConfigs[i].config = &config;
		const Font& font = fontManager.get(config.idFont ? config.idFont : defaultID);
		renderConfigs[i].font = &font;
		width += renderConfigs[i].width = font.getWidth(config);
		++i;
	}
	return width;
}

int Font::drawSingle(const RenderableString::StringConfig& config, const int x, const int y, const unsigned int defaultColor) const {
	SelectObject(renderer.canvasDC, tryCreate(config));
	RECT rect{
		.left = x,
		.top = y + yOffsetPx,
		.right = 0,
		.bottom = 0
	};
	SetTextColor(renderer.canvasDC, config.isDefaultColor() ? defaultColor : Renderer::changeColorFormat(config.color));
	DrawTextW(renderer.canvasDC, config.text.c_str(), static_cast<int>(config.text.length()), &rect, DT_SINGLELINE | DT_NOCLIP | DT_CALCRECT);
	if (!config.isDefaultBackground()) renderer.fill(&rect, config.background);
	DrawTextW(renderer.canvasDC, config.text.c_str(), static_cast<int>(config.text.length()), &rect, DT_SINGLELINE | DT_NOCLIP);
	return rect.right;
}

void Font::drawDirect(const RenderableString::StringConfig& config, const int x, const int y, const unsigned int defaultColor) const {
	SelectObject(renderer.canvasDC, tryCreate(config));
	RECT rect{
		.left = x,
		.top = y + yOffsetPx,
		.right = 0,
		.bottom = 0
	};
	SetTextColor(renderer.canvasDC, config.isDefaultColor() ? defaultColor : Renderer::changeColorFormat(config.color));
	if (!config.isDefaultBackground()) renderer.fill(&rect, config.background);
	DrawTextW(renderer.canvasDC, config.text.c_str(), static_cast<int>(config.text.length()), &rect, DT_SINGLELINE | DT_NOCLIP);
}

void Font::clear() const {
	for (const auto& fnt : fonts | std::views::values) renderer.deleteObject(fnt);
	fonts.clear();
}

void Font::draw(const RenderableString& text, int x, const int y, const unsigned int color) const {
	const COLORREF defaultColor = Renderer::changeColorFormat(color);
	for (const RenderableString::StringConfig& config : text.configs) {
		if (config.idFont) {
			x = fontManager.get(config.idFont).drawSingle(config, x, y, defaultColor);
			continue;
		}
		SelectObject(renderer.canvasDC, tryCreate(config));
		RECT rect{
			.left = x,
			.top = y + yOffsetPx,
			.right = 0,
			.bottom = 0
		};
		SetTextColor(renderer.canvasDC, config.isDefaultColor() ? defaultColor : Renderer::changeColorFormat(config.color));
		DrawTextW(renderer.canvasDC, config.text.c_str(), static_cast<int>(config.text.length()), &rect, DT_SINGLELINE | DT_NOCLIP | DT_CALCRECT);
		if (!config.isDefaultBackground()) renderer.fill(&rect, config.background);
		x = rect.right;
		DrawTextW(renderer.canvasDC, config.text.c_str(), static_cast<int>(config.text.length()), &rect, DT_SINGLELINE | DT_NOCLIP);
	}
}

void Font::drawCenter(const RenderableString& text, int x, int y, const int w, const int h, const unsigned int color) const {
	const COLORREF defaultColor = Renderer::changeColorFormat(color);
	using RenderConfig = RenderableString::RenderConfig;
	const QWORD size = text.configs.size();
	RenderConfig* configs = allocatedFor(new RenderConfig[size], sizeof(RenderConfig) * size);
	const int stringWidth = text.getWidth(configs, id);
	x += w - stringWidth >> 1;
	y += h - text.getHeight() >> 1;
	for (QWORD i = 0; i < size; ++i) {
		configs[i].font->drawDirect(*configs[i].config, x, y, defaultColor);
		x += configs[i].width;
	}
	delete[] deallocating(configs);
}

int Font::getWidth(const RenderableString::StringConfig& config) const {
	HFONT font = tryCreate(config);
	RECT rect{};
	SelectObject(renderer.assistDC, font);
	DrawTextW(renderer.assistDC, config.text.c_str(), static_cast<int>(config.text.length()), &rect, DT_CALCRECT | DT_NOCLIP | DT_SINGLELINE);
	return rect.right;
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

void languageMakeChinese(Language& lang) {
	Logger.debug(L"languageMakeChinese called");
	Map<std::wstring, LiteralText>& map = lang.translateTable;
	map.emplace(String(L"hbp.title"), TranslatedText(L"高血压"));
	map.emplace(String(L"hbp.confirm.confirm"), TranslatedText(L"确认"));
	map.emplace(String(L"hbp.confirm.cancel"), TranslatedText(L"取消"));
}

inline Translator translator = Translator();
inline FontManager fontManager = FontManager();
