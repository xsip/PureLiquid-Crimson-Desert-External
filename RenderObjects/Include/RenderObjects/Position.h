#pragma once
#include <Overlay/RenderObject.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
class View3D;
namespace RenderObjects {
    class Position : public RenderObject {
    public:
        Position() {
            printf("Position Render Object constructed...\n");
        };
        void Render(ImVec2 DisplaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D) override;

    };
}