//
// Created by ParadoxFang on 2025/5/26.
//

#include <vsg/all.h>

#ifdef vsgXchange_FOUND
#    include <vsgXchange/all.h>
#endif

#include <vsgImGui/RenderImGui.h>
#include <vsgImGui/SendEventsToImGui.h>

#include <ui/ImInitializer.h>
#include <ui/ImHUD.h>

#ifdef WIN32
#   include <windows.h>
#   include <resources/resources.h>
#endif


#include <filesystem>

// test
#include <VSGExtra/io/TinyObjReader.h>
#include <VSGExtra/app/ViewerPawn.h>
#include <VSGExtra/app/ViewerCamera.h>
#include <VSGExtra/threading/SceneGraphTasks.h>

#include <core/VSGApplication.h>

using namespace vsg;
using namespace VSGExtra;


class VSGApplication::VSGAppPImpl
{
public:
    VSGAppPImpl(VSGApplication* owner);

    ref_ptr<Options> options;
    ref_ptr<Window> window;
    ref_ptr<Viewer> viewer;

    ref_ptr<Group> scene_root;
    ref_ptr<RenderGraph> render_graph;
    ref_ptr<CommandGraph> command_graph;

    ref_ptr<DefaultPawn> pawn;

    ref_ptr<XCamera> camera;

private:
    VSGApplication* _owner;
};

class TestVisitor : public Inherit<Visitor, TestVisitor>
{
public:
    TestVisitor() = delete;
    
    explicit TestVisitor(VSGApplication::VSGAppPImpl* impl) : impl_(impl) {}

    void apply(Object& object) override
    {
        object.traverse(*this);
    }

    void apply(KeyPressEvent& key_press_event) override
    {
        if (key_press_event.keyBase == KEY_p)
        {
            impl_->render_graph->setClearValues({{0.8734f, 0.9393f, 1.0f, 1.0f}});
        }
        if (key_press_event.keyBase == KEY_d)
        {
            auto viewer = impl_->viewer;
            
            auto root = impl_->scene_root;
            auto& children = root->children;
            if (!children.empty())
            {
                auto node = children[0];
                DetachNodeFrom(root, node, viewer);
            }
        }
    }

private:
    VSGApplication::VSGAppPImpl* impl_;
};

VSGApplication::VSGAppPImpl::VSGAppPImpl(VSGApplication* owner) : _owner(owner)
{
    // init options
    options = Options::create();
    options->paths = getEnvPaths("DataPath");
    // options->sharedObjects = SharedObjects::create();

    // set data path to current path
    if (!options->paths.empty())
        std::filesystem::current_path(options->paths[0].c_str());

#ifdef vsgXchange_all
    options->add(TinyObjReader::create());
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
    auto perspective = Perspective::create(60, aspect_ratio, 0.03, 1000);
    // look-at matrix
    dvec3 center{};
    double focal_distance = 10.;
    auto look_at = LookAt::create(center + dvec3(0, -1, 0) * focal_distance,
                                  center, dvec3(0, 0, 1));
    camera = ViewerCamera::create(perspective, look_at, ViewportState::create(window->extent2D()));

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
    auto hud = ImHUD::create(_owner);
    auto render_ui = vsgImGui::RenderImGui::create(window, hud);

    // set render graph content
    render_graph->addChild(view);
    render_graph->addChild(render_ui);

    // init viewer
    viewer = Viewer::create();

    // create pawn
    pawn = ViewerPawn::create(camera);

    /*=========================  TEST  ==============================*/
    pawn->AddKeyViewpoint(KEY_1, LookAt::create(dvec3{0, 0, 14}, dvec3{0, 0, 0}, dvec3{0, 1, 0}), 0.5);
    /*=========================  TEST  ==============================*/

    viewer->addWindow(window);
    viewer->addEventHandler(vsgImGui::SendEventsToImGui::create());
    viewer->addEventHandler(CloseHandler::create(viewer));
    viewer->addEventHandler(pawn);
    viewer->addEventHandler(TestVisitor::create(this));

    viewer->setupThreading();

    viewer->assignRecordAndSubmitTaskAndPresentation({command_graph});
    viewer->compile();
}

VSGApplication::VSGApplication()
{
    _m_pimpl = new VSGAppPImpl(this);
}

int VSGApplication::RenderLoop() const
{
    auto viewer = _m_pimpl->viewer;
    if (!viewer)
        return 1;

    while (viewer->advanceToNextFrame())
    {
        viewer->handleEvents();

        viewer->update();

        viewer->recordAndSubmit();

        viewer->present();
    }
    return 0;
}

void VSGApplication::OpenModel(const char* path) const
{
    Path filename(path);
    if (auto node = vsg::read_cast<Node>(filename, _m_pimpl->options))
    {
        AttachNodeTo(_m_pimpl->scene_root, node, _m_pimpl->viewer);

        _m_pimpl->pawn->FitView(_m_pimpl->scene_root);
        _m_pimpl->pawn->AddKeyViewpoint(KEY_Space, 0.5);
    }
}
