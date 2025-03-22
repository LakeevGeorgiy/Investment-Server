add_library(BusinessLayer
            StockService.cpp 
            StockService.h 
            UserService.cpp 
            UserService.h
)

target_link_libraries(BusinessLayer PRIVATE DataLayer)