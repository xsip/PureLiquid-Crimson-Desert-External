#pragma once

#include <Windows.h>

#include <map>
#include <d3d11.h>
#include <tchar.h>
#include <dxgi1_4.h>
#include <Overlay/Definitions.h>


struct ProcData {
public:
    HWND hWnd{};
    int left{};
    int top{};
    int width{};
    int height{};
};
class Renderer;
class OVERLAY_API Overlay {
public:
    static ProcData GetProcData();

    static void RenderDropShadow(ImTextureID tex_id, float size, ImU8 opacity);
    static bool RenderButton(const char* szTitle, bool& checked, ImVec2 pos, ImVec2 size, ImTextureID img = NULL);

     static void Show(HWND hWnd, Renderer* _pRenderer);

private:
    static bool menuOpen;
    static int windowState;
    static HWND hwnd;
    static HWND targetHwnd;
    static WNDCLASS wc;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    static void MessageThread();
    static bool Initialize();

    static const int APP_NUM_FRAMES_IN_FLIGHT = 2;
    static const int APP_NUM_BACK_BUFFERS = 2;
    static const int APP_SRV_HEAP_SIZE = 64;

    static void KeepOverlayed();
    static void ChangeClickability(bool canclick, HWND ownd);

    // ImGui/DirectX
    static ID3D11Device* g_pd3dDevice;
    static ID3D11DeviceContext* g_pd3dDeviceContext;
    static IDXGISwapChain* g_pSwapChain;
    static bool g_SwapChainOccluded;
    static UINT g_ResizeWidth;
    static UINT g_ResizeHeight;
    static ID3D11RenderTargetView* g_mainRenderTargetView;
    static bool bSetupDone;

    static bool CreateDeviceD3D();
    static void CleanupDeviceD3D();
    static void CreateRenderTarget();
    static void CleanupRenderTarget();

};