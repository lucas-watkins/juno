#ifndef JUNO_DEADLOCKVER_HPP
#define JUNO_DEADLOCKVER_HPP

#include <command.hpp>

namespace juno {
    class deadlockversion : public command {
    public:
        constexpr std::string name() const override {
            return "deadlockversion";
        }

        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) override;
        dpp::slashcommand make_command(dpp::cluster& cluster) const override;
    };
}

#endif //JUNO_DEADLOCKVER_HPP
