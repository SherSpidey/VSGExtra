//
// Created by ParadoxFang on 2025/6/18.
//

#include <iostream>
#include <vsg/io/read.h>
#include <vsg/nodes/StateGroup.h>
#include <vsg/state/material.h>
#include <vsg/utils/GraphicsPipelineConfigurator.h>
#include <vsg/utils/ShaderSet.h>

#include <VSGExtra/io/TinyObjReader.h>

#include <VSGExtra/io/rapidobj.hpp>

using namespace vsg;
using namespace VSGExtra;

// temp code here
ref_ptr<ShaderSet> CreateShaderSet(const ref_ptr<const Options>& options)
{
    auto vertex_shader = read_cast<ShaderStage>("shaders/simple.vert", options);
    auto fragment_shader = read_cast<ShaderStage>("shaders/simple.frag", options);

    if (!vertex_shader || !fragment_shader)
        return {};

    auto shader_set = ShaderSet::create(ShaderStages{vertex_shader, fragment_shader});

    // add attribute info
    shader_set->addAttributeBinding("in_vertex", "", 0,
                                    VK_FORMAT_R32G32B32_SFLOAT, vec3Array::create(1));
    shader_set->addAttributeBinding("in_normal", "", 1,
                                    VK_FORMAT_R32G32B32_SFLOAT, vec3Array::create(1));
    shader_set->addAttributeBinding("in_UV", "", 2,
                                    VK_FORMAT_R32G32_SFLOAT, vec2Array::create(1));
    shader_set->addAttributeBinding("in_color", "", 3,
                                    VK_FORMAT_R32G32B32A32_SFLOAT, vec4Array::create(1), CoordinateSpace::LINEAR);
    // add descriptor info
    shader_set->addDescriptorBinding("lightData", "", 0, 0,
                                     VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
                                     VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, vec4Array::create(64));

    shader_set->addDescriptorBinding("diffuse_sampler", "", 1, 0,
                                     VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
                                     VK_SHADER_STAGE_FRAGMENT_BIT,
                                     ubvec4Array2D::create(1, 1, Data::Properties{VK_FORMAT_R8G8B8A8_UNORM}));
    shader_set->addDescriptorBinding("normal_sampler", "", 1, 1,
                                     VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
                                     VK_SHADER_STAGE_FRAGMENT_BIT,
                                     vec3Array2D::create(1, 1, Data::Properties{VK_FORMAT_R32G32B32_SFLOAT}),
                                     CoordinateSpace::LINEAR);

    shader_set->addDescriptorBinding("material", "", 1, 2,
                                     VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
                                     VK_SHADER_STAGE_FRAGMENT_BIT, PhongMaterialValue::create(),
                                     CoordinateSpace::LINEAR);

    shader_set->addPushConstantRange("pc", "", VK_SHADER_STAGE_ALL, 0, 128);

    return shader_set;
}

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
    bool success = rapidobj::Triangulate(result);
    if (!success)
        return {};

    auto shader_set = CreateShaderSet(options);
    auto pipeline_config = GraphicsPipelineConfigurator::create(shader_set);

    auto node = StateGroup::create();

    auto& vertices_ori = result.attributes.positions;
    auto vertices = vec3Array::create(vertices_ori.size() / 3);
    std::memcpy(vertices->dataPointer(), vertices_ori.data(), vertices_ori.size() * sizeof(vertices_ori[0]));

    std::cout << (*vertices)[vertices->size() -1] << std::endl;
    std::cout << vertices_ori[vertices_ori.size() - 3] << " " << vertices_ori[vertices_ori.size() - 2] <<  " " << vertices_ori[vertices_ori.size() - 1] <<std::endl;

    

    return {};
}
