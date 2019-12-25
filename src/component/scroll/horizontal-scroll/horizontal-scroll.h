#pragma once

#include "../scroll.h"

namespace Kit
{
	class Component;

	class HorizontalScroll : public Scroll
	{
	public:
		HorizontalScroll(SDL_Renderer* renderer, Rect size, int maxValue, double relSizes);

	protected:
		virtual void init();

	public:
		friend Component;

	public:
		void shift(int delta);
	};

}