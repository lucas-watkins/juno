#include <nlohmann/json.hpp>
#include <commands/deadlockversion.hpp>
#include <util.hpp>

dpp::slashcommand juno::deadlockversion::make_command(dpp::cluster& cluster) const {
    return dpp::slashcommand{ name(), "Get the Current Versions of the Game Deadlock", cluster.me.id }
        .add_option(
            dpp::command_option{ dpp::co_string, "platform", "Version of what to get", true }
            .add_choice(dpp::command_option_choice{ "client", "platform_client" })
            .add_choice(dpp::command_option_choice{ "server", "platform_server" })
        );
}

void juno::deadlockversion::on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) {
    const std::string platform{ std::get<std::string>(event.get_parameter("platform")) };

    auto send_error = [](const dpp::slashcommand_t& e, const int httpstatus) {
        e.reply(
            util::log_and_error_embed(
                "HTTP Error",
                std::format("HTTP Status -> {}", httpstatus)
            )
        );
    };

    if (platform == "platform_client") {
        cluster.request(
            "https://api.steampowered.com/IGCVersion_1422450/GetClientVersion/v1/",
            dpp::m_get,
            [event, send_error](const dpp::http_request_completion_t& req) {
                if (req.error != dpp::h_success || req.status != 200) {
                    send_error(event, req.status);
                }

                try {
                    nlohmann::json j(nlohmann::json::parse(req.body));
                    const std::size_t min_allowed_version{ j["result"]["min_allowed_version"].get<std::size_t>() };
                    const std::size_t active_version{ j["result"]["active_version"].get<std::size_t>() };

                    const dpp::embed resp = dpp::embed{}
                        .add_field("Minimum Allowed Version", std::to_string(min_allowed_version))
                        .add_field("Active Version", std::to_string(active_version));

                    event.reply(resp);
                } catch (const std::exception& e) {
                    event.reply(util::log_and_error_embed("JSON Exception!", e.what()));
                }
            }
        );
    } else if (platform == "platform_server") {
        cluster.request(
            "https://api.steampowered.com/IGCVersion_1422450/GetServerVersion/v1/",
            dpp::m_get,
            [event, send_error](const dpp::http_request_completion_t& req) {
                if (req.error != dpp::h_success || req.status != 200) {
                    send_error(event, req.status);
                }

                try {
                    nlohmann::json j(nlohmann::json::parse(req.body));
                    const std::size_t engine{ j["result"]["engine"].get<std::size_t>() };
                    const std::size_t deploy_version{ j["result"]["deploy_version"].get<std::size_t>() };
                    const std::size_t active_version{ j["result"]["active_version"].get<std::size_t>() };
                    const std::size_t response_time{ j["result"]["response_time"].get<std::size_t>() };

                    const dpp::embed resp = dpp::embed{}
                        .add_field("Engine", std::to_string(engine))
                        .add_field("Deploy Version", std::to_string(deploy_version))
                        .add_field("Active Version", std::to_string(active_version))
                        .add_field("Response Time", std::to_string(response_time));

                    event.reply(resp);
                } catch (const std::exception& e) {
                    event.reply(util::log_and_error_embed("JSON Exception!", e.what()));
                }
            }
        );
    } else {
        event.reply(util::log_and_error_embed("Unknown Argument", "Argument was not valid"));
    }
}
