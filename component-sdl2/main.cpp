#include "kit/kit.h"

#include "MyWindowClass/MyWindow.h"
using namespace Kit;

int main(int argc, char** argv)
{
	$.addWindow(new MyWindow("new window", { 100, 100, 1100, 650 }));
	
	return $.run();
}
