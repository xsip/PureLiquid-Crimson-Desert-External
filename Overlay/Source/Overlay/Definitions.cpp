#include <Overlay/Definitions.h>
#include <imgui/imgui.h>
#include <Windows.h>
#include <iostream>

#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>

namespace OverlayAPI {
    // In DLL (Overlay.cpp)
    static ImGuiContext* g_ctx = nullptr;

    void InitImGuiContext()
    {
        g_ctx = ::ImGui::CreateContext();
        ::ImGui::SetCurrentContext(g_ctx);

        // Initialize your backend here
        // ImGui_ImplWin32_Init(hwnd);
        // ImGui_ImplDX11_Init(device, deviceContext);
    }

    // Expose the context for EXE
    ImGuiContext* GetImGuiContext()
    {
        return g_ctx;
    }

    void Overlay_BeginFrame()
    {
        ImGui::SetCurrentContext(g_ctx);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void Overlay_EndFrame()
    {
        ImGui::SetCurrentContext(g_ctx);
    }
}