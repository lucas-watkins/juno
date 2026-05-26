//
// Created by lucas on 5/24/26.
//

#ifndef JUNO_CATS_HPP
#define JUNO_CATS_HPP

#include <module.hpp>

namespace juno {
    // Gets cat images from cat as a service (https://cataas.com)
    class cat : public easy_module<cat> {
        // Respond to http request to service
        static void post_image_to_channel(const dpp::slashcommand_t& event, const dpp::http_request_completion_t& req);

    public:
        constexpr std::string name() const override {
            return "cat";
        }

        registration_result make_command(dpp::cluster& cluster) const override;
        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const override;
    };
}

#endif //JUNO_CATS_HPP
