#include <RenderObjects/MapHelper.h>
#include <CrimsonDesert/Structures/CPosition.h>
#include <CrimsonDesert/Structures/UIGamePlayControlCommonCurrencyIndicator.h>
#include <CrimsonDesert/Structures/ServerTransformSyncActorComponent.h>
#include <CrimsonDesert/Structures/GlobalContext.h>
#include <Overlay/RenderObject.h>
#include <Math/Vector.h>
#include <math/Matrix.h>
#undef min;
#undef max;
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <cstdio>
#include <Overlay/View3D.h>
#include <Windows.h>

bool IsLeftDoubleClick()
{
    static DWORD lastClick = 0;
    static bool wasPressed = false;

    bool pressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

    if (pressed && !wasPressed)
    {
        DWORD now = GetTickCount();
        wasPressed = true;

        if (now - lastClick <= GetDoubleClickTime())
        {
            lastClick = 0;
            return true;
        }

        lastClick = now;
    }

    if (!pressed)
        wasPressed = false;

    return false;
}
namespace RenderObjects {



    void MapHelper::Render(ImVec2 displaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D) {


        if (!m_bInitDone) {
            auto pGlobalCtx = CrimsonDesert::GlobalContext::GetInstance();
            pRootWorldMap = pGlobalCtx->GetUIGamePlayControlRootWorldMap();
            m_bInitDone = true;
        }

        if (!pRootWorldMap->m_bIsHoveringMapItem)
            return;
        auto pActiveIcon = pRootWorldMap->m_hoverdMapIconContainer->m_pIcon;

        if (!pActiveIcon)
            return;

        auto activeMarkerString = pActiveIcon->GetIconName();

        if (activeMarkerString.empty())
            return;

        Vector3 vPosition = pActiveIcon->GetWorldPosition();
        char buf[128];
        snprintf(buf, sizeof(buf),
            "Left doubleclick=Teleport %s\n(%.2f %.2f %.2f )", activeMarkerString.c_str(), vPosition.x,vPosition.y, vPosition.z);


        if (IsLeftDoubleClick()) {
            CrimsonDesert::CPosition::GetInstance()->vLocalPos = vPosition;
        }

        ImGui::GetForegroundDrawList()->AddText(
            ImVec2(cursorPos.x + 10+5, cursorPos.y + 10+5),
            ImColor(255, 0, 255, 230),
            buf
        );



    }

}