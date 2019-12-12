#include "kit/kit.h"

#include "MyWindowClass/MyWindow.h"
#include "DialogWindow/DialogWindow.h"

using namespace Lib;

int main(int argc, char** argv)
{
	$.addWindow(new MyWindow("new window", { 100, 100, 1000, 500 }));

	$.addWindow(new DialogWindow("", { 300, 300, 560, 95 }));

	$.run();
	return 0;
}
