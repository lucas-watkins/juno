//
// Created by lucas on 5/23/26.
//

#ifndef JUNO_LOGGING_HPP
#define JUNO_LOGGING_HPP

#include <optional>
#include <dpp/dpp.h>
#include <util.hpp>

namespace juno {

    // Juno's logging always goes to standard output
    class logging {
        static std::string time();

        public:
        enum class loglevel {
            debug,
            info,
            warning,
            error
        };

        template <typename T>
        friend const juno::logging& operator<<(const juno::logging& l, const T&);
    };

    // log instance
    inline logging log{};

    template <typename T>
    [[jetbrains::has_side_effects]] const juno::logging& operator<<(const juno::logging& l, const T& t) {
        std::cout << t;
        return l;
    }

    template <>
    inline const juno::logging& operator<<(const juno::logging& l, const dpp::slashcommand_t& event) {
        const dpp::interaction& cmd{ event.command };
        std::cout << cmd.get_issuing_user().format_username() << " used " << cmd.get_command_name();

        if (const auto& opts{ cmd.get_command_interaction().options }; !opts.empty()) {
            std::cout << " with options: \n";

            for (const auto& option : opts) {
                std::cout << std::setw(53 /* len of date and level */ + option.name.size()) << option.name << " -> ";

                if (const std::optional v{ juno::util::variant_to_string(option.value) }) {
                    std::cout << *v << '\n';
                } else {
                    std::cout << "[null value]\n";
                }
            }
        } else {
            std::cout << '\n';
        }

        return l;
    }

    template <>
    inline const juno::logging& operator<<(const juno::logging& l, const juno::logging::loglevel& lv) {
        std::cout << '[' << juno::logging::time() << ' ';

        switch (lv) {
            case logging::loglevel::debug:
            std::cout << "DBG";
            break;

            case logging::loglevel::info:
            std::cout << "INF";
            break;

            case logging::loglevel::warning:
            std::cout << "WRN";
            break;

            case logging::loglevel::error:
            std::cout << "ERR";
            break;

            // Shouldn't happen
            default:
            std::cout << "UNK";
            break;
        }

        std::cout << ']';

        return l;
    }


}

#endif //JUNO_LOGGING_HPP