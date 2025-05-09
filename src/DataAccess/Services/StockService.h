#pragma once

#include <memory>
#include <vector>

#include "../../BusinessLogic/Repositories/StockRepositoryInterface.h"
#include "../../BusinessLogic/Services/StockServiceInterface.h"

class StockService : public StockServiceInterface {
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

private:

    pointer<StockRepositoryInterface> stock_repository_;

public:

    StockService(pointer<StockRepositoryInterface>& stock_repository);
    StockService(const StockService& other);
    void operator=(const StockService& other);

    std::vector<Stock> ListStocks() override;
    ResultType<Stock> GetStock(uint64_t id) override;
    ResultType<void> BuyStocks(uint64_t stock_id, uint32_t count) override;
    ResultType<void> SellStocks(uint64_t stock_id, uint32_t count) override;
};