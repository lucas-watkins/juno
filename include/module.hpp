#ifndef JUNO_MODULE_HPP
#define JUNO_MODULE_HPP

#include <dpp/dpp.h>
#include <variant>

namespace juno {
    // Defines a module, being the information necessary for a command.
    class module {
    public:
        // Returns either a single command or multiple commands
        class registration_result {
            std::variant<dpp::slashcommand, std::vector<dpp::slashcommand>> m_result{};

        public:
            // Perfect forwarding:
            // https://www.justsoftwaresolutions.co.uk/cplusplus/rvalue_references_and_perfect_forwarding.html


            template <typename T>
            registration_result(std::vector<T>&& result) : m_result{ std::forward<T>(result) } {};

            template <typename T> requires (!std::is_same_v<T, registration_result>)
            registration_result(T&& result) : m_result{ std::forward<T>(result) } {};

            template <typename T>
            T get() const {
                return std::get<T>(m_result);
            }

            template <typename T>
            bool holds() const {
                return std::holds_alternative<T>(m_result);
            }
        };

        // Command registration
        virtual registration_result make_command(dpp::cluster& cluster) const = 0;
        virtual void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) const = 0;

        constexpr virtual std::string name() const = 0;

        virtual ~module() = default;
    };

    // Easy template class to quickly create a module
    template <typename T>
    class easy_module : public module {
    public:
        inline static T instance{};
    };
}

#endif //JUNO_MODULE_HPP
