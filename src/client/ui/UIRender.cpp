#include "UIRender.hpp"

#include <client/Client.hpp>
#include <imgui_internal.h>

UIRender::UIRender(WindowProcHook* windowProcHook, PresentHook* presentHook, ExecuteCommandListHook* executeCommandListHook, ResizeBuffersHook* resizeBuffersHook) {
	windowProcHook->registerCallback(
		[&](CallbackContext& cbCtx, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

			switch (msg) {
			case WM_KEYDOWN:
				keyboardCallback((int16_t)wParam, true);
				break;
			case WM_KEYUP:
				keyboardCallback((int16_t)wParam, false);
			}
			// Logger::info(std::format("Window proc message: {}", msg));
		}
	);

	presentHook->registerCallback(
		[&](CallbackContext& cbCtx, IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
			renderCallback(swapChain, a1, a2);
		}
	);

	executeCommandListHook->registerCallback(
		[&](CallbackContext& cbCtx, ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* commandList) {
			executeCommandListCallback(commandQueue, a1, commandList);
		}
	);

	resizeBuffersHook->registerCallback(
		[&](CallbackContext& cbCtx, IDXGISwapChain3* swapChain, UINT a1, UINT a2, UINT a3, DXGI_FORMAT format, UINT a4) {
			resizeBuffersCallback(swapChain, a1, a2, a3, format, a4);
		}
	);
}

void UIRender::keyboardCallback(int16_t key, bool isDown) {
	if (key == VK_TAB && !isDown) {
		this->draw_main_ui_flag = !this->draw_main_ui_flag;
	}

	if (key == VK_SHIFT && !isDown) {
		ToastManager::addToast("Test", 3.0f);
	}
}

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
	/*else
	{
		Logger::info(std::format("D3D11 device: 0x{:X}, context: 0x{:X}", (uintptr_t)this->d3d11Device, (uintptr_t)this->d3d11DeviceContext));
	}*/

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
	ImGui_ImplDX11_Init(this->d3d11Device, this->d3d11DeviceContext);

	initialized = true;
}

void UIRender::drawSetupScreen() {
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.7f, 0.8f));

	if (ImGui::Button("Settings", ImVec2(120, 90))) {

	}

	ImGui::PopStyleColor(1);
}

void DrawModuleItem(Module* mod) {
	ImVec2 cardSize = ImVec2(180.f, 180.f);
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems) return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// --- Geometry Setup ---
	ImVec2 p_min = ImGui::GetCursorScreenPos();
	ImVec2 p_max = ImVec2(p_min.x + cardSize.x, p_min.y + cardSize.y);
	ImRect totalRect(p_min, p_max);

	float bottomButtonHeight = 40.0f; // Height of the green "ENABLED" area
	float topAreaHeight = cardSize.y - bottomButtonHeight;
	float cardRounding = 12.0f;

	// Define the split rectangles
	ImRect topRect(p_min, ImVec2(p_max.x, p_min.y + topAreaHeight));
	ImRect bottomRect(ImVec2(p_min.x, p_max.y - bottomButtonHeight), p_max);

	// --- IDs for interactions ---
	ImGuiID topID = window->GetID((mod->getName() + "_top").c_str());
	ImGuiID bottomID = window->GetID((mod->getName() + "_bottom").c_str());

	// ========================================================
	// Interaction Logic (Invisible Buttons)
	// ========================================================

	// 1. Top Area Interaction (Click to go to options)
	ImGui::SetCursorScreenPos(topRect.Min);
	if (ImGui::InvisibleButton(("##" + mod->getName() + "_top_btn").c_str(), topRect.GetSize())) {
		// Logic to open settings screen goes here
		Logger::info(std::format("Opened settings for: ", mod->getName()));
	}
	bool topHovered = ImGui::IsItemHovered();
	bool topActive = ImGui::IsItemActive();

	// 2. Bottom Area Interaction (Click to toggle enable)
	ImGui::SetCursorScreenPos(bottomRect.Min);
	if (ImGui::InvisibleButton(("##" + mod->getName() + "_bottom_btn").c_str(), bottomRect.GetSize())) {
		mod->enabled = !mod->enabled;
	}
	bool bottomHovered = ImGui::IsItemHovered();
	bool bottomActive = ImGui::IsItemActive();


	// ========================================================
	// Rendering Logic (Manual Drawing)
	// ========================================================

	// --- 1. Draw Main Card Background & Border ---
	ImU32 cardBgCol = IM_COL32(30, 30, 35, 255); // Dark background
	ImU32 borderColor = IM_COL32(60, 60, 65, 255); // Subtle lighter border

	// Draw overall background with rounding on all corners
	draw_list->AddRectFilled(totalRect.Min, totalRect.Max, cardBgCol, cardRounding);

	// Optional: Visual feedback when hovering top area
	if (topHovered) {
		draw_list->AddRectFilled(topRect.Min, topRect.Max, IM_COL32(255, 255, 255, 15), cardRounding, ImDrawFlags_RoundCornersTop);
	}

	// --- 2. Draw Top Content (Icon & Text) ---
	// Center points for top area
	ImVec2 topCenter = topRect.GetCenter();

	// B. Draw Module Name
	ImVec2 textSize = ImGui::CalcTextSize(mod->getName().c_str());
	ImVec2 textPos = ImVec2(topCenter.x - textSize.x * 0.5f, topCenter.y + 15.0f); // Shift down below icon
	draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), mod->getName().c_str());

	// --- 3. Draw Separator Line ---
	draw_list->AddLine(bottomRect.Min, ImVec2(bottomRect.Max.x, bottomRect.Min.y), IM_COL32(50, 50, 55, 255), 1.0f);

	// --- 4. Draw Bottom Button Area ---
	ImU32 bottomBgCol;
	ImU32 bottomTextCol = IM_COL32(255, 255, 255, 255);
	const char* statusText;

	if (mod->enabled) {
		// Green color when enabled
		bottomBgCol = bottomActive ? IM_COL32(35, 130, 75, 255) : IM_COL32(46, 174, 98, 255);
		statusText = "ENABLED";
	}
	else {
		// Dark gray/red color when disabled
		bottomBgCol = bottomActive ? IM_COL32(50, 50, 50, 255) : IM_COL32(70, 70, 70, 255);
		statusText = "DISABLED";
		bottomTextCol = IM_COL32(180, 180, 180, 255);
	}

	// IMPORTANT: Draw rect with rounding ONLY on the bottom corners so it fits perfectly.
	draw_list->AddRectFilled(bottomRect.Min, bottomRect.Max, bottomBgCol, cardRounding, ImDrawFlags_RoundCornersBottom);

	// Draw status text centered in bottom area
	ImVec2 statusTextSize = ImGui::CalcTextSize(statusText);
	ImVec2 bottomCenter = bottomRect.GetCenter();
	draw_list->AddText(ImVec2(bottomCenter.x - statusTextSize.x * 0.5f, bottomCenter.y - statusTextSize.y * 0.5f), bottomTextCol, statusText);

	// --- 5. Draw Final Outer Border ---
	// drawn last to overlay everything nicely
	draw_list->AddRect(totalRect.Min, totalRect.Max, borderColor, cardRounding, 0, 1.5f);

	// Advance cursor space so the next item doesn't overlap
	ImGui::SetCursorScreenPos(p_max);
	// Add a little padding below the card
	ImGui::Dummy(ImVec2(0, 10));
}

void UIRender::drawSettingsScreen() {
	if (!this->draw_main_ui_flag) {
		return;
	}

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 20.0f;
	style.FrameRounding = 12.0f;
	style.GrabRounding = 12.0f;

	style.WindowPadding = ImVec2(20, 20);
	style.FramePadding = ImVec2(10, 6);

	// ImVec4 backgroundColor = ImVec4(0.10f, 0.10f, 0.11f, 0.95f);
	// ImGui::PushStyleColor(backgroundColor);

	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImVec2 windowSize(900, 500);

	const float baseW = 900.0f;
	const float baseH = 500.0f;

	float scale = min(viewport->Size.x / baseW, viewport->Size.y / baseH);

	scale = min(scale, 1.0f);

	ImVec2 size(baseW * scale, baseH * scale);

	ImVec2 centerPos = ImVec2(
		viewport->Pos.x + (viewport->Size.x - windowSize.x) * 0.5f,
		viewport->Pos.y + (viewport->Size.y - windowSize.y) * 0.5f
	);

	ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);

	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Loom", nullptr, windowFlags)) {
		static char search[128] = "";

		ImGui::SetNextItemWidth(300);
		ImGui::InputTextWithHint("##search", "Search", search, sizeof(search));

		ImGui::BeginChild("ModulesRegion", ImVec2(0, 0), false);

		if (ImGui::BeginTable("ModulesGrid", 4, ImGuiTableFlags_PadOuterX)) {
			for (const std::unique_ptr<Module>& mod : Client::getModules()) {
				ImGui::TableNextColumn();
				DrawModuleItem(mod.get());
			}

			ImGui::EndTable();
		}

		ImGui::EndChild();
	}

	ImGui::End();
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

	ToastManager::renderToasts();
	// this->drawSetupScreen();
	this->drawSettingsScreen();

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

void UIRender::resizeBuffersCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2, UINT a3, DXGI_FORMAT format, UINT a4) {
	this->initialized = false;
	this->initImgui(swapChain);
}