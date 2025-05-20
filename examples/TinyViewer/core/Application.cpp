//
// Created by ParadoxFang on 2025/5/20.
//


#include <iostream>

#include <vsg/all.h>

#ifdef vsgXchange_FOUND
#    include <vsgXchange/all.h>
#endif

#include "Application.h"

using namespace vsg;

class AppPimpl
{
public:
    AppPimpl();

    ref_ptr<Options> options;
    ref_ptr<Window> window;
    ref_ptr<Viewer> viewer;
};

AppPimpl::AppPimpl()
{
    // init options
    options = Options::create();

#ifdef vsgXchange_all
    // add vsgXchange's support for reading and writing 3rd party file formats
    options->add(vsgXchange::all::create());
#endif

    // init window traits
    auto window_traits = WindowTraits::create();
    window_traits->windowTitle = "TinyViewer";

    window = Window::create(window_traits);
    if (!window)
    {
        throw Exception{"Window creation failed", 1};
    }

    // init viewer
    viewer = Viewer::create();
}

Application::Application()
{
    try
    {
        _m_pimpl = new AppPimpl;
    }
    catch (const Exception& e)
    {
        _result = ERROR_VSG_FAILED;
        std::cerr << "[Exception] - " << e.message << '\n';
        std::cerr.flush();
    }
}

int Application::exec()
{
    try
    {
        if (auto viewer =  _m_pimpl->viewer)
        {
            while (viewer->advanceToNextFrame() )
            {
                viewer->handleEvents();

                viewer->update();

                viewer->recordAndSubmit();

                viewer->present();
            }
        }
    }
    catch (const Exception& e)
    {
        _result = ERROR_VSG_FAILED;
        std::cerr << "[Exception] - " << e.message << '\n';
        std::cerr.flush();
    }
    
    return _result;
}
