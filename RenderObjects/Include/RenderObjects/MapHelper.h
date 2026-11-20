#pragma once
#include <Overlay/RenderObject.h>
#include <Math/Vector.h>
#include <string>
#include <map>
class View3D;
namespace CrimsonDesert {
    class UIGamePlayControlRootWorldMap;
};
namespace RenderObjects {


    class MapHelper : public RenderObject {
    private:
        CrimsonDesert::UIGamePlayControlRootWorldMap* pRootWorldMap = 0x0;
        bool m_bInitDone = false;
    public:
        MapHelper() {
            printf("MapHelper Render Object constructed...\n");
        };
        void Render(ImVec2 DisplaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D) override;

    };
}