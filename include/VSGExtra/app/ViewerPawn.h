//
// Created by ParadoxFang on 2025/5/16.
//

#ifndef VIEWERPAWN_H
#define VIEWERPAWN_H

#include <VSGExtra/app/DefaultPawn.h>

namespace VSGExtra
{
    class VSGEXTRA_DECLSPEC ViewerPawn : public vsg::Inherit<DefaultPawn, ViewerPawn>
    {
    public:
        explicit ViewerPawn(const vsg::ref_ptr<vsg::Camera>& camera);


    protected:
        // get trackball coordinates
        vsg::dvec3 TBC(const vsg::PointerEvent& event) const;
    };
}

EVSG_type_name(VSGExtra::ViewerPawn)

#endif //VIEWERPAWN_H
