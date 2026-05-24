#include <logging.hpp>
#include <ctime>

std::string juno::logging::time() {
    const std::time_t now{ std::time(nullptr) };

    // buffer
    char formatted[100];
    std::strftime(formatted, std::size(formatted), "%c", std::localtime(&now));

    return formatted;
}
