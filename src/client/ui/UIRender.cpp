#include "UIRender.hpp"

void UIRender::initImgui(IDXGISwapChain3* swapChain) {
	if (this->initialized) {
		return;
	}

	swapChain->GetDevice(__uuidof(ID3D12Device), (void**)&this->d3d12Device);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D11_CREATE_DEVICE_FLAG deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	HRESULT hr = D3D11On12CreateDevice(
		this->d3d12Device,
		deviceFlags,
		featureLevels,
		_countof(featureLevels),
		(IUnknown**)&this->commandQueue,
		1,
		0,
		&this->d3d11Device,
		&this->d3d11DeviceContext,
		nullptr
	);

	if (FAILED(hr)) {
		Logger::info("D3D11On12CreateDevice failed");
	}
	else
	{
		Logger::info(std::format("D3D11 device: 0x{:X}, context: 0x{:X}", (uintptr_t)this->d3d11Device, (uintptr_t)this->d3d11DeviceContext));
	}

	this->d3d11Device->QueryInterface(__uuidof(ID3D11On12Device), (void**)&this->d3d11On12Device);

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	DXGI_SWAP_CHAIN_DESC desc;
	swapChain->GetDesc(&desc);

	ImGui_ImplWin32_Init(desc.OutputWindow);
	Logger::info(std::format("ImGui window: {}", (void*)desc.OutputWindow));
	// SetWindowLongPtr(desc.OutputWindow, GWLP_WNDPROC, (LONG_PTR)wndProcCallback);
	ImGui_ImplDX11_Init(this->d3d11Device, this->d3d11DeviceContext);

	initialized = true;
}

void UIRender::renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
	if (!this->commandQueue) {
		return;
	}

	this->initImgui(swapChain);

	ID3D12Resource* backBuffer12 = nullptr;
	swapChain->GetBuffer(swapChain->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&backBuffer12));

	D3D11_RESOURCE_FLAGS flags = { D3D11_BIND_RENDER_TARGET };

	ID3D11Resource* backBuffer11 = nullptr;

	this->d3d11On12Device->CreateWrappedResource(
		backBuffer12,
		&flags,
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		IID_PPV_ARGS(&backBuffer11)
	);

	this->d3d11On12Device->AcquireWrappedResources(&backBuffer11, 1);

	ID3D11RenderTargetView* rtv = nullptr;
	this->d3d11Device->CreateRenderTargetView(backBuffer11, nullptr, &rtv);

	this->d3d11DeviceContext->OMSetRenderTargets(1, &rtv, nullptr);

	DXGI_SWAP_CHAIN_DESC desc;
	swapChain->GetDesc(&desc);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (FLOAT)desc.BufferDesc.Width;
	vp.Height = (FLOAT)desc.BufferDesc.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();

	// Draw your menu
	ImGui::ShowDemoWindow();

	// Render
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ID3D11RenderTargetView* nullRTV[1] = { nullptr };
	this->d3d11DeviceContext->OMSetRenderTargets(1, nullRTV, nullptr);
	this->d3d11On12Device->ReleaseWrappedResources(&backBuffer11, 1);
	this->d3d11DeviceContext->Flush();

	backBuffer11->Release();
	backBuffer12->Release();
}

void UIRender::executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* commandList) {
	this->commandQueue = commandQueue;
	this->commandList = commandList;
}