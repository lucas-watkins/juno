#ifndef JUNO_LOGGING_HPP
#define JUNO_LOGGING_HPP

// For ignoring compiler errors caused by the jetbrains attribute below
#pragma GCC diagnostic ignored "-Wattributes"

#include <optional>
#include <dpp/dpp.h>

namespace juno {
    // Forward declaration
    namespace util {
        template <typename... Ts>
        std::optional<std::string> variant_to_string(const std::variant<Ts...>& v);
    }

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
        const dpp::user& cmd_issuer{ cmd.get_issuing_user() };
        std::cout << cmd_issuer.username << " (" << cmd_issuer.id << ")" << " used " << cmd.get_command_name();

        if (const auto opts{ cmd.get_command_interaction().options }; !opts.empty()) {
            std::cout << " with options: \n";

            for (auto option{ opts.cbegin() }; option != opts.cend(); ++option) {
                std::cout << std::setw(53 /* len of date and level */ + option->name.size()) << option->name << " -> ";

                if (const std::optional v{ juno::util::variant_to_string(option->value) }) {
                    std::cout << *v;
                } else {
                    std::cout << "[null value]";
                }

                if (option < opts.cend() - 1) {
                    std::cout << '\n';
                }
            }
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
