cmake_minimum_required(VERSION 3.11)
include(FetchContent)
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

# Make DPP and Qalculate Available
FetchContent_MakeAvailable(DPP)

# Copy config.json over for convenience
file(COPY "config.json" DESTINATION .)

add_executable(
        juno
        "src/main.cpp"
        "src/ping.cpp"
)

target_include_directories(juno PRIVATE "${dpp_SOURCE_DIR}/include" "include")
target_link_libraries(juno dppstatic)

