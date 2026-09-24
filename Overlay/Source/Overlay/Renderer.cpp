#include <Overlay/Definitions.h>
#include <imgui/imgui.h>
#include <Overlay/Renderer.h>
#include <Overlay/RenderObject.h>
#include <Overlay/View3D.h>

void Renderer::RenderWatermark(ImVec2 displaySize) {
	auto pDrawList = ImGui::GetForegroundDrawList();
	pDrawList->AddText(ImVec2(5, 5), ImColor(255, 0, 255, 230), "xsip's PureLiquid v0.1");
}

void Renderer::Render(ImVec2 displaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D) {
	RenderWatermark(displaySize);
	for (auto& renderObject : renderObjects) {
		renderObject->Render(displaySize, cursorPos, bMouseReleased, view3D);
	}
}