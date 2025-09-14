cmake_minimum_required(VERSION 3.10)
project(client_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# tenta Qt6, senão usa Qt5
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_LIBS Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_LIBS Qt5::Widgets)
endif()

add_executable(client_gui main.cpp)

# includes: nlohmann json (system) and httplib header-only
target_include_directories(client_gui PRIVATE /usr/local/include)

# link Qt
target_link_libraries(client_gui PRIVATE ${QT_LIBS} pthread)

# link nlohmann_json if available via find_package (optional)
find_package(nlohmann_json QUIET)
if (nlohmann_json_FOUND)
    target_link_libraries(client_gui PRIVATE nlohmann_json::nlohmann_json)
endif()
