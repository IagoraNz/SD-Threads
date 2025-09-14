cmake_minimum_required(VERSION 3.10)
project(trabalho04)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

# Inclui httplib (header-only)
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include)

# Se precisar de JSON (nlohmann/json)
find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json pthread)
