#pragma once

#include <memory>
#include <vector>

#include "../Models/Stock.h"
#include "../Models/ResultType.h"

class StockServiceInterface {
public:

    virtual ResultType<std::vector<Stock>> ListStocks() = 0;
    virtual ResultType<Stock> GetStock(uint64_t id) = 0;
    virtual ~StockServiceInterface() = default;

};