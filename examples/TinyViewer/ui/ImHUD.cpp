//
// Created by ParadoxFang on 2025/5/22.
//

#include <ui/ImFileDialog.h>
#include <ui/ImButton.h>

#include <core/VSGApplication.h>

#include "ImHUD.h"

using namespace vsg;

ImHUD::ImHUD(VSGApplication* app) : _app(app)
{
    Init();
}

void ImHUD::record(CommandBuffer& commandBuffer) const
{
    for (const auto& widget : _widgets)
    {
        widget->RecordDraw();
    }
}

void ImHUD::Init()
{
    auto model_button_params = ImButtonParams::create();
    model_button_params->Show();
    model_button_params->position = {10, 10};
    model_button_params->text = "Open";

    auto file_dialog_params = ImFileDialogParams::create();
    model_button_params->active = [file_dialog_params]
    {
        file_dialog_params->Show();
    };
    model_button_params->execute = [this, file_dialog_params]
    {
        ImFileDialog::Draw(file_dialog_params);
        if (file_dialog_params->status == ImFileDialogParams::Picked)
        {
            // important !!!
            file_dialog_params->status = ImFileDialogParams::Picking;

            // open model
            _app->OpenModel(file_dialog_params->selected_path.c_str());
        }
    };

    auto model_button = ImButton::create(model_button_params);
    _widgets.emplace_back(model_button);
}
