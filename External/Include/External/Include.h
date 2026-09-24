#pragma once
#include <GlobalData/Include.h>
#include <LiquidHookEx/Include.h>
#define IMGUI_API __declspec(dllimport)
#define INCLUDE_OVERLAY
#ifdef INCLUDE_OVERLAY
#include <Overlay/Definitions.h>
#include <Overlay/Overlay.h>
#include <Overlay/Renderer.h>
#include <RenderObjects/Position.h>
#endif