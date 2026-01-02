#pragma once
#include <sdk/mc/client/gui/GuiData.hpp>
#include <sdk/mc/client/render/Tessellator.hpp>

class ScreenContext {
public:
	// $padding(0xA7);

	// GuiData* guiData;

	$field(GuiData*, guiData);
	$field(Tessellator*, tessellator);

	// $padding(0x8);

	// Tessellator* tessellator;
};
