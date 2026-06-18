#ifndef JUNO_FILTER_HPP
#define JUNO_FILTER_HPP

#include <dpp/dpp.h>
#include <string>

namespace juno {
    // Used to filter each message and take an action based on it
    class filter {
    public:
        virtual bool should_filter(const dpp::message_create_t& message, dpp::cluster& cluster) const = 0;
        virtual void on_filter_execution(const dpp::message_create_t& message, dpp::cluster& cluster) = 0;
        virtual std::string name() const = 0;

        virtual ~filter() = default;
    };
}

#endif //JUNO_FILTER_HPP
