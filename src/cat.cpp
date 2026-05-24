#include <cat.hpp>

dpp::slashcommand juno::cat::make_command(dpp::cluster& cluster) const {
    return dpp::slashcommand{ name(), "Cats as a Service!", cluster.me.id };
}

void juno::cat::on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const {
    cluster.request(
        "https://cataas.com/cat",
        dpp::m_get,
        [event](const dpp::http_request_completion_t& completion) {
            if (completion.error != dpp::h_success) {
                throw std::runtime_error("Unable to make HTTPS request");
            }

            const dpp::message msg = dpp::message{event.command.channel_id, ""}
                                       .add_file("cat.jpeg", completion.body);

            event.reply(msg);
        }
    );
}
