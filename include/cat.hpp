//
// Created by lucas on 5/24/26.
//

#ifndef JUNO_CATS_HPP
#define JUNO_CATS_HPP

#include <module.hpp>

namespace juno {

    // Gets cat images from cat as a service (https://cataas.com)
    class cat : public easy_module<cat> {
    public:
        constexpr std::string name() const override {
            return "cat";
        }

        dpp::slashcommand make_command(dpp::cluster& cluster) const override;
        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const override;
    };
}

#endif //JUNO_CATS_HPP
