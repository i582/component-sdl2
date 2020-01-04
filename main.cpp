#include "src/kit.h"

#include "simple-window.h"

using namespace Kit;

int main(int argc, char** argv)
{
    $.addWindow(new SimpleWindow("Results", { 100, 100, 390, 460 }));
    return $.run();
}

