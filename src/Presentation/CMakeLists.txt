cmake_minimum_required(VERSION 3.12)
project(PresentationLayer)

add_library(${PROJECT_NAME} 
            Server.h 
            Server.cpp
            Session.h 
            Session.cpp
            Listener.h 
            Listener.cpp
            TcpServer.h 
            TcpServer.cpp
)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(Boost REQUIRED)

target_include_directories(${PROJECT_NAME} 
        PUBLIC
            ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
            BusinessLayer
            DataLayer
        PUBLIC
            ${Boost_LIBRARIES}
            nlohmann_json::nlohmann_json
)

target_link_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE BusinessLayer DataLayer)