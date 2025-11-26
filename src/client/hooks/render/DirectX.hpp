#pragma once
#include "../Hook.hpp"
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d12.h>
#include <core/Logger.hpp>

using Microsoft::WRL::ComPtr;

class DirectXBoilerplate {
public:
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12CommandQueue> queue;
};

class PresentHook : public Hook<HRESULT, IDXGISwapChain3*, UINT, UINT> {
public:
	static uintptr_t getAddress() {
		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = DefWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = "Kiero";
		windowClass.hIconSm = NULL;

		RegisterClassEx(&windowClass);

		HWND window = CreateWindow(windowClass.lpszClassName, "Kiero DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

		HMODULE libDXGI = GetModuleHandle("dxgi.dll");
		HMODULE libD3D12 = GetModuleHandle("d3d12.dll");

		if (libDXGI == nullptr || libD3D12 == nullptr) {
			return 0;
		}

		void* createDXGIFactory = GetProcAddress(libDXGI, "CreateDXGIFactory");
		IDXGIFactory* factory;
		((long(__stdcall*)(const IID&, void**))(createDXGIFactory))(__uuidof(IDXGIFactory), (void**)&factory);
		IDXGIAdapter* adapter;
		factory->EnumAdapters(0, &adapter);

		void* D3D12CreateDevice = GetProcAddress(libD3D12, "D3D12CreateDevice");
		ID3D12Device* device;
		((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&device);

		D3D12_COMMAND_QUEUE_DESC queueDesc;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Priority = 0;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask = 0;

		ID3D12CommandQueue* commandQueue;
		device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue);

		ID3D12CommandAllocator* commandAllocator;
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator);

		ID3D12GraphicsCommandList* commandList;
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList);

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = window;
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* swapChain;
		factory->CreateSwapChain(commandQueue, &swapChainDesc, &swapChain);

		void** swapChainVTable = *reinterpret_cast<void***>(swapChain);
		void* presentPtr = swapChainVTable[8];

		return (uintptr_t)presentPtr;
	}

	PresentHook() : Hook("DirectXHook", getAddress()) {
		this->hook();
	}
};

class ExecuteCommandListHook : public Hook<void, ID3D12CommandQueue*, UINT, ID3D12CommandList*> {
public:
	static uintptr_t getAddress() {
		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = DefWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = "Kiero";
		windowClass.hIconSm = NULL;

		RegisterClassEx(&windowClass);

		HWND window = CreateWindow(windowClass.lpszClassName, "Kiero DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

		HMODULE libDXGI = GetModuleHandle("dxgi.dll");
		HMODULE libD3D12 = GetModuleHandle("d3d12.dll");

		if (libDXGI == nullptr || libD3D12 == nullptr) {
			return 0;
		}

		void* createDXGIFactory = GetProcAddress(libDXGI, "CreateDXGIFactory");
		IDXGIFactory* factory;
		((long(__stdcall*)(const IID&, void**))(createDXGIFactory))(__uuidof(IDXGIFactory), (void**)&factory);
		IDXGIAdapter* adapter;
		factory->EnumAdapters(0, &adapter);

		void* D3D12CreateDevice = GetProcAddress(libD3D12, "D3D12CreateDevice");
		ID3D12Device* device;
		((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&device);

		D3D12_COMMAND_QUEUE_DESC queueDesc;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Priority = 0;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask = 0;

		ID3D12CommandQueue* commandQueue;
		device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue);

		ID3D12CommandAllocator* commandAllocator;
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator);

		ID3D12GraphicsCommandList* commandList;
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList);

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = window;
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* swapChain;
		factory->CreateSwapChain(commandQueue, &swapChainDesc, &swapChain);

		void** queueVTable = *(void***)commandQueue;
		void* executeCommandListsPtr = queueVTable[10];

		return (uintptr_t)executeCommandListsPtr;
	}

	ExecuteCommandListHook() : Hook("ID3D12CommandQueue::ExecuteCommandLists", getAddress()) {
		this->hook();
	}
};

//class ResizeBuffersHook : public Hook<HRESULT, IDXGISwapChain3, UINT, UINT, UINT, DXGI_FORMAT, UINT> {
//public:
//	ResizeBuffersHook() : Hook("IDXGISwapChain::ResizeBuffers", 0) {
//
//	}
//};