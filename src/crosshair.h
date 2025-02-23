#pragma once
#include <ImGui/imgui.h>

namespace Crosshair
{




    static void DrawCrosshair(float screenWidth, float screenHeight, ImVec4 color, float size)
    {
        ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);
        ImU32 crosshairColor = IM_COL32((int)(color.x * 255), (int)(color.y * 255), (int)(color.z * 255), (int)(color.w * 255));

        ImGui::GetForegroundDrawList()->AddLine(
            ImVec2(center.x - size, center.y),
            ImVec2(center.x + size, center.y),
            crosshairColor, 2.0f);

        ImGui::GetForegroundDrawList()->AddLine(
            ImVec2(center.x, center.y - size),
            ImVec2(center.x, center.y + size),
            crosshairColor, 2.0f);
    }

    static void DrawCircle(float screenWidth, float screenHeight, ImVec4 color, float radius)
    {
        ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);
        ImU32 circleColor = IM_COL32((int)(color.x * 255), (int)(color.y * 255), (int)(color.z * 255), (int)(color.w * 255));

        ImGui::GetForegroundDrawList()->AddCircle(center, radius, circleColor, 100, 2.0f);
    }


}
