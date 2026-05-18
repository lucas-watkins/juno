#include <iostream>
#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <config.hpp>

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

    cluster.on_slashcommand([&](const dpp::slashcommand_t& cmd) {
        if (cmd.command.get_command_name() == "ping") {
            cmd.reply("pong!");
        }
    });

    cluster.on_ready([&](const dpp::ready_t& ready) {
        if (dpp::run_once<class register_bot_cmds>()) {
            cluster.global_command_create(dpp::slashcommand{ "ping", "Ping Pong!", cluster.me.id });
        }
    });

    cluster.start();

    return 0;
}