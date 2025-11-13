#include "../Hook.h"
#include <d3d12.h>
#include <dxgi.h>

enum DirectXVersion {
	D3D12,
	D3D11,
	UNKNOWN,
};

class DirectXHook : public Hook {
public:
	DirectXHook();

	static DirectXVersion getDirectXVersion();
	static uintptr_t locatePresentAddress();
};