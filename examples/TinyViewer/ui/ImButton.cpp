//
// Created by ParadoxFang on 2025/5/23.
//

#include <vsgImGui/imgui.h>

#include "ImButton.h"

ImButtonParams::ImButtonParams():
    position({0, 0}),
    text("Button")
{
}

IM_WIDGET_DEFINITION(ImButton)

void ImButton::Draw(const vsg::ref_ptr<ImParams>& in_params)
{
    if (in_params->show)
    {
        auto _params = in_params.cast<ImButtonParams>();
        if (!_params)
            return;

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
        if (ImGui::Begin("Button", &in_params->show, window_flags))
        {
            ImGui::SetWindowPos(ImVec2(_params->position.x, _params->position.y));

            // add open file button
            if (ImGui::Button(_params->text.c_str()))
            {
               if (_params->active)
               {
                   _params->active();
               }
            }

            // perform execution
            _params->execute();

            ImGui::End();
        }

        // resume ImGUI style
        ImGui::PopStyleVar(2);
    }
}
