#pragma once

#include <vector>
#include <iostream>

#include "../Models/Stock.h"

class StockRepositoryInterface {
public:

    virtual void CreateStock(const Stock& new_stock) = 0;
    virtual std::vector<Stock> ReadStocks() = 0;
    virtual Stock ReadStock(uint64_t id) = 0;
    virtual bool FindStock(uint64_t id) = 0;
    virtual void UpdateStock(const Stock& updated_stock) = 0;
    virtual void DeleteStock(uint64_t id) = 0;

};