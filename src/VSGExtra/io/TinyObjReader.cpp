//
// Created by ParadoxFang on 2025/6/18.
//

#include <VSGExtra/io/TinyObjReader.h>

#include <VSGExtra/io/rapidobj.hpp>

using namespace vsg;
using namespace VSGExtra;

ref_ptr<Object> TinyObjReader::read(const Path& path, ref_ptr<const Options> options) const
{
    if (!fileExists(path))
        return {};

    auto ext = lowerCaseFileExtension(path);
    if (ext != ".obj")
        return {};

    auto result = rapidobj::ParseFile(path.c_str());
    if (result.error)
        return {};

    return {};
}
