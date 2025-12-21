#pragma once
#include <core/Memory.hpp>
#include <sdk/mc/client/gui/ScreenSizeData.hpp>

#pragma pack(push, 1)
class GuiData {
public:
	// $padding(0x5B);
	$padding(0x40);

	ScreenSizeData mScreenSizeData;
	bool mScreenSizeDataValid;

	$padding(0x2);

	float mGuiScale;
	float mInvGuiScale;		// mathematical inverse of mGuiScale

	$padding(0x1A);

	short mPointerX;
	short mPointerY;
};
#pragma pack(pop)