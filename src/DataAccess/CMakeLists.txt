include(FetchContent)
project(DataLayer)

FetchContent_Declare(
  libpqxx
  GIT_REPOSITORY https://github.com/jtv/libpqxx.git
  GIT_TAG 9bfaf9cb5a73ac23f9b7a9dfd7ef069a6e2124fb
)
set(PQXX_LIBRARIES pqxx)

FetchContent_MakeAvailable(libpqxx)


add_library(${PROJECT_NAME}
    Repositories/StockRepository.h 
    Repositories/StockRepository.cpp
    Repositories/UserRepository.h 
    Repositories/UserRepository.cpp 
    Repositories/DatabaseConnection.h
    Repositories/DatabaseConnection.cpp 
    Services/StockService.cpp 
    Services/StockService.h 
    Services/UserService.cpp 
    Services/UserService.h
)

target_link_libraries(${PROJECT_NAME} 
      PUBLIC 
        ${PQXX_LIBRARIES} 
        BusinessLayer
)