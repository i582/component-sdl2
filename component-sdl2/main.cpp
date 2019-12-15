#include "kit/kit.h"

#include "MyWindowClass/MyWindow.h"
using namespace Lib;

int main(int argc, char** argv)
{
	$.addWindow(new MyWindow("new window", { 100, 100, 1000, 500 }));

	$.run();
	return 0;
}
