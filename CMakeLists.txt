cmake_minimum_required(VERSION 3.11)
include(FetchContent)
find_package(PkgConfig REQUIRED)
project(juno)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address -fsanitize=leak")

# Find libqalculate (needs to be installed on system)
pkg_check_modules(LIBQALCULATE REQUIRED libqalculate)

# Find dpp (needs to be installed on system)
pkg_check_modules(DPP REQUIRED dpp)

# Copy config.json over for convenience
if (EXISTS "config.json")
    file(COPY "config.json" DESTINATION .)
endif()

add_executable(
        juno
        "src/main.cpp"
        "src/ping.cpp"
        "src/qalc.cpp"
        "src/logging.cpp"
)

target_include_directories(juno PRIVATE ${DPP_INCLUDE_DIRS} ${LIBQALCULATE_INCLUDE_DIRS} "include")
target_link_libraries(juno ${LIBQALCULATE_LIBRARIES} ${DPP_LIBRARIES})
target_compile_options(juno PRIVATE ${LIBQALCULATE_CFLAGS_OTHER} ${DPP_CFLAGS_OTHER})
