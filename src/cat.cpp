#include <cat.hpp>

juno::module::registration_result juno::cat::make_command(dpp::cluster& cluster) const {
    return dpp::slashcommand{ name(), "Cats as a Service!", cluster.me.id };
}

void juno::cat::on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const {
    cluster.request(
        "https://cataas.com/cat",
        dpp::m_get,
        [event](const dpp::http_request_completion_t& req) {
            post_image_to_channel(event, req);
        }
    );
}

void juno::cat::post_image_to_channel(const dpp::slashcommand_t& event, const dpp::http_request_completion_t& req) {
    if (req.error != dpp::h_success || req.status != 200) {
        throw std::runtime_error("Unable to query API");
    }

    const dpp::message msg = dpp::message{ event.command.channel_id, "" }
        .add_file("cat.jpeg", req.body);

    event.reply(msg);
}
