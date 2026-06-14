#ifndef JUNO_MODULE_HPP
#define JUNO_MODULE_HPP

#include <dpp/dpp.h>

namespace juno {
    // Defines a command which is explicitly triggered by the user
    class command {
    public:

        // Command registration
        virtual dpp::slashcommand make_command(dpp::cluster& cluster) const = 0;
        virtual void on_command_execution(const dpp::slashcommand_t& event, dpp::cluster& cluster) = 0;

        virtual std::string name() const = 0;

        virtual ~command() = default;
    };
}

#endif //JUNO_MODULE_HPP
