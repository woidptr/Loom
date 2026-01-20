#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/client/gui/GuiData.hpp>

namespace mce {
	class MeshContext;
}

class Tessellator;

class ScreenContext {
public:
	$padding(0xA7);

	GuiData* guiData;

	// $field(GuiData*, guiData);
	$build_access(mce::MeshContext*, meshContext, 0x10);
	$field(Tessellator*, tessellator);
};
