#pragma once

#include "css-attributes.h"

namespace CSS
{

using std::map;


class css_block_state
{
private:
	map <string, std::variant<int, double, string, Color> > styles;


public:
	css_block_state();
	css_block_state(bool general);

public:
	void mergeWith(css_block_state& block);
	void mergeWithBaseIs(css_block_state& block);

	template<typename T>
	void set(string key, T value);

	template<typename T>
	T get(string key);
};


template<typename T>
void css_block_state::set(string key, T value)
{
	if (key.empty())
		return;

	styles[key] = value;
}

template<typename T>
T css_block_state::get(string key)
{
	T result;

	if (styles.find(key) == styles.end())
	{
		cout << "ERROR: " << key << " is NOT found in this map!" << endl;
		return T(0);
	}

	try
	{
		result = std::get<T>(styles[key]);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		return T(0);
	}

	return result;
}


}