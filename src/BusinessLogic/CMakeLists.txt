project(BusinessLayer)

add_library(${PROJECT_NAME}
            Models/User.h
            Models/User.cpp
            Models/Stock.h 
            Models/Stock.cpp
            Models/UserStock.h 
            Models/UserStock.cpp
            Models/ResultType.h
            Repositories/UserRepositoryInterface.h
            Repositories/StockRepositoryInterface.h 
            Repositories/DatabaseConnectionInterface.h 
            Repositories/UserStockRepositoryInterface.h
            Services/StockServiceInterface.h 
            Services/UserServiceInterface.h 
            Services/UserStockServiceInterface.h
)

# target_link_libraries(BusinessLayer PRIVATE DataLayer)