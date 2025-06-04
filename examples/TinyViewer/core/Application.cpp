//
// Created by ParadoxFang on 2025/5/20.
//

#include <iostream>
#include <vsg/all.h>

#include <core/VSGApplication.h>

#include "Application.h"

using namespace vsg;

Application::Application()
{
    try
    {
        _vsg_app = new VSGApplication;
    }
    catch (const Exception& e)
    {
        _result = ERROR_VSG_FAILED;
        std::cerr << "[Exception] - " << e.message << '\n';
    }
}

int Application::exec()
{
    try
    {
        _result = static_cast<ExitCode>(_vsg_app->RenderLoop());
    }
    catch (const Exception& e)
    {
        _result = ERROR_VSG_FAILED;
        std::cerr << "[Exception] - " << e.message << '\n';
    }

    return _result;
}
