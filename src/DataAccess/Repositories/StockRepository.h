#pragma once

#include <memory>
#include <shared_mutex>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "../Stock.h"

class StockRepository {
public:

    using pointer = std::shared_ptr<Stock>;

private:

    uint64_t counter_;
    std::shared_mutex mutex_;
    std::unordered_map<uint64_t, pointer> stocks_;

public:

    StockRepository();
    StockRepository(const StockRepository& other);
    void operator=(const StockRepository& other);

    void CreateStock(const Stock& new_stock);
    std::vector<Stock> ReadStocks();
    Stock ReadStock(uint64_t id);
    bool FindStock(uint64_t id);
    void UpdateStock(const Stock& updated_stock);
    void DeleteStock(uint64_t id);
};