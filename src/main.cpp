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
    try {
        std::ifstream ifs{ "config.json" };

        if (!std::filesystem::exists("config.json") || ifs.bad()) {
            juno::global_log << juno::logging::loglevel::error << " config.json not found! Exiting...\n";
            return 0;
        }

        nlohmann::json json_config(nlohmann::json::parse(ifs));

        juno::global_config = std::move(json_config.get<juno::config>());

    } catch (const std::exception& e) {
        juno::global_log << juno::logging::loglevel::error << " Retrieval of config failed: " << e.what() << " Exiting...\n";

        return 0;
    }

    dpp::cluster cluster{ juno::global_config.token(), dpp::i_default_intents | dpp::i_message_content };

    // The modules that will be utilized for the bot, added by cmake generated code
    std::vector<std::unique_ptr<juno::command>> commands;
    juno::add_commands(commands);

    // The filters that will be utilized for the bot, added by cmake generated code
    std::vector<std::unique_ptr<juno::filter>> filters;
    juno::add_filters(filters);

    cluster.on_slashcommand([&](const dpp::slashcommand_t& event) {
        juno::global_log << juno::logging::loglevel::info << ' ' << event << '\n';

        for (const auto& command : commands) {
            if (event.command.get_command_name() == command->name()) {
                try {
                    command->on_command_execution(event, cluster);
                } catch (const std::exception& ex) {
                    event.reply(juno::util::log_and_error_embed("Exception Occurred!", ex.what()));
                }
            }
        }
    });

    cluster.on_message_create([&](const dpp::message_create_t& event) {
        for (const auto& filter : filters) {
            if (filter->should_filter(event, cluster)) {
                try {
                    filter->on_filter_execution(event, cluster);
                } catch (const std::exception& ex) {
                    event.reply(juno::util::log_and_error_embed("Exception Occurred!", ex.what()));
                }
            }
        }
    });

    cluster.on_ready([&](const dpp::ready_t& ready) {
        if (dpp::run_once<class register_bot_cmds>()) {
            juno::global_log << juno::logging::loglevel::info << " Connected to Discord\n";

            std::vector<dpp::slashcommand> cmds_bath_for_post{};
            cmds_bath_for_post.reserve(commands.size()); // approx guess


            for (const auto& cmd : commands) {
                cmds_bath_for_post.emplace_back(cmd->make_command(cluster));
            }

            cluster.global_bulk_command_create(cmds_bath_for_post);

            juno::global_log << juno::logging::loglevel::info << ' ' << commands.size() << " commands loaded (";
            for (int i{ 0 }; i < commands.size(); ++i) {
                juno::global_log << commands[i]->name();
                if (i < commands.size() - 1) {
                    juno::global_log << ", ";
                }
            }
            juno::global_log << ")\n";

            juno::global_log << juno::logging::loglevel::info << ' ' << filters.size() << " filters loaded (";
            for (int i{ 0 }; i < filters.size(); ++i) {
                juno::global_log << filters[i]->name();
                if (i < filters.size() - 1) {
                    juno::global_log << ", ";
                }
            }
            juno::global_log << ")\n";
        }
    });

    juno::global_log << juno::logging::loglevel::info <<  " Starting...\n";

    cluster.start(dpp::st_wait);

    return 0;
}
