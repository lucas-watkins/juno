#ifndef JUNO_CONFIG_HPP
#define JUNO_CONFIG_HPP

#include <string>
#include <dpp/nlohmann/json.hpp>

namespace juno {

    /*
     * Class to manage configuration.
     * Construction is forbidden, get from nlohmann JSON instead.
     */
    class config {
        std::string m_token{};

        friend void to_json(nlohmann::json& j, const config& c);
        friend void from_json(const nlohmann::json& j, config& c);

        public:
        const std::string& token() const { return m_token; }
    };

    inline void to_json(nlohmann::json& j, const config& c) {
        j = nlohmann::json{{ "token", c.token() }};
    }

    inline void from_json(const nlohmann::json& j, config& c) {
        j.at("token").get_to(c.m_token);
    }
}

#endif //JUNO_CONFIG_HPP