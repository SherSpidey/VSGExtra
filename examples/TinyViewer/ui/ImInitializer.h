//
// Created by ParadoxFang on 2025/5/22.
//

#ifndef IMINITIALIZER_H
#define IMINITIALIZER_H

#include <vsg/core/Inherit.h>
#include <vsg/io/Options.h>

class ImInitializer : public vsg::Inherit<vsg::Object, ImInitializer>
{
public:
    ImInitializer(const vsg::ref_ptr<vsg::Options>& options);

    void AddChineseSupport(const std::string& ttf_font = {}) const;

protected:
    vsg::ref_ptr<vsg::Options> _options;
};

#endif //IMINITIALIZER_H
