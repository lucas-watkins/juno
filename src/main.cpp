#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <config.hpp>
#include <util.hpp>
#include <logging.hpp>
#include <generated.hpp>


int main() {
    std::string token{};

    try {
        std::ifstream ifs{ "config.json" };

        if (!std::filesystem::exists("config.json") || ifs.bad()) {
            juno::log << juno::logging::loglevel::error << " config.json not found! Exiting...\n";
            return 0;
        }

        nlohmann::json json_config(nlohmann::json::parse(ifs));

        juno::config config{ json_config.get<juno::config>() };

        token = config.token();
    } catch (const std::exception& e) {
        juno::log << juno::logging::loglevel::error << " Retrieval of config failed: " << e.what() << " Exiting...\n";

        return 0;
    }

    dpp::cluster cluster{ token };

    // The modules that will be utilized for the bot, added by cmake generated code
    std::vector<std::unique_ptr<juno::command>> commands;
    juno::add_commands(commands);

    cluster.on_slashcommand([&](const dpp::slashcommand_t& event) {
        juno::log << juno::logging::loglevel::info << ' ' << event << '\n';

        for (const auto& cmd : commands) {
            if (event.command.get_command_name() == cmd->name()) {
                try {
                    cmd->on_command_execution(event, cluster);
                } catch (const std::exception& ex) {
                    event.reply(juno::util::log_and_error_embed("Exception Occurred!", ex.what()));
                }
            }
        }
    });

    cluster.on_ready([&](const dpp::ready_t& ready) {
        if (dpp::run_once<class register_bot_cmds>()) {
            // Remove all previous slash commands to avoid ghost commands
            //cluster.guild_bulk_command_delete(1504533559212249179); // Juno Testing Arena server ID

            juno::log << juno::logging::loglevel::info << " Connected to Discord\n";

            std::vector<dpp::slashcommand> cmds_bath_for_post{};
            cmds_bath_for_post.reserve(commands.size()); // approx guess


            for (const auto& cmd : commands) {
                cmds_bath_for_post.emplace_back(cmd->make_command(cluster));
            }

            cluster.global_bulk_command_create(cmds_bath_for_post);
            //cluster.guild_bulk_command_create(commands, 1504533559212249179);

            juno::log << juno::logging::loglevel::info << ' ' << commands.size() << " commands loaded (";
            for (int i{ 0 }; i < commands.size(); ++i) {
                juno::log << commands[i]->name();
                if (i < commands.size() - 1) {
                    juno::log << ", ";
                }
            }
            juno::log << ")\n";
        }
    });

    juno::log << juno::logging::loglevel::info <<  " Starting...\n";

    cluster.start(dpp::st_wait);

    return 0;
}
