//
// Created by ParadoxFang on 2025/5/22.
//

#include <iostream>
#include <ui/ImFileDialog.h>
#include <ui/ImButton.h>

#include "ImHUD.h"

using namespace vsg;

ImHUD::ImHUD(
    const ref_ptr<Viewer>& viewer,
    const ref_ptr<Group>& root,
    const ref_ptr<Camera>& camera,
    const ref_ptr<Trackball>& trackball,
    const ref_ptr<Options>& options
):
    _viewer(viewer),
    _root(root),
    _camera(camera),
    _trackball(trackball),
    _options(options)
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
            // import !!!
            file_dialog_params->status = ImFileDialogParams::Picking;

            // open model
            OpenModel(file_dialog_params->selected_path.c_str());
        }
    };

    auto model_button = ImButton::create(model_button_params);
    _widgets.emplace_back(model_button);
}

void ImHUD::OpenModel(const char* path) const
{
    Path filename(path);
    if (auto node = vsg::read_cast<Node>(filename, _options))
    {
        // ComputeBounds computeBounds;
        // node->accept(computeBounds);
        // dvec3 center = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
        // double radius = length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
        //
        // auto look_at = LookAt::create(center + dvec3(0.0, -radius * 3.5, 0.0),
        //                               center,
        //                               dvec3(0.0, 0.0, 1.0));
        // *_camera->viewMatrix.cast<LookAt>() = *look_at;
        // _trackball->addKeyViewpoint(KEY_Space, look_at, 1.0);

        _root->addChild(node);

        auto result = _viewer->compileManager->compile(node);
        updateViewer(*_viewer, result);
    }
}
