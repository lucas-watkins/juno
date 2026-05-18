cmake_minimum_required(VERSION 3.11)
include(FetchContent)
find_package(PkgConfig REQUIRED)
project(juno)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address -fsanitize=leak")

# Declare DPP
FetchContent_Declare(
    DPP
    GIT_REPOSITORY https://github.com/brainboxdotcc/DPP
    GIT_TAG v10.1.4
)

# Static libraries
set(BUILD_SHARED_LIBS OFF)

# Make DPP available
FetchContent_MakeAvailable(DPP)

# Find libqalculate (needs to be installed on system)
pkg_check_modules(LIBQALCULATE REQUIRED libqalculate)

# Copy config.json over for convenience
if (EXISTS "config.json")
    file(COPY "config.json" DESTINATION .)
endif()

add_executable(
        juno
        "src/main.cpp"
        "src/ping.cpp"
        "src/qalc.cpp"
)

target_include_directories(juno PRIVATE "${dpp_SOURCE_DIR}/include" ${LIBQALCULATE_INCLUDE_DIRS} "include")
target_link_libraries(juno dppstatic ${LIBQALCULATE_LIBRARIES})
target_compile_options(juno PRIVATE ${LIBQALCULATE_CFLAGS_OTHER})
