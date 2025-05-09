#pragma once

#include <shared_mutex>
#include <unordered_map>
#include <memory>

#include "../../BusinessLogic/Repositories/StockRepositoryInterface.h"

class StockRepository : public StockRepositoryInterface {
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

    void CreateStock(const Stock& new_stock) override;
    std::vector<Stock> ReadStocks() override;
    Stock ReadStock(uint64_t id) override;
    bool FindStock(uint64_t id) override;
    void UpdateStock(const Stock& updated_stock) override;
    void DeleteStock(uint64_t id) override;
};