#pragma once

#include "window.h"

#include "button.h"
#include "checkbox.h"
#include "select.h"
#include "table.h"

using namespace Kit;

class simple_window : public window
{
public:
    simple_window(const string& title_, SimpleRect size_, bool noBorder_);

public:
    void setup() override;

};