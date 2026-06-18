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
        std::vector<std::string> m_languagebarrier_regexes{};

        friend void to_json(nlohmann::json& j, const config& c);
        friend void from_json(const nlohmann::json& j, config& c);

    public:
        const std::string& token() const {
            return m_token;
        }

        const std::vector<std::string>& languagebarrier_regexes() const {
            return m_languagebarrier_regexes;
        }
    };

    inline void to_json(nlohmann::json& j, const config& c) {
        j = nlohmann::json{
            { "token", c.token() },
            {"languagebarrier_regexes", c.languagebarrier_regexes() }
        };
    }

    inline void from_json(const nlohmann::json& j, config& c) {
        j.at("token").get_to(c.m_token);

        if (j.contains("languagebarrier_regexes")) {
            j.at("languagebarrier_regexes").get_to(c.m_languagebarrier_regexes);
        } else {
            c.m_languagebarrier_regexes = {};
        }
    }

    inline config global_config{};
}

#endif //JUNO_CONFIG_HPP
