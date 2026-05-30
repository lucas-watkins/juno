#ifndef JUNO_UTIL_HPP
#define JUNO_UTIL_HPP

#include <string>
#include <sstream>
#include <optional>
#include <random>
#include <logging.hpp>

namespace juno::util {
    // To Markdown codeblock
    inline std::string to_codeblock(const std::string& str) {
        return std::format("`{}`", str);
    }

    /*
     * Returns optional string to get type from variant as long as it's stream insertable.
     * When a monostate is encountered, function returns std::nullopt
     */
    template <typename... Ts>
    std::optional<std::string> variant_to_string(const std::variant<Ts...>& v) {
        std::stringstream ss{};

        ([&] {
            if (std::holds_alternative<Ts>(v)) {
                if constexpr (!std::is_same_v<Ts, std::monostate>) {
                    ss << std::get<Ts>(v);
                }
            }
        }(), ...);

        if (!ss.view().empty()) {
            return ss.str();
        }

        return std::nullopt;
    }

    // Creates an error embed with the specified parameters **AND** logs error to console
    inline dpp::embed log_and_error_embed(const std::string_view title, const std::string_view message) {

        juno::log << logging::loglevel::error << ' ' << title << ": " << message << '\n';

        return dpp::embed{}
               .set_title(title)
               .add_field("Details", message)
               .set_color(dpp::colors::red)
               .set_thumbnail(
                   "https://media.discordapp.net/attachments/763092900972068888/1509036791502733454/juno_error.png"
                );
    }

    // Returns random integer N given A and B with A <= N <= B
    inline int randint(const int min, const int max) {
        // Mersenne twister
        static std::random_device rd{};
        static std::seed_seq seed{rd(), rd(), rd(), rd(), rd()};
        static std::mt19937 gen{seed};

        std::uniform_int_distribution dist{ min, max };

        return dist(gen);
    }
}

#endif //JUNO_UTIL_HPP
