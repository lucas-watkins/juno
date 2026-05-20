#include <iostream>
#include <ctime>
#include <cstdint>
#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <config.hpp>
#include <vector>
#include <ping.hpp>
#include <qalc.hpp>
#include <util.hpp>

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
        &juno::ping::instance,
        &juno::qalc::instance
    };

    cluster.on_slashcommand([&](const dpp::slashcommand_t& event) {
        // TODO: Replace with real logging later
        const dpp::interaction& cmd{ event.command };
        const std::time_t now{ std::time(nullptr) };

        // buffer
        char formatted[100];
        std::strftime(formatted, std::size(formatted), "%c", std::localtime(&now));

        std::cout << '[' << formatted << " INFO] ";
        std::cout << cmd.get_issuing_user().format_username() << " used " << cmd.get_command_name();

        const auto& opts{ cmd.get_command_interaction().options };

        if (!opts.empty()) {
            std::cout << " with options: \n";
            for (const auto& option : opts) {
                std::cout << std::setw(std::strlen(formatted) + 16 + option.name.size()) << option.name << " -> ";

                if (const std::optional v{ juno::util::variant_to_string(option.value) }) {
                    std::cout << *v << '\n';
                } else {
                    std::cout << "[null value]\n";
                }
            }
        } else {
            std::cout << '\n';
        }

        for (const auto& module : modules) {
            if (event.command.get_command_name() == module->name()) {
                module->on_command_execution(event);
            }
        }
    });

    cluster.on_ready([&](const dpp::ready_t& ready) {
        if (dpp::run_once<class register_bot_cmds>()) {

            // Remove all previous slash commands to avoid ghost commands
            //cluster.guild_bulk_command_delete(1504533559212249179); // LF-2 server ID

            std::vector<dpp::slashcommand> commands{};
            commands.reserve(modules.size());


            for (const auto& module : modules) {
                commands.emplace_back(module->make_command(cluster));
            }

            cluster.global_bulk_command_create(commands);

            std::cout << commands.size() << " modules loaded (";
            for (int i {0}; i < commands.size(); ++i) {
                std::cout << modules[i]->name();
                if (i < commands.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << ")\n";
        }
    });

    std::cout << "Starting...\n";

    cluster.start(dpp::st_wait);

    return 0;
}