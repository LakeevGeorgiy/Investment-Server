#pragma once

#include <memory>
#include <vector>

#include "../DataAccess/Repositories/StockRepository.h"

class StockService {
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

private:

    pointer<StockRepository> stock_repository_;

public:

    StockService(pointer<StockRepository>& stock_repository);
    StockService(const StockService& other);
    void operator=(const StockService& other);

    std::vector<Stock> ListStocks();
    Stock GetStock(uint64_t id);
    void BuyStocks(uint64_t stock_id, uint32_t count);
    void SellStocks(uint64_t stock_id, uint32_t count);
};