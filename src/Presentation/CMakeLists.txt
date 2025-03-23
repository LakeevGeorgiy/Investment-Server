cmake_minimum_required(VERSION 3.12)
project(PresentationLayer)

add_library(${PROJECT_NAME} 
            Server.h 
            Server.cpp
            Session.h 
            Session.cpp
            Listener.h 
            Listener.cpp
)

find_package(Boost REQUIRED)

target_include_directories(${PROJECT_NAME} 
        PUBLIC 
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PUBLIC
            ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
            BusinessLayer
        PUBLIC
            ${Boost_LIBRARIES}
)

target_link_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE BusinessLayer)