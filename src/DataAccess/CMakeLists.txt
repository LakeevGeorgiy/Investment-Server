include(FetchContent)
project(DataLayer)

FetchContent_Declare(
  libpqxx
  GIT_REPOSITORY https://github.com/jtv/libpqxx.git
  GIT_TAG a6b1d60e74c1427c8ac2324b85cd4a0dc2068332
)
set(PQXX_LIBRARIES pqxx_static)

FetchContent_MakeAvailable(libpqxx)

add_library(${PROJECT_NAME}
    Repositories/StockRepository.h 
    Repositories/StockRepository.cpp
    Repositories/UserRepository.h 
    Repositories/UserRepository.cpp 
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