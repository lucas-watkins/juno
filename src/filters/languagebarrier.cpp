#include <regex>
#include <config.hpp>
#include <logging.hpp>
#include <filters/languagebarrier.hpp>

bool juno::languagebarrier::should_filter(const dpp::message_create_t& message, dpp::cluster& cluster) const {
    for (const auto& r : m_regexes) {
        if (std::regex_search(message.msg.content, r)) {
            return true;
        }
    }

    return false;
}

void juno::languagebarrier::on_filter_execution(const dpp::message_create_t& message, dpp::cluster& cluster) {
    juno::global_log << logging::loglevel::info << ' '
                     << message.msg.author.username << " (" << message.msg.author.id << ") was censored for saying \""
                     << message.msg.content << "\"\n";

    cluster.message_delete(message.msg.id, message.msg.channel_id);
}
