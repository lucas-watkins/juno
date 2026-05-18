#include <ping.hpp>

dpp::slashcommand juno::ping::make_command(dpp::cluster& cluster) const {
        return dpp::slashcommand{ name(), "Replies with \"Pong!\"", cluster.me.id };
}

void juno::ping::on_command_execution(const dpp::slashcommand_t& event) const {
    event.reply("Pong!");
}
