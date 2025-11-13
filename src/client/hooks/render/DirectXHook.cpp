#include "DirectXHook.h"

DirectXHook::DirectXHook() : Hook("DirectXHook", DirectXHook::locatePresentAddress()) {

}

DirectXVersion DirectXHook::getDirectXVersion() {
	if (GetModuleHandleA("d3d12.dll") != nullptr || GetModuleHandleA("dxgi.dll") != nullptr) {
		return DirectXVersion::D3D12;
	}
	else if (GetModuleHandleA("d3d11.dll") != nullptr) {
		return DirectXVersion::D3D11;
	}

	return DirectXVersion::UNKNOWN;
}

uintptr_t DirectXHook::locatePresentAddress() {
	/*switch getDirectXVersion() {
	case DirectXVersion::D3D12:
		IDXGISwapChain* swapChain;
	case DirectXVersion::D3D11:
		IDXGISwapChain* swapChain;
	}*/

	return 1;
}
