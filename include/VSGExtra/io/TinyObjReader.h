//
// Created by ParadoxFang on 2025/6/18.
//

#ifndef TINYOBJREADER_H
#define TINYOBJREADER_H

#include <vsg/io/ReaderWriter.h>

#include <VSGExtra/export.h>

namespace VSGExtra
{
    class VSGEXTRA_DECLSPEC TinyObjReader : public vsg::Inherit<vsg::ReaderWriter, TinyObjReader>
    {
    public:
        TinyObjReader() = default;

        vsg::ref_ptr<Object> read(const vsg::Path&, vsg::ref_ptr<const vsg::Options>) const override;
    };
}


#endif //TINYOBJREADER_H
