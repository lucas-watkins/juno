#ifndef JUNO_UTIL_HPP
#define JUNO_UTIL_HPP

#include <string>
#include <random>
#include <logging.hpp>

namespace juno::util {
    // To Markdown codeblock
    inline std::string to_codeblock(const std::string& str) {
        return std::format("`{}`", str);
    }

    // Creates an error embed with the specified parameters **AND** logs error to console
    inline dpp::embed log_and_error_embed(const std::string_view title, const std::string_view message) {

        juno::global_log << logging::loglevel::error << ' ' << title << ": " << message << '\n';

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
