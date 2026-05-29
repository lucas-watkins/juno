//
// Created by lucas on 5/29/26.
//

#ifndef JUNO_DEADLOCKVER_HPP
#define JUNO_DEADLOCKVER_HPP

#include <module.hpp>

namespace juno {
    class deadlockversion : public easy_module<deadlockversion> {
    public:
        constexpr std::string name() const override {
            return "deadlockversion";
        }

        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const override;
        dpp::slashcommand make_command(dpp::cluster& cluster) const override;
    };
}

#endif //JUNO_DEADLOCKVER_HPP
