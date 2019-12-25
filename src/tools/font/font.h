#pragma once

#include "SDL_ttf.h"

#include "font-find/font-find.h"

namespace Kit
{
	using std::map;
	using std::string;
	using std::to_string;
	using std::vector;


	class font
	{
	private:
		string path;


	private:
		static map<string, TTF_Font*> _cache;

	public:
		font();
		font(const string& name, font_style style, font_weight weight);
		font(const string& name, const string& style, int weight);
		~font();

	private:
		void init();

	public:
		TTF_Font* at(unsigned int size) const;
		TTF_Font* operator[](unsigned int size) const;

		bool operator==(const font& font);
		font& operator=(const font& font);

		static void clear_cache();

	};

}