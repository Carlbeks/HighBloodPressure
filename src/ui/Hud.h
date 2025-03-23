//
// Created by EmsiaetKadosh on 25-1-14.
//

#pragma once

#include "..\render\Renderer.h"

class Hud final : public IRenderable, public ITickable {
public:
	void render(double tickDelta) const noexcept override;
	void tick() noexcept override;
	void onResize() noexcept {}
};

