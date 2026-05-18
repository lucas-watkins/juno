#include <ping.hpp>

void juno::ping::register_command(dpp::cluster& cluster) const {
    cluster.global_command_create(
        dpp::slashcommand(name(), "Replies with \"Pong!\"", cluster.me.id)
    );
}

void juno::ping::on_command_execution(const dpp::slashcommand_t& event) const {
    event.reply("Pong!");
}
