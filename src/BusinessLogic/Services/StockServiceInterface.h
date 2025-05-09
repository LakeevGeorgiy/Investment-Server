#pragma once

#include <memory>
#include <vector>

#include "../Models/Stock.h"
#include "../Models/ResultType.h"

class StockServiceInterface {
public:

    virtual std::vector<Stock> ListStocks() = 0;
    virtual ResultType<Stock> GetStock(uint64_t id) = 0;
    virtual ResultType<void> BuyStocks(uint64_t stock_id, uint32_t count) = 0;
    virtual ResultType<void> SellStocks(uint64_t stock_id, uint32_t count) = 0;

};