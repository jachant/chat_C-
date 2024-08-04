cmake_minimum_required(VERSION 3.14)
project(Client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(SOURCE_FILES source/main.cpp
        source/App.cpp
        source/App.h
        source/Reader.cpp
        source/Reader.h
        source/Textbox.cpp
        source/Textbox.h
        source/Button.cpp
        source/Button.h
        source/Bubble.cpp
        source/Bubble.h
        source/RoundedRect.cpp
        source/RoundedRect.h
        source/Login.cpp
        source/Login.h
        source/ChatLabel.cpp
        source/ChatLabel.h
        source/Server.cpp
        source/Server.h
        source/Chat.cpp
        source/Chat.h
)
add_executable(Client ${SOURCE_FILES})

# Добавление textures
FILE(
        COPY "textures"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)
FILE(
        COPY "config.txt"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)
FILE(
        COPY "fonts"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)
if(WIN32)
    set(SFML_ROOT "C:\\SFML-2.6.1")
    set(SFML_STATIC_LIBRARIES TRUE)
else()
    include_directories(/usr/local/include)
endif()

# Поиск компонентов библиотеки
find_package(SFML 2.6.1 COMPONENTS graphics window system network REQUIRED)

if(SFML_FOUND)
    # Добавление headers
    target_include_directories(Client PRIVATE ${SFML_INCLUDE_DIR})
    # Линковка библиотек
    target_link_libraries(Client PRIVATE m sfml-graphics sfml-window sfml-system sfml-network)
endif()
