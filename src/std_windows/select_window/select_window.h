#pragma once

#include "window.h"

#include "button.h"
#include "checkbox.h"
#include "table.h"

using namespace Kit;

class select_window : public window
{
public:
    select_window(const string& title_, const SimpleRect& size_, bool noBorder_);

public:
    void setup() override;

};