#pragma once

#include "../scroll.h"

namespace Kit
{
	class Component;

	class VerticalScroll : public Scroll
	{
	public:
		VerticalScroll(SDL_Renderer* renderer, Rect size, int maxValue, double relSizes);
		
	protected:
		virtual void init();

	public: 
		friend Component;

	public:
		void shift(int delta);
	};

}