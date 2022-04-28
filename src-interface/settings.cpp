#include "settings.h"
#include "imgui/imgui.h"
#include <string>
#include "core/params.h"

#include "core/config.h"

#include "main_ui.h"
#include "common/opencl.h"

namespace satdump
{
    namespace settings
    {
        std::vector<std::pair<std::string, satdump::params::EditableParameter>> settings_user_interface;
        std::vector<std::pair<std::string, satdump::params::EditableParameter>> settings_general;

#ifdef USE_OPENCL
        // OpenCL Selection
        int opencl_devices_id = 0;
        std::string opencl_devices_str;
        std::vector<std::tuple<std::string, int, int>> opencl_devices_enum;
#endif

        void setup()
        {
            nlohmann::ordered_json params = satdump::config::main_cfg["user_interface"];

            for (nlohmann::detail::iteration_proxy_value<nlohmann::detail::iter_impl<nlohmann::ordered_json>> cfg : params.items())
            {
                // Check setting type, and create an EditableParameter if possible
                if (cfg.value().contains("type") && cfg.value().contains("value") && cfg.value().contains("name"))
                    settings_user_interface.push_back({cfg.key(), params::EditableParameter(nlohmann::json(cfg.value()))});
            }

            params = satdump::config::main_cfg["satdump_general"];

            for (nlohmann::detail::iteration_proxy_value<nlohmann::detail::iter_impl<nlohmann::ordered_json>> cfg : params.items())
            {
                // Check setting type, and create an EditableParameter if possible
                if (cfg.value().contains("type") && cfg.value().contains("value") && cfg.value().contains("name"))
                    settings_general.push_back({cfg.key(), params::EditableParameter(nlohmann::json(cfg.value()))});
            }

#ifdef USE_OPENCL
            opencl_devices_enum = opencl::getAllDevices();
            int p = satdump::config::main_cfg["satdump_general"]["opencl_device"]["platform"].get<int>();
            int d = satdump::config::main_cfg["satdump_general"]["opencl_device"]["device"].get<int>();
            int dev_id = 0;
            for (std::tuple<std::string, int, int> &dev : opencl_devices_enum)
            {
                opencl_devices_str += std::get<0>(dev) + '\0';
                if (std::get<1>(dev) == p && std::get<2>(dev) == d)
                    opencl_devices_id = dev_id;
                dev_id++;
            }
#endif
        }

        void render()
        {
            if (ImGui::CollapsingHeader("User Interface"))
            {
                if (ImGui::BeginTable("##satdumpuisettings", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                {
                    for (std::pair<std::string, satdump::params::EditableParameter> &p : settings_user_interface)
                        p.second.draw();
                    ImGui::EndTable();
                }
            }

            if (ImGui::CollapsingHeader("General SatDump"))
            {
                if (ImGui::BeginTable("##satdumpgeneralsettings", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                {
#ifdef USE_OPENCL
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("OpenCL Device");
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("OpenCL Device SatDump will use for accelerated computing where it can help, eg, for some image processing tasks such as projections.");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Combo("##opencldeviceselection", &opencl_devices_id, opencl_devices_str.c_str());
#endif

                    for (std::pair<std::string, satdump::params::EditableParameter> &p : settings_general)
                        p.second.draw();
                    ImGui::EndTable();
                }
            }

            if (ImGui::Button("Save"))
            {
#ifdef USE_OPENCL
                satdump::config::main_cfg["satdump_general"]["opencl_device"]["platform"] = std::get<1>(opencl_devices_enum[opencl_devices_id]);
                satdump::config::main_cfg["satdump_general"]["opencl_device"]["device"] = std::get<2>(opencl_devices_enum[opencl_devices_id]);
#endif

                for (std::pair<std::string, satdump::params::EditableParameter> &p : settings_user_interface)
                    satdump::config::main_cfg["user_interface"][p.first]["value"] = p.second.getValue();
                for (std::pair<std::string, satdump::params::EditableParameter> &p : settings_general)
                    satdump::config::main_cfg["satdump_general"][p.first]["value"] = p.second.getValue();
                config::saveUserConfig();
            }
        }
    }
}