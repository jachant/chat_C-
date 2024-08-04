cmake_minimum_required(VERSION 3.27)
project(Server)

set(CMAKE_CXX_STANDARD 17)

add_executable(Server main.cpp
        Server.cpp
        Server.h
        User.cpp
        User.h
        Chat.cpp
        Chat.h
        defines.h
)

if(WIN32)
    set(SFML_ROOT "C:\\SFML-2.6.1")
    set(SFML_STATIC_LIBRARIES TRUE)
else()
    include_directories(/usr/local/include)
endif()

# Поиск компонентов библиотеки
find_package(SFML 2.6.1 COMPONENTS graphics window  network system REQUIRED)

if(SFML_FOUND)
    # Добавление headers
    target_include_directories(Server PRIVATE ${SFML_INCLUDE_DIR})
    # Линковка библиотек
    target_link_libraries(Server PRIVATE m sfml-graphics sfml-window sfml-network sfml-system)
endif()
