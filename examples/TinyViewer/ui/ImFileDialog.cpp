//
// Created by Administrator on 2025/5/21.
//

#include "ImFileDialog.h"

ImFileDialog::ImFileDialog()
{
    static std::string current_path = std::filesystem::current_path().string();

    ImGui::Begin("File Dialog", &params->showFileDialog);

    ImGui::Text("Current Path: %s", current_path.c_str());

    if (ImGui::Button("..")) {
        std::filesystem::path path(current_path);
        if (path.has_parent_path()) {
            current_path = path.parent_path().string();
        }
    }

    for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
        const auto& path = entry.path();
        std::string filename = path.filename().string();
        bool is_directory = std::filesystem::is_directory(entry);

        std::string label = is_directory ? "[Dir] " + filename : filename;

        if (ImGui::Selectable(label.c_str())) {
            if (is_directory) {
                current_path = path.string();
            } else {
                params->selected_path = path.string();
                params->showFileDialog = false;
            }
        }
    }

    ImGui::End();
}
