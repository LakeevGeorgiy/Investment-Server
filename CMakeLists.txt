cmake_minimum_required(VERSION 3.12)
project(Server)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)
enable_testing()

add_subdirectory(src/Presentation)
add_subdirectory(src/DataAccess)
add_subdirectory(src/BusinessLogic)
# add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
            PresentationLayer
            BusinessLayer
            DataLayer
        PUBLIC
            ${Boost_LIBRARIES}
)
