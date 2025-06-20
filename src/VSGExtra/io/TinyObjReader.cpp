//
// Created by ParadoxFang on 2025/6/18.
//

#include <iostream>
#include <vsg/io/read.h>
#include <vsg/nodes/StateGroup.h>
#include <vsg/state/material.h>
#include <vsg/utils/GraphicsPipelineConfigurator.h>
#include <vsg/utils/ShaderSet.h>
#include <vsg/nodes/VertexIndexDraw.h>

#include <VSGExtra/io/TinyObjReader.h>

#pragma warning(disable: 4819)
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

    auto draw_nodes = AssignToDraw(result);

    auto shader_set = CreateShaderSet(options);

    DescriptorSetLayoutBindings descriptorBindings;
    auto descriptorSetLayout = DescriptorSetLayout::create(descriptorBindings);

    // Set up push constants for transformation matrices
    PushConstantRanges pushConstantRanges{
        {VK_SHADER_STAGE_VERTEX_BIT, 0, 128} // projection and model-view matrices
    };

    VertexInputState::Bindings vertexBindingsDescriptions{
        VkVertexInputBindingDescription{0, sizeof(float) * 8, VK_VERTEX_INPUT_RATE_VERTEX}, // vertices
        VkVertexInputBindingDescription{1, sizeof(vec4), VK_VERTEX_INPUT_RATE_INSTANCE} // colors
    };

    VertexInputState::Attributes vertexAttributeDescriptions{
        VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
        VkVertexInputAttributeDescription{1, 0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3},
        VkVertexInputAttributeDescription{2, 0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 6},
        
        VkVertexInputAttributeDescription{3, 1, VK_FORMAT_R32G32B32A32_SFLOAT, 0} // colors
    };

    auto pipelineLayout = PipelineLayout::create(DescriptorSetLayouts{descriptorSetLayout},
                                                 pushConstantRanges);

    // Set up graphics pipeline states
    GraphicsPipelineStates pipelineStates{
        VertexInputState::create(vertexBindingsDescriptions, vertexAttributeDescriptions),
        InputAssemblyState::create(),
        RasterizationState::create(),
        MultisampleState::create(),
        ColorBlendState::create(),
        DepthStencilState::create()
    };

    auto graphicsPipeline = GraphicsPipeline::create(
        pipelineLayout,
        shader_set->stages,
        pipelineStates);

    auto bindGraphicsPipeline = BindGraphicsPipeline::create(graphicsPipeline);

    // Create StateGroup as the root of the scene
    auto scenegraph = StateGroup::create();
    scenegraph->add(bindGraphicsPipeline);

    for (const auto& node : draw_nodes)
        scenegraph->addChild(node);

    return scenegraph;
}

std::vector<ref_ptr<VertexIndexDraw>> TinyObjReader::AssignToDraw(const rapidobj::Result& result)
{
    std::vector<ref_ptr<VertexIndexDraw>> nodes;

    // map raw float array to vec array
    auto& pos = result.attributes.positions;
    auto position = vec3Array::create(pos.size() / 3);
    std::memcpy(position->dataPointer(), pos.data(), pos.size() * sizeof(pos[0]));

    auto& nor = result.attributes.normals;
    auto normals = vec3Array::create(nor.size() / 3);
    std::memcpy(normals->dataPointer(), nor.data(), nor.size() * sizeof(nor[0]));

    auto& tex = result.attributes.texcoords;
    auto texture_coords = vec2Array::create(tex.size() / 2);
    std::memcpy(texture_coords->dataPointer(), tex.data(), tex.size() * sizeof(tex[0]));

    std::vector<Vertex> vertices;
    std::vector<std::vector<uint16_t>> indices_list;
    std::map<std::tuple<int, int, int>, uint16_t> vertex_map;

    // map all vertices
    indices_list.reserve(result.shapes.size());
    for (const auto& shape : result.shapes)
    {
        auto& mesh = shape.mesh;
        std::vector<uint16_t> indices;

        for (const auto& index : mesh.indices)
        {
            auto [v, t, n] = index;
            auto key = std::make_tuple(v, t, n);
            if (vertex_map.count(key) == 0)
            {
                Vertex vertex;
                vertex.pos = (*position)[v];
                vertex.normal = (*normals)[n];
                vertex.texture_coord = (*texture_coords)[t];
                vertex_map[key] = static_cast<uint16_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(vertex_map[key]);
        }

        indices_list.emplace_back(indices);
    }

    // create attribute
    auto vertices_data = Array<Vertex>::create(vertices.size());
    std::copy_n(vertices.begin(), vertices.size(), vertices_data->begin());

    auto color_data = vec4Array::create({
        {1.0f, 0.0f, 0.0f, 1.f}
    });

    for (const auto& indices : indices_list)
    {
        auto indices_data = ushortArray::create(indices.size());
        std::copy_n(indices.begin(), indices.size(), indices_data->begin());

        DataList arrays{vertices_data, color_data};
        auto vid = VertexIndexDraw::create();
        vid->assignArrays(arrays);
        vid->assignIndices(indices_data);
        vid->indexCount = indices_data->width();
        vid->instanceCount = 1;

        nodes.push_back(vid);
    }

    return nodes;
}
