//
// Created by ParadoxFang on 2025/5/22.
//

#include "ImWidget.h"

void ImParams::Show()
{
    show = true;
}

void ImParams::Hide()
{
    show = false;
}

ImWidget::ImWidget(const vsg::ref_ptr<ImParams>& in_params) : params(in_params)
{
}

ImWindow::ImWindow(const vsg::ref_ptr<ImWidget>& in_widget, const vsg::ref_ptr<vsg::Options>& options):
    widget(in_widget)
{
}

void ImWindow::record(vsg::CommandBuffer& commandBuffer) const
{
    if (widget)
    {
        widget->RecordDraw();
    }
}
