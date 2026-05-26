#include <iostream>
#include <ctime>
#include <cstdint>
#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <config.hpp>
#include <vector>
#include <ping.hpp>
#include <qalc.hpp>
#include <cat.hpp>
#include <util.hpp>
#include <logging.hpp>

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
        &juno::qalc::instance,
        &juno::cat::instance
    };

    cluster.on_slashcommand([&](const dpp::slashcommand_t& event) {
        // operator<< for dpp::slashcommand_t handles newlines itself
        juno::log << juno::logging::loglevel::info << ' ' << event;

        for (const auto& module : modules) {
            if (event.command.get_command_name() == module->name()) {
                try {
                    module->on_command_execution(event, cluster);
                } catch (const std::exception& ex) {
                    juno::log << juno::logging::loglevel::error << " Exception Occurred!: " << ex.what() << '\n';

                    dpp::embed embed = dpp::embed{}
                                       .set_title(">˷< Exception Occurred!")
                                       .add_field("Details", ex.what())
                                       .set_color(dpp::colors::red_blood);

                    event.reply(embed);
                }
            }
        }
    });

    cluster.on_ready([&](const dpp::ready_t& ready) {
        if (dpp::run_once<class register_bot_cmds>()) {
            // Remove all previous slash commands to avoid ghost commands
            //cluster.guild_bulk_command_delete(1504533559212249179); // Juno Testing Arena server ID

            std::vector<dpp::slashcommand> commands{};
            commands.reserve(modules.size()); // approx guess


            for (const auto& module : modules) {
                const juno::module::registration_result r{ module->make_command(cluster) };

                if (r.holds<dpp::slashcommand>()) {
                    commands.push_back(r.get<dpp::slashcommand>());
                } else {
                    commands.insert_range(commands.cend(), r.get<std::vector<dpp::slashcommand>>());
                }
            }

            cluster.global_bulk_command_create(commands);

            std::cout << commands.size() << " modules loaded (";
            for (int i{ 0 }; i < commands.size(); ++i) {
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
