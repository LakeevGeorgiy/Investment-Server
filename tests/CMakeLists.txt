project(Server_test)
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        ${PROJECT_NAME}
        Repository_tests.cpp
        Service_tests.cpp
        Server_tests.cpp
)

target_link_libraries(${PROJECT_NAME} 
        PUBLIC 
            PresentationLayer
            BusinessLayer
            DataLayer
            GTest::gtest_main
        PUBLIC
            ${Boost_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})