#pragma once

#include "../component.h"


namespace Lib
{



class Components
{
private:
	vector <Component*>* components;

public:
	Components(vector <Component*>* components);
	~Components();

public:
	void addEventListener(string action, eventCallback event_callback);
	void each(eachCallback each_callback);

	Component* at(size_t index);
	Component* operator[](size_t index);
};



}