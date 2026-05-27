#include <cat.hpp>
#include <util.hpp>

dpp::slashcommand juno::cat::make_command(dpp::cluster& cluster) const {
    return dpp::slashcommand{ name(), "Cats as a Service!", cluster.me.id }
        .add_option(
            dpp::command_option(dpp::co_string, "says", "Add text to image")
        );
}

void juno::cat::on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const {
    const auto cb = [event](const dpp::http_request_completion_t& r) { post_image_to_channel(event, r); };

    const dpp::command_value says_param{ event.get_parameter("says") };

    if (!std::holds_alternative<std::monostate>(says_param)) {
        cluster.request(
            std::format(
                "https://cataas.com/cat/says/{}",
                dpp::utility::url_encode(std::get<std::string>(says_param))
            ),
            dpp::m_get,
            cb
        );
    } else {
        cluster.request("https://cataas.com/cat", dpp::m_get, cb);
    }
}

void juno::cat::post_image_to_channel(const dpp::slashcommand_t& event, const dpp::http_request_completion_t& req) {
    if (req.error != dpp::h_success || req.status != 200) {
        event.reply(util::error_embed("HTTP Error", std::format("HTTP Status -> {}", req.status)));
        return;
    }

    const dpp::message msg = dpp::message{ event.command.channel_id, "" }
        .add_file("cat.jpeg", req.body);

    event.reply(msg);
}
