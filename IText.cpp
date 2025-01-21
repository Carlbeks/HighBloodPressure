//
// Created by EmsiaetKadosh on 25-1-18.
//

#include "IText.h"

#include "def.h"

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


