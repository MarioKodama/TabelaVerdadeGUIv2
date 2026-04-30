#pragma once
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include <dwmapi.h>

//header para declaração de funções e tipos utilizados para a renderização do GUI. 

extern HWND hwnd;

extern IDXGISwapChain* swapchain;
extern ID3D11Device* device;
extern ID3D11DeviceContext* context;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void Init(HINSTANCE hInstance);
void SetupD3D(HWND hWnd);
void ImGuiInit();
void RenderWindow();

 