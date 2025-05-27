//
// Created by ParadoxFang on 2025/5/22.

#ifndef IMHUD_H
#define IMHUD_H

#include <vsg/core/ref_ptr.h>
#include <vsg/commands/Command.h>

class VSGApplication;

/**
 *  Main HUD used
 */
class ImHUD : public vsg::Inherit<vsg::Command, ImHUD>
{
public:
    explicit ImHUD(VSGApplication* app);
    void record(vsg::CommandBuffer& commandBuffer) const override;

protected:
    void Init();

private:
    VSGApplication* _app;
    std::vector<vsg::ref_ptr<class ImWidget>> _widgets;
};


#endif //IMHUD_H
