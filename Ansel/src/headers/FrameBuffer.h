#pragma once

#include "Def.h"
#include "Texture.h"
#include "Window.h"

namespace Ansel
{
	class FrameBuffer
	{
		unsigned int ID;
		unsigned int RBO;

		Texture* color;
		Texture* depth;

		void generateFrameBuffer(Window* w);

	public:
		ANSEL_API  FrameBuffer(Window* w);
		ANSEL_API ~FrameBuffer();

		Texture ANSEL_API * getColorTexture() const;
		Texture ANSEL_API * getDepthTexture() const;

		void ANSEL_API bind();
		void ANSEL_API unbind();
	};
}