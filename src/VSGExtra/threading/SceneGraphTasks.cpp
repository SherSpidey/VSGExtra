//
// Created by ParadoxFang on 2025/6/18.
//

#include <iostream>
#include <vsg/app/Viewer.h>

#include <VSGExtra/threading/SceneGraphTasks.h>

using namespace vsg;
using namespace VSGExtra;

DeferredDelete::DeferredDelete(
    const ref_ptr<Node>& node,
    const ref_ptr<Viewer>& viewer,
    int frame):
    _node(node),
    _viewer(observer_ptr<Viewer>(viewer)),
    _frame(frame)
{
}

void DeferredDelete::run()
{
    if (auto viewer = _viewer.ref_ptr())
    {
        if (_frame == 0)
        {
            // ref count check
            auto ref_cout = _node->referenceCount();
            if (ref_cout > 1)
            {
                std::cerr << "Try to delete a node with more than one ref-count: " << ref_cout << std::endl;
            }

            // remove from viewer's operation to perform delete
            auto ref_this = ref_ptr(this);
            viewer->updateOperations->remove(ref_this);
        }

        _frame--;
    }
}

void VSGExtra::AttachNodeTo(
    const ref_ptr<Group>& parent,
    const ref_ptr<Node>& node,
    const ref_ptr<Viewer>& viewer)
{
    parent->addChild(node);
    auto result = viewer->compileManager->compile(node);
    updateViewer(*viewer, result);
}

void VSGExtra::DetachNodeFrom(
    const ref_ptr<Group>& parent,
    const ref_ptr<Node>& node,
    const ref_ptr<Viewer>& viewer,
    int frames)
{
    auto& children = parent->children;
    auto it = std::find(children.begin(), children.end(), node);
    if (it != children.end())
    {
        // detach node first
        children.erase(it);

        // add delay cleanup operation
        auto op_delete = DeferredDelete::create(node, viewer, frames);
        viewer->updateOperations->add(op_delete, UpdateOperations::ALL_FRAMES);
    }
}
