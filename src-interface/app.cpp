#include "app.h"
#include "imgui/imgui.h"
#include "viewer/viewer.h"
#include "core/plugin.h"

namespace satdump
{
    Application::Application(std::string id)
        : app_id(id)
    {
    }

    Application::~Application()
    {
    }

    void Application::drawWindow()
    {
        ImGui::Begin(app_id.c_str());
        drawUI();
        ImGui::End();
    }

    void Application::draw()
    {
        ImGui::BeginChild(app_id.c_str(), ImGui::GetContentRegionAvail(), false);
        drawUI();
        ImGui::EndChild();
    }

    void Application::drawUI()
    {
        ImGui::Text("Nothing implemented there yet!");
    }

    SATDUMP_DLL std::map<std::string, std::function<std::shared_ptr<Application>()>> application_registry;

    void registerApplications()
    {
        // Register local apps
        application_registry.emplace(ViewerApplication::getID(), ViewerApplication::getInstance);

        // Plugin applicatins
        satdump::eventBus->fire_event<RegisterApplicationsEvent>({application_registry});
    }
};