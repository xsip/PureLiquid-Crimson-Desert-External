#include <RenderObjects/Position.h>
#include <CrimsonDesert/Structures/CPosition.h>
#include <CrimsonDesert/Structures/UIGamePlayControlCommonCurrencyIndicator.h>
#include <Overlay/RenderObject.h>
#include <Math/Vector.h>
#include <math/Matrix.h>
#undef min;
#undef max;
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <cstdio>
#include <Overlay/View3D.h>

namespace RenderObjects {


    bool WorldToScreen(
        const Vector& worldPos,
        Vector& screenPos,
        const ViewMatrix_t& viewProj,
        int screenWidth,
        int screenHeight)
    {
        // Multiply the world position by the view-projection matrix (row-major)
        float x = worldPos.x * viewProj.arrData[0][0] + worldPos.y * viewProj.arrData[1][0] + worldPos.z * viewProj.arrData[2][0] + viewProj.arrData[3][0];
        float y = worldPos.x * viewProj.arrData[0][1] + worldPos.y * viewProj.arrData[1][1] + worldPos.z * viewProj.arrData[2][1] + viewProj.arrData[3][1];
        float z = worldPos.x * viewProj.arrData[0][2] + worldPos.y * viewProj.arrData[1][2] + worldPos.z * viewProj.arrData[2][2] + viewProj.arrData[3][2];
        float w = worldPos.x * viewProj.arrData[0][3] + worldPos.y * viewProj.arrData[1][3] + worldPos.z * viewProj.arrData[2][3] + viewProj.arrData[3][3];

        // Clip check
        if (w < 0.0001f)
            return false;

        // Normalize device coordinates
        float invW = 1.0f / w;
        x *= invW;
        y *= invW;

        // Convert to screen space
        screenPos.x = (x * 0.5f + 0.5f) * screenWidth;
        screenPos.y = (1.0f - (y * 0.5f + 0.5f)) * screenHeight; // Invert Y for top-left origin

        return true;
    }

    void Position::Render(ImVec2 displaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D) {
        char buf[128];
        char bufCoins[128];

        auto pPos = CrimsonDesert::CPosition::GetInstance();
        if (!pPos)
            return;
        Vector position = pPos->vLocalPos;
        snprintf(buf, sizeof(buf),
            "Position: %.2f %.2f %.2f",
            position.x, position.y, position.z);


        snprintf(bufCoins, sizeof(bufCoins),
            "Coins: %i",
            CrimsonDesert::UIGamePlayControlCommonCurrencyIndicator::GetInstance()->m_iCoins);
        ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(2, 20), ImVec2(250, 55), ImColor(0.0f, 0.0f, 0.0f, 0.5f),0.2f);
        ImGui::GetForegroundDrawList()->AddText(
            ImVec2(5, 25),
            ImColor(255, 0, 255, 230),
            buf
        );

        ImGui::GetForegroundDrawList()->AddText(
            ImVec2(5, 35),
            ImColor(255, 0, 255, 230),
            bufCoins
        );
        
	}

}