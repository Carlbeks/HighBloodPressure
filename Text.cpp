//
// Created by EmsiaetKadosh on 25-1-18.
//

#include "Text.h"

#include "def.h"

const String& TranslatableText::getText() const noexcept {
	if (target == nullptr || langConfig == translator.getConfigVersion()) {
		target = translator.getText(idSrc);
		langConfig = translator.getConfigVersion();
	}
	return *target;
}


