#pragma once

#include <memory>
#include <vector>

#include "../../BusinessLogic/Repositories/StockRepositoryInterface.h"
#include "../../BusinessLogic/Services/StockServiceInterface.h"

class StockService : public StockServiceInterface {
private:

    std::shared_ptr<StockRepositoryInterface> repository_;

public:

    StockService(std::shared_ptr<StockRepositoryInterface>& repository);
    StockService(const StockService& other) = delete;
    StockService(StockService&& other);
    StockService& operator=(const StockService& other) = delete;
    StockService& operator=(StockService&& other);

    ResultType<std::vector<Stock>> ListStocks() override;
    ResultType<Stock> GetStock(uint64_t id) override;
};