#pragma once
#include <vector>
#include <Overlay/RenderObject.h>
struct ImVec2;
class View3D;
class Renderer {
public:
	std::vector<RenderObject*> renderObjects;
	View3D* pView3D{};
public:
	Renderer(std::vector<RenderObject*> _renderObjects) {
		renderObjects = _renderObjects;
	}
	void AddRenderObject(RenderObject* obj) {
		renderObjects.push_back(obj);
	}

	View3D* GetView3D() {
		return pView3D;
	}

	void SetView3D(View3D* pView) {
		if (pView3D)
			return;

		pView3D = pView;
	}


private:
	static void RenderWatermark(ImVec2 displaySize);
public:
	void Render(ImVec2 displaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D);
};