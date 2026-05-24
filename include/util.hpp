#ifndef JUNO_UTIL_HPP
#define JUNO_UTIL_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <optional>

namespace juno::util {

    inline std::string to_codeblock(const std::string& str) {
        return std::format("`{}`", str);
    }

    /*
     * Returns optional string to get type from variant as long as it's stream insertable.
     * When a monostate is encountered, function returns std::nullopt
     */
    template <typename ...Ts>
    std::optional<std::string> variant_to_string(const std::variant<Ts...>& v) {
        std::stringstream ss{};

        ([&]{
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

}

#endif //JUNO_UTIL_HPP
