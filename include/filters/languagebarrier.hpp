#ifndef JUNO_LANGUAGEBARRIER_HPP
#define JUNO_LANGUAGEBARRIER_HPP

#include <regex>
#include <dpp/dpp.h>
#include <filter.hpp>
#include <config.hpp>

namespace juno {
    // Regex based message censorship system
    class languagebarrier : public filter {
        // Proper type regexes
        std::vector<std::regex> m_regexes{};
    public:
        languagebarrier() {
            m_regexes.reserve(juno::global_config.languagebarrier_regexes().size());

            for (const auto& r : juno::global_config.languagebarrier_regexes()) {
                m_regexes.push_back(std::regex{ r });
            }
        }

        std::string name() const override {
            return "languagebarrier";
        }

        bool should_filter(const dpp::message_create_t& message, dpp::cluster& cluster) const override;
        void on_filter_execution(const dpp::message_create_t& message, dpp::cluster& cluster) override;

    };
}
#endif //JUNO_LANGUAGEBARRIER_HPP
