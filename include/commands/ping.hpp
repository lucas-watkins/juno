#ifndef JUNO_PINGPONG_HPP
#define JUNO_PINGPONG_HPP

#include <command.hpp>

namespace juno {
    class ping : public command {
    public:
        constexpr std::string name() const override {
            return "ping";
        }

        dpp::slashcommand make_command(dpp::cluster& cluster) const override;
        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) override;
    };
}

#endif //JUNO_PINGPONG_HPP