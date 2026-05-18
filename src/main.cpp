#include <iostream>
#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <config.hpp>
#include <vector>
#include <ping.hpp>

int main() {
    std::string token{};

    try {
        std::ifstream ifs{ "config.json" };
        nlohmann::json json_config(nlohmann::json::parse(ifs));

        juno::config config{ json_config.get<juno::config>() };

        token = config.token();

    } catch (const std::exception& e) {
        std::cerr << "Retrieval of config failed: " << e.what() << '\n';

        return 0;
    }

    dpp::cluster cluster{ token };

    // The modules that will be utilized for the bot
    std::vector<juno::module*> modules{
        &juno::ping::instance
    };

    cluster.on_slashcommand([&](const dpp::slashcommand_t& event) {
        for (const auto& module : modules) {
            if (event.command.get_command_name() == module->name()) {
                module->on_command_execution(event);
            }
        }
    });

    cluster.on_ready([&](const dpp::ready_t& ready) {
        if (dpp::run_once<class register_bot_cmds>()) {
            for (const auto& module : modules) {
                module->register_command(cluster);
            }
        }
    });

    cluster.start(dpp::st_wait);

    return 0;
}