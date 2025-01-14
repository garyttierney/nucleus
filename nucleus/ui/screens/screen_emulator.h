/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/ui/screen.h"

#include "nucleus/ui/widgets/widget_texture.h"

namespace ui {

class ScreenEmulator : public Screen {
public:
    ScreenEmulator(UI* parent);

    virtual void update() override;
};

}  // namespace ui
