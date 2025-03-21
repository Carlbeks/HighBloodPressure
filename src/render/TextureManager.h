//
// Created by EmsiaetKadosh on 25-1-21.
//

#pragma once

#include "..\def.h"

class ITexture {};

class TextureManager {
	Map<String, ObjectHolder<ITexture>> textures;
	using IterTexture = Map<String, ObjectHolder<ITexture>>::const_iterator;

public:
	TextureManager() = default;
};

inline static TextureManager textureManager = TextureManager();
