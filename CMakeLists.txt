cmake_minimum_required(VERSION 3.29)
project(Salon_Management)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")


include_directories(include)

add_definitions(-DMY_CODE)
add_subdirectory(FTXUI)

add_executable(Salon_Management
        main.cpp
        src/Database.cpp
        src/serviceDone.cpp
        src/Identifiable.cpp
        src/User.cpp
        src/Appointment.cpp
        src/define.cpp
        src/Datetime.cpp
        src/Salon.cpp
        src/Statistics.cpp
        src/call.cpp
        src/ui.cpp
        src/Salon.cpp
)

# Dùng các thành phần trong thư viện FTXUI
target_link_libraries(Salon_Management
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)