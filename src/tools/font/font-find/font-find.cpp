#include "font-find.h"

using namespace std;
using namespace Kit;

vector<font_cache> Kit::font_find::_cache = {};

string Kit::font_find::to_lower(const string& str)
{
	string str_result = str;

	for (auto& symbol : str_result)
	{
		symbol = tolower(symbol);
	}

	return str_result;
}

Kit::font_find::font_find(const string& font_name, font_style font_style, font_weight font_weight)
{
	this->_font_name = to_lower(font_name);
	this->_font_style = font_style;
	this->_font_weight = font_weight;


	this->_equivalent_names = {
		{"segoe script", "segoesc"},
		{"segoe print", "segoepr"},
		{"segoe ui", "segoeui"},

		{"consolas", "consola"},

		{"comic sans", "comic"},

		{"courier", "cour"},

		{"times new roman", "times"},

		{"trebuchet", "trebuc"},

		{"palatino", "pala"},
	};

	this->_count_font_weights = 9;
	this->_font_weight_string = {
		{"hairline"},
		{"thin", "extralight"},
		{"light"},
		{"regular", "book"},
		{"medium"},
		{"semibold"},
		{"bold"},
		{"extrabold"},
		{"black"},
	};

	this->_font_weight_found = { false, false, false, false, false, false, false, false, false};
	this->_font_weight_found_path.resize(this->_count_font_weights);
}

string Kit::font_find::find()
{
	// replace font names with the names under which they are written in the font folder
	if (_equivalent_names.find(_font_name) != _equivalent_names.end())
	{
		_font_name = _equivalent_names[_font_name];
	}



	// check availability in cache
	font_cache needle_font = font_cache(_font_name, _font_style, _font_weight, "");

	const auto it = std::find(_cache.begin(), _cache.end(), needle_font);
	if (it != _cache.end())
	{
		return it->path();
	}



	const string relative_font_directory = "\\Windows\\Fonts";
	string font_directory;

	// look for where the system fonts in windows are
	for (char i = 'a'; i <= 'z'; i++)
	{
		const string disc(1, i);
		const string string_path = disc + ":" + relative_font_directory;
		const fs::path current_path = string_path;
		if (fs::exists(current_path))
		{
			font_directory = current_path.generic_string();
			break;
		}
	}

	if (font_directory.empty())
	{
		cout << "ERROR: system folder with fonts not found!" << endl;
		return "";
	}


	vector<fs::path> paths;

	// try recursive search in current path
	const fs::path current_path = fs::current_path();

	for (auto& p : fs::recursive_directory_iterator(current_path))
	{
		if (p.path().filename().extension() == ".ttf" ||
			p.path().filename().extension() == ".otf")
		{
			const string& path = to_lower(p.path().filename().generic_string());

			if (path.find(_font_name) != -1)
			{
				paths.push_back(p.path());
			}
		}
	}

	// try directory search in system font folder
	for (auto& p : fs::directory_iterator(font_directory))
	{
		if (p.path().filename().extension() == ".ttf" ||
			p.path().filename().extension() == ".otf")
		{
			const string& path = to_lower(p.path().filename().generic_string());

			if (path.find(_font_name) != -1)
			{
				paths.push_back(p.path());
			}
		}
	}

	if (paths.empty())
	{
		cout << "ERROR: Font not found!" << endl;
		return "";
	}
	
	vector<fs::path> paths_with_style;

	bool italic_font = false;

	switch (_font_style)
	{
	case Kit::font_style::NORMAL:
	{
		for (auto& path : paths)
		{
			const string& filename = to_lower(path.filename().generic_string());
            const string& filename_without_ext = to_lower(path.filename().stem().generic_string());

			if (filename_without_ext.find("italic") == -1 &&
				filename_without_ext.find("it") == -1 &&
				filename_without_ext.find("slanted") == -1)
			{
				paths_with_style.push_back(path);
			}
		}
		break;
	}

	case Kit::font_style::ITALIC:
	{
		for (auto& path : paths)
		{
            const string& filename = to_lower(path.filename().generic_string());
            const string& filename_without_ext = to_lower(path.filename().stem().generic_string());

			if (filename_without_ext.find("italic") != -1 ||
				filename_without_ext[filename_without_ext.size() - 1] == 'i' || 
				filename_without_ext[filename_without_ext.size() - 1] == 'z' ||
				filename_without_ext.find("it") != -1 ||
				filename_without_ext.find("slanted") != -1)
			{
				paths_with_style.push_back(path);
				italic_font = true;
			}
		}
		break;
	}

	default:break;
	}

	if (paths_with_style.empty())
	{
		cout << "ERROR: Font with style not found!" << endl;
		return "";
	}



	vector<fs::path> paths_with_weight;

	
	for (auto& path : paths_with_style)
	{
        const string& filename_without_ext = to_lower(path.filename().stem().generic_string());

		for (size_t i = 0; i < _count_font_weights; i++)
		{
			vector<string> _font_weight_possible = _font_weight_string[i];
			bool find_this = false;

			for (auto& _font_weight_now : _font_weight_possible)
			{
				if (filename_without_ext.find(_font_weight_now) != -1)
				{
					find_this = true;
					break;
				}
			}
	
			if (find_this)
			{
				_font_weight_found[i] = true;
				_font_weight_found_path[i] = path;
			}
		}
	}



	for (auto& path : paths_with_style)
	{
        const string& filename_without_ext = to_lower(path.filename().stem().generic_string());

		// if font_name like "consola" and file_name like "consolai" or "consolaz"
		if (filename_without_ext.size() - _font_name.size() < 3)
		{
			// get "i" or "z" as in the example above
            const string& _font_modifiers = filename_without_ext.substr(_font_name.size(), filename_without_ext.size() - _font_name.size());

			if ((_font_modifiers.find('b') != -1 &&
				_font_modifiers.find('i') != -1 && italic_font) ||
				_font_modifiers.find('b') != -1 ||
				(_font_modifiers.find('z') != -1 && italic_font) || _font_modifiers.find("bd") != -1)
			{
				_font_weight_found[(int)font_weight::BOLD] = true;
				_font_weight_found_path[(int)font_weight::BOLD] = path;
			}
			else if (_font_modifiers.empty() || (_font_modifiers.find('i') != -1 && italic_font))
			{
				_font_weight_found[(int)font_weight::REGULAR] = true;
				_font_weight_found_path[(int)font_weight::REGULAR] = path;
			}

			
		}
	}



	int _needle_font_weight = (int)_font_weight;

	if (_font_weight_found[_needle_font_weight] == false)
	{
		for (size_t i = _needle_font_weight; i < _count_font_weights; i++)
		{
			if (_font_weight_found[i] == true)
			{
				cout << "Warning: a font with that weight was not found; the closest font was returned!" << endl;

				// cache
				_cache.push_back(font_cache(_font_name, _font_style, _font_weight, _font_weight_found_path[i].generic_string()));

				return _font_weight_found_path[i].generic_string();
			}
		}

		cout << "ERROR: font with this style not exist!" << endl;
	}
	else
	{
		// cache
		_cache.push_back(font_cache(_font_name, _font_style, _font_weight, _font_weight_found_path[_needle_font_weight].generic_string()));

		return _font_weight_found_path[_needle_font_weight].generic_string();
	}


	return "";
}

void Kit::font_find::delete_cache()
{
	_cache.clear();
}


Kit::font_cache::font_cache(const string& _font_name, font_style _font_style, font_weight _font_weight, const string& _path)
{
	this->_font_name = _font_name;
	this->_font_style = _font_style;
	this->_font_weight = _font_weight;

	this->_path = _path;
}

bool Kit::font_cache::operator==(const font_cache& cache)
{
	return	
		_font_name == cache._font_name && 
		_font_style == cache._font_style &&
		_font_weight == cache._font_weight;
}

string Kit::font_cache::path()
{
	return _path;
}
