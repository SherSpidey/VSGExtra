//
// Created by ParadoxFang on 2025/5/20.
//

#include <iostream>
#include <vsg/all.h>
#include <vsgImGui/RenderImGui.h>
#include <vsgImGui/SendEventsToImGui.h>

#ifdef vsgXchange_FOUND
#    include <vsgXchange/all.h>
#endif

#include <core/Application.h>

#include <ui/ImFileDialog.h>
#include <ui/ImInitializer.h>

#include "ui/ImHUD.h"

#ifdef WIN32
#   include <windows.h>
#   include <resources/resources.h>
#endif

using namespace vsg;


class Controller : public Inherit<Visitor, Controller>
{
public:
    explicit Controller(AppPimpl* pimpl): _pimpl(pimpl)
    {
    }

    void apply(KeyPressEvent&) override;

private:
    AppPimpl* _pimpl;
};

class AppPimpl
{
public:
    AppPimpl();

    ref_ptr<Options> options;
    ref_ptr<Window> window;
    ref_ptr<Viewer> viewer;
    ref_ptr<Camera> camera;

    ref_ptr<ImParams> params;
    
    ref_ptr<Group> scene_root;
    ref_ptr<RenderGraph> render_graph;
    ref_ptr<CommandGraph> command_graph;
    
    ref_ptr<Trackball> trackball;
};

void Controller::apply(KeyPressEvent& event)
{
    if (event.keyBase == KEY_Equals)
    {
        Path filename{"data/models/teapot.vsgt"};
        if (auto node = vsg::read_cast<Node>(filename, _pimpl->options))
        {
            ComputeBounds computeBounds;
            node->accept(computeBounds);
            dvec3 center = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
            double radius = length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;

            auto look_at = LookAt::create(center + dvec3(0.0, -radius * 3.5, 0.0),
                                            center,
                                            dvec3(0.0, 0.0, 1.0));
            *_pimpl->camera->viewMatrix.cast<LookAt>() = *look_at;
            _pimpl->trackball->addKeyViewpoint(KEY_Space, look_at, 1.0);
            
            _pimpl->scene_root->addChild(node);
            
            auto result = _pimpl->viewer->compileManager->compile(node);
            // updateViewer(*(_pimpl->viewer), result);
        }
    }

    if (event.keyBase == KEY_Minus)
    {
        auto& children = _pimpl->scene_root->children;
        if (!children.empty())
        {
            _pimpl->viewer->deviceWaitIdle();
            children.erase(children.begin());

            // auto result = _pimpl->viewer->compileManager->compile(_pimpl->scene_root);
            // updateViewer(*(_pimpl->viewer), result);
        }
    }

    if (event.keyBase == KEY_F1)
    {
        _pimpl->params->Show();
    }
}

AppPimpl::AppPimpl()
{
    // init options
    options = Options::create();
    options->paths = getEnvPaths("MyPath");

#ifdef vsgXchange_all
    // add vsgXchange's support for reading and writing 3rd party file formats
    options->add(vsgXchange::all::create());
#endif

    // init window traits
    auto window_traits = WindowTraits::create();
    window_traits->windowTitle = "TinyViewer";
    window_traits->debugLayer = true;

    window = Window::create(window_traits);
    if (!window)
    {
        throw Exception{"Window creation failed", 1};
    }
    
    // set window icon
#ifdef WIN32
    // check win32 native window
    if (auto hwnd = std::any_cast<HWND>(window_traits->nativeWindow))
    {
        // get instance then icon
        auto hInstance = GetModuleHandle(nullptr);
        auto hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAIN_ICON));
        // set icon
        SendMessage(hwnd, WM_SETICON, ICON_BIG,
                    reinterpret_cast<LPARAM>(hIcon));
        SendMessage(hwnd, WM_SETICON, ICON_SMALL,
                    reinterpret_cast<LPARAM>(hIcon));
    }
#endif

    // init camera
    auto [width, height] = window->extent2D();
    auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    // perspective matrix
    // using unity default camera setting
    auto perspective = Perspective::create(60, aspect_ratio, 0.3f, 1000.0f);
    // look-at matrix
    dvec3 center{};
    double focal_distance = 10.;
    auto look_at = LookAt::create(center + dvec3(0, -1, 0) * focal_distance,
                                  center, dvec3(0, 0, 1));
    camera = Camera::create(perspective, look_at, ViewportState::create(window->extent2D()));

    // create scene root
    scene_root = Group::create();

    // create command graph
    command_graph = CommandGraph::create(window);
    render_graph = RenderGraph::create(window);
    command_graph->addChild(render_graph);

    // create a view with scene_root
    auto view = View::create(camera);
    // add a headlight and the scene_root
    view->addChild(createHeadlight());
    view->addChild(scene_root);

    // gui init
    auto gui_initializer = ImInitializer::create(options);
    gui_initializer->AddChineseSupport();

    // add gui
    auto hud = ImHUD::create(options);
    
    params = ImFileDialogParams::create();
    auto file_window = ImWindow::create(ImFileDialog::create(params), options);
    
    auto render_ui = vsgImGui::RenderImGui::create(window, hud);
    render_ui->add(file_window);

    // set render graph content
    render_graph->addChild(view);
    render_graph->addChild(render_ui);

    // init viewer
    viewer = Viewer::create();
    viewer->addWindow(window);
    viewer->addEventHandler(vsgImGui::SendEventsToImGui::create());
    viewer->addEventHandler(Controller::create(this));
    viewer->addEventHandler(CloseHandler::create(viewer));

    trackball = Trackball::create(camera);
    viewer->addEventHandler(trackball);

    viewer->setupThreading();

    viewer->assignRecordAndSubmitTaskAndPresentation({command_graph});
    viewer->compile();
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
        auto viewer = _m_pimpl->viewer;
        if (!viewer)
            return ERROR_VSG_FAILED;

        while (viewer->advanceToNextFrame())
        {
            viewer->handleEvents();

            viewer->update();

            viewer->recordAndSubmit();

            viewer->present();
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
