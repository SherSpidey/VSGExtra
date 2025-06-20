//
// Created by ParadoxFang on 2025/6/18.
//

#ifndef TINYOBJREADER_H
#define TINYOBJREADER_H

#include <vsg/io/ReaderWriter.h>

#include <VSGExtra/export.h>

namespace rapidobj
{
    struct Result;
}

namespace VSGExtra
{
    class VSGEXTRA_DECLSPEC TinyObjReader : public vsg::Inherit<vsg::ReaderWriter, TinyObjReader>
    {
    public:
        TinyObjReader() = default;

        vsg::ref_ptr<Object> read(const vsg::Path&, vsg::ref_ptr<const vsg::Options>) const override;

    protected:
        struct Vertex
        {
            vsg::vec3 pos;
            vsg::vec3 normal;
            vsg::vec2 texture_coord;
        };

        static std::vector<vsg::ref_ptr<vsg::VertexIndexDraw>> AssignToDraw(const rapidobj::Result& result);
    };
}


#endif //TINYOBJREADER_H
