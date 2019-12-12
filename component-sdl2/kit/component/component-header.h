#pragma once

#include "SDL.h"
#include "string"
#include "vector"
#include "map"
#include "functional"

#include "../event/event.h"

namespace Lib
{
	using std::string;
	using std::vector;
	using std::function;
	using std::map;


	class Component;
	using eventCallback = function< void(Component* sender, Event* e) >;
	using eachCallback = function< void(Component* sender) >;

}