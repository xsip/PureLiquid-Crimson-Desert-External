#pragma once

#ifdef OVERLAY_DEFINITION
#define OVERLAY_API __declspec(dllexport)
#define IMGUI_API __declspec(dllexport)


#else
#define OVERLAY_API __declspec(dllimport)
#define IMGUI_API __declspec(dllimport)
#endif


#include <imgui/imgui.h>

namespace OverlayAPI {
    OVERLAY_API void InitImGuiContext();
    // Expose the context for EXE
    OVERLAY_API ImGuiContext* GetImGuiContext();

    OVERLAY_API void Overlay_BeginFrame();
    OVERLAY_API void Overlay_EndFrame();
}