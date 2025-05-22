//
// Created by ParadoxFang on 2025/5/22.
//

#include <vsgImGui/imgui.h>

#include "ImHUD.h"

#include <iostream>

// void ImHUD::record(vsg::CommandBuffer& commandBuffer) const
// {
//     // set hud flag
//     ImGuiWindowFlags window_flags =
//         ImGuiWindowFlags_NoDecoration |
//         ImGuiWindowFlags_NoMove |
//         ImGuiWindowFlags_AlwaysAutoResize |
//         ImGuiWindowFlags_NoNav |
//         ImGuiWindowFlags_NoBringToFrontOnFocus |
//         ImGuiWindowFlags_NoFocusOnAppearing |
//         ImGuiWindowFlags_NoBackground;
//
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // remove padding
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // remove boarder
//
//     // begin daw
//     if (ImGui::Begin("HUD", nullptr, window_flags))
//     {
//         ImGui::SetWindowPos(ImVec2(10, 10));
//
//         // add open file button
//         if (ImGui::Button("Open"))
//         {
//             std::cout << "clicked!" << "\n";
//             std::cout.flush();
//         }
//
//         ImGui::End();
//     }
//
//     // resume ImGUI style
//     ImGui::PopStyleVar(2);
// }
ImHUD::ImHUD(const vsg::ref_ptr<vsg::Options>& options) : _options(options)
{
    
}

void ImHUD::record(vsg::CommandBuffer& commandBuffer) const
{
    // set hud flag
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoBackground;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // remove padding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // remove boarder
    
    // begin daw
    if (ImGui::Begin("HUD", nullptr, window_flags))
    {
        ImGui::SetWindowPos(ImVec2(10, 10));
    
        // add open file button
        if (ImGui::Button("Open"))
        {
            std::cout << "clicked!" << "\n";
            std::cout.flush();
        }
    
        ImGui::End();
    }
    
    // resume ImGUI style
    ImGui::PopStyleVar(2);
}
