//
// Created by ParadoxFang on 2025/6/18.
//

#ifndef SCENEGRAPHTASKS_H
#define SCENEGRAPHTASKS_H

#include <vsg/core/observer_ptr.h>
#include <vsg/threading/OperationQueue.h>

#include <VSGExtra/export.h>

namespace VSGExtra
{

    /**
     * Manages a node referenced exclusively by this DeferredDelete instance.
     * The node will be released automatically when this operation is destroyed.
     */
    class VSGEXTRA_DECLSPEC DeferredDelete : public vsg::Inherit<vsg::Operation, DeferredDelete>
    {
    public:
        DeferredDelete() = delete;

        explicit DeferredDelete(
            const vsg::ref_ptr<vsg::Node>& node,
            const vsg::ref_ptr<vsg::Viewer>& viewer,
            int frame = 3);

        void run() override;

    private:
        vsg::ref_ptr<vsg::Node> _node;
        vsg::observer_ptr<vsg::Viewer> _viewer;
        int _frame;
    };
    
    /**
     * Add a node instantly
     */
    void VSGEXTRA_DECLSPEC AttachNodeTo(
        const vsg::ref_ptr<vsg::Group>& parent,
        const vsg::ref_ptr<vsg::Node>& node,
        const vsg::ref_ptr<vsg::Viewer>& viewer);

    /**
     * Remove a node with delayed cleanup
     */
    void VSGEXTRA_DECLSPEC DetachNodeFrom(
        const vsg::ref_ptr<vsg::Group>& parent,
        const vsg::ref_ptr<vsg::Node>& node,
        const vsg::ref_ptr<vsg::Viewer>& viewer,
        int frames = 3);
}


#endif //SCENEGRAPHTASKS_H
