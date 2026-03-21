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
	// $field(Tessellator*, tessellator);

	// member fields

	/// @sig {48 8B 9A ? ? ? ? C6 44 24 ? ? 41 B9 04 00 00 00 41 B0 01 48 8B CB E8 ? ? ? ? 0F 57 DB}
	/// @type field
	Tessellator* tessellator();
};
