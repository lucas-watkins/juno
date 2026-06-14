#ifndef JUNO_CATS_HPP
#define JUNO_CATS_HPP

#include <command.hpp>

namespace juno {
    // Gets cat images from cat as a service (https://cataas.com)
    class cat : public command {
        // Respond to http request to service
        static void post_image_to_channel(const dpp::slashcommand_t& event, const dpp::http_request_completion_t& req);

    public:
        constexpr std::string name() const override {
            return "cat";
        }

        dpp::slashcommand make_command(dpp::cluster& cluster) const override;
        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) override;
    };
}

#endif //JUNO_CATS_HPP
