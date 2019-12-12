#include "MyComponent.h"

MyComponent::MyComponent(string id, Rect size, string classes)
	: Component(id, size, classes)
{
	setup();
}

void MyComponent::setup()
{
	append({ 

		new Component(_id + "__left-side", { "0px", "0px", "50%", "100%" }, ".left-col"),
		new Component(_id + "__right-side", { "50%", "0px", "50%", "100%" }, ".right-col")

	});
	
	this->_childrens[0]->addEventListener("click", [](Component* sender, Event* e)
	{
		cout << "Click left-col" << endl;
	});
}
