//
// Created by ParadoxFang on 2025/5/22.
//

#ifndef IMWIDGET_H
#define IMWIDGET_H

#include <vsg/core/Inherit.h>
#include <vsg/commands/Command.h>
#include <vsg/io/Options.h>

#define IM_WIDGET_DECLARATION(ClassName) \
    explicit ClassName(const vsg::ref_ptr<ImParams>& in_params);\
    void RecordDraw() override;

#define IM_WIDGET_DEFINITION(ClassName) \
ClassName::ClassName(const vsg::ref_ptr<ImParams>& in_params): Inherit(in_params){}\
void ClassName::RecordDraw() { \
    ClassName::Draw(params); \
}

class ImParams : public vsg::Inherit<vsg::Object, ImParams>
{
public:
    void Show();
    void Hide();

    bool show{false};
};

class ImWidget : public vsg::Inherit<vsg::Object, ImWidget>
{
public:
    ImWidget(const vsg::ref_ptr<ImParams>& in_params);

    virtual void RecordDraw() = 0;

    vsg::ref_ptr<ImParams> params;
};


class ImWindow : public vsg::Inherit<vsg::Command, ImWindow>
{
public:
    ImWindow(const vsg::ref_ptr<ImWidget>& in_widget, const vsg::ref_ptr<vsg::Options>& options = {});

    void record(vsg::CommandBuffer& commandBuffer) const override;

protected:
    vsg::ref_ptr<ImWidget> widget;
};


#endif //IMWIDGET_H
