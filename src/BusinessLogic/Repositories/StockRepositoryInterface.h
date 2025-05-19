#pragma once

#include <vector>
#include <iostream>

#include "../Models/Stock.h"
#include "../Models/ResultType.h"

class StockRepositoryInterface {
public:

    virtual ResultType<uint64_t> CreateStock(const Stock& stock) = 0;
    virtual ResultType<std::vector<Stock>> ReadStocks() = 0;
    virtual ResultType<Stock> ReadStock(uint64_t id) = 0;
    virtual ResultType<void> UpdateStock(const Stock& stock) = 0;
    virtual ResultType<void> DeleteStock(uint64_t id) = 0;
    virtual ~StockRepositoryInterface() = default;
};