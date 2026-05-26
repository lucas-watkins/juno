#ifndef JUNO_QALC_HPP
#define JUNO_QALC_HPP

#include <memory>
#include <module.hpp>
#include <libqalculate/qalculate.h>

namespace juno {
    class qalc : public easy_module<qalc> {
        std::unique_ptr<Calculator> m_calculator{ std::make_unique<Calculator>() };


        constexpr std::string name() const override {
            return "qalc";
        }

        registration_result make_command(dpp::cluster& cluster) const override;
        void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const override;
    };
}

#endif //JUNO_QALC_HPP
