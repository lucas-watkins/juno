#ifndef JUNO_QALC_HPP
#define JUNO_QALC_HPP

#include <memory>
#include <command.hpp>
#include <libqalculate/qalculate.h>

namespace juno {
    class qalc : public easy_command<qalc> {
        std::unique_ptr<Calculator> m_calculator{ std::make_unique<Calculator>() };


        constexpr std::string name() const override {
            return "qalc";
        }

        dpp::slashcommand make_command(dpp::cluster& cluster) const override;
        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const override;
    };
}

#endif //JUNO_QALC_HPP
