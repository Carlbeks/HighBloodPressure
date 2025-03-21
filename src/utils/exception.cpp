//
// Created by EmsiaetKadosh on 25-1-22.
//

#pragma once

#include "..\def.h"
#include "exception.h"
#include "..\game\Game.h"

String PublicLogger::build(const String& msg, const String& type) const {
	return L"T-" + qwtowb10(game.getTick(), 8) + name + type + msg + L"\n";
}
