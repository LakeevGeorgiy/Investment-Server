project(BusinessLayer)

add_library(${PROJECT_NAME}
            Models/User.h
            Models/User.cpp
            Models/Stock.h 
            Models/Stock.cpp
            Models/ResultType.h
            Repositories/UserRepositoryInterface.h
)

target_link_libraries(BusinessLayer PRIVATE DataLayer)