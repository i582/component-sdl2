#pragma once

#include "SDL.h"
#include "string"
#include "vector"
#include "map"
#include "functional"
#include "any"

#include "../event/event.h"
#include "../tools/texture/texture.h"

namespace Kit
{
    using std::string;
    using std::vector;
    using std::function;
    using std::map;
    using std::any;

    class Component;

    using eventCallback = function<void(Component* sender, Event* e)>;
    using eachCallback = function<void(Component* sender)>;

}