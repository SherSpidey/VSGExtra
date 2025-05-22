//
// Created by ParadoxFang on 2025/5/22.
//

#include <iostream>

#include <vsgImGui/imgui.h>

#include "ImInitializer.h"

ImInitializer::ImInitializer(const vsg::ref_ptr<vsg::Options>& options): _options(options)
{
    ImGui::CreateContext();
}

void ImInitializer::AddChineseSupport(const std::string& ttf_font) const
{
    auto font = ttf_font;
    if (font.empty())
    {
        font = R"(C:\Windows\Fonts\simsun.ttc)";
    }

    if (const auto font_file = vsg::findFile(font, _options))
    {
        font = font_file.string();

        auto& io = ImGui::GetIO();
        io.Fonts->AddFontDefault();

        ImFontConfig config;
        config.MergeMode = true;
        config.GlyphRanges = io.Fonts->GetGlyphRangesChineseFull();

        auto im_font = io.Fonts->AddFontFromFileTTF(font.c_str(), 13, &config);

        if (!im_font)
        {
            std::cout << "Failed to load font: " << font << '\n';
            std::cout.flush();
        }
    }
}
