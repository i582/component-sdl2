#pragma once

#include "../component.h"

namespace Kit
{
class Window;

class Navigator : public Component
{
public:
	Navigator(Window* parent);

public:
	void update();

};

}