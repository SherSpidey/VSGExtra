//
// Created by ParadoxFang on 2025/5/22.

#ifndef IMHUD_H
#define IMHUD_H

#include <ui/ImWidget.h>


/**
 *  Main HUD used
 */
class ImHUD : public vsg::Inherit<vsg::Command, ImHUD>
{
public:
   ImHUD(const vsg::ref_ptr<vsg::Options>& options = {});
   void record(vsg::CommandBuffer& commandBuffer) const override;

private:
   vsg::ref_ptr<vsg::Options> _options;
};


#endif //IMHUD_H
