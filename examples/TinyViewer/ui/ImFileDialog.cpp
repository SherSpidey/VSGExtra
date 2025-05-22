//
// Created by ParadoxFang on 2025/5/21.
//

#include <filesystem>
#include <vsgImGui/imgui.h>

#include "ImFileDialog.h"

WIDGET_DEFINITION(ImFileDialog)

void ImFileDialog::Draw(const vsg::ref_ptr<ImParams>& in_params)
{
    if (in_params->show)
    {
        auto _params = in_params.cast<ImFileDialogParams>();
        if (!_params)
            return;

        static std::string current_path = std::filesystem::current_path().u8string();

        ImGui::Begin("File Dialog", &_params->show);

        ImGui::Text("Current Path: %s", current_path.c_str());

        if (ImGui::Button(".."))
        {
            std::filesystem::path path(current_path);
            if (path.has_parent_path())
            {
                current_path = path.parent_path().string();
            }
        }

        // sort entry list
        std::map<std::string, std::string> dir_set, file_set;
        for (const auto& entry : std::filesystem::directory_iterator(current_path))
        {
            // get entry path
            const auto& path = entry.path();
            std::string filename = path.filename().u8string();

            bool is_directory = std::filesystem::is_directory(entry);
            std::string label = is_directory ? "[Dir] " + filename : filename;
            is_directory ? dir_set[label] = path.string() : file_set[label] = path.string();
        }

        // show dir list
        for (const auto& entry : dir_set)
        {
            if (ImGui::Selectable(entry.first.c_str()))
            {
                current_path = entry.second;
            }
        }

        for (const auto& entry : file_set)
        {
            if (ImGui::Selectable(entry.first.c_str()))
            {
                _params->selected_path = entry.second;
                _params->show = false;
            }
        }

        ImGui::End();
    }
}
