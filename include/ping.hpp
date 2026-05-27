#ifndef JUNO_PINGPONG_HPP
#define JUNO_PINGPONG_HPP

#include <module.hpp>

namespace juno {
    class ping : public easy_module<ping> {
        constexpr std::string name() const override {
            return "ping";
        }

        dpp::slashcommand make_command(dpp::cluster& cluster) const override;
        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const override;
    };
}

#endif //JUNO_PINGPONG_HPP