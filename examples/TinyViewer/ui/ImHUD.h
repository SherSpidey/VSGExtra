//
// Created by ParadoxFang on 2025/5/22.

#ifndef IMHUD_H
#define IMHUD_H

#include <ui/ImWidget.h>

#include <vsg/all.h>


/**
 *  Main HUD used
 */
class ImHUD : public vsg::Inherit<vsg::Command, ImHUD>
{
public:
   ImHUD(
      const vsg::ref_ptr<vsg::Viewer>& viewer,
      const vsg::ref_ptr<vsg::Group>& root,
      const vsg::ref_ptr<vsg::Camera>& camera,
      const vsg::ref_ptr<vsg::Trackball>& trackball,
      const vsg::ref_ptr<vsg::Options>& options = {});
   void record(vsg::CommandBuffer& commandBuffer) const override;

protected:
   void Init();

   void OpenModel(const char*) const;

private:
   vsg::ref_ptr<vsg::Viewer> _viewer;
   vsg::ref_ptr<vsg::Group> _root;
   vsg::ref_ptr<vsg::Camera> _camera;
   vsg::ref_ptr<vsg::Trackball> _trackball;
   vsg::ref_ptr<vsg::Options> _options;
   std::vector<vsg::ref_ptr<ImWidget>> _widgets;
};


#endif //IMHUD_H
