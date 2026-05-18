#ifndef JUNO_UTIL_HPP
#define JUNO_UTIL_HPP

#include <string>
#include <sstream>
#include <iomanip>

namespace juno {
    namespace util {

        inline std::string to_codeblock(const std::string& str) {
            std::stringstream ss{};
            ss << std::quoted(str, '`');

            return ss.str();
        }

    }
}

#endif //JUNO_UTIL_HPP