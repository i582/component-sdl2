#include "font.h"

using namespace Kit;
using namespace std;

map<string, TTF_Font*> Kit::font::_cache = {};

Kit::font::font() : font("Segoe UI", font_style::NORMAL, font_weight::REGULAR) {}

Kit::font::font(const string& name, font_style style, font_weight weight)
{
	init();

	font_find find(name, style, weight);
	path = find.find();
}

Kit::font::font(const string& name, const string& style, int weight)
{
	init();

	font_style _style = font_style::NORMAL;
	font_weight _weight = font_weight::REGULAR;

	if (style == "normal") _style = font_style::NORMAL;
	if (style == "italic") _style = font_style::ITALIC;

	if (weight == 100) _weight = font_weight::ULTRATHIN;
	if (weight == 200) _weight = font_weight::THIN;
	if (weight == 300) _weight = font_weight::LIGHT;
	if (weight == 400) _weight = font_weight::REGULAR;
	if (weight == 500) _weight = font_weight::MEDIUM;
	if (weight == 600) _weight = font_weight::SEMIBOLD;
	if (weight == 700) _weight = font_weight::BOLD;
	if (weight == 800) _weight = font_weight::EXTRABOLD;
	if (weight == 900) _weight = font_weight::BLACK;


	font_find find(name, _style, _weight);
	path = find.find();
}

Kit::font::~font()
{
	
}

void Kit::font::init()
{
	if (!TTF_WasInit())
	{
		if (TTF_Init() == -1)
		{
			cout << "Error: TTF wasn't init!" << endl;
		}
	}
}

TTF_Font* Kit::font::at(unsigned int size) const
{
	string cache_path = path + "@" + to_string(size);

	// check availability in cache
	const auto it = _cache.find(cache_path);
	if (it != _cache.end())
	{
		return it->second;
	}


	if (path.empty())
	{
		cout << "ERROR: empty path" << endl;
		return nullptr;
	}

	TTF_Font* ttf_font = TTF_OpenFont(path.c_str(), size);

	if (ttf_font == nullptr)
	{
		cout << "ERROR: " << TTF_GetError() << endl;
		return nullptr;
	}

	_cache.insert(std::make_pair(cache_path, ttf_font));

	return ttf_font;
}

TTF_Font* Kit::font::operator[](unsigned int size) const
{
	return at(size);
}

bool Kit::font::operator==(const font& font)
{
	return path == font.path;
}

font& Kit::font::operator=(const font& font)
{
	this->path = font.path;

	return *this;
}

void Kit::font::clear_cache()
{
	for (auto& _cache_font : _cache)
	{
		TTF_CloseFont(_cache_font.second);
	}

	_cache.clear();
}
