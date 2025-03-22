#include <mutex>

#include "StockRepository.h"

StockRepository::StockRepository(): counter_(1), mutex_(), stocks_(){}

StockRepository::StockRepository(const StockRepository &other){
    if (this == &other) {
        return;
    }

    counter_ = other.counter_;
    stocks_ = other.stocks_;
}

void StockRepository::operator=(const StockRepository &other) {
    if (this == &other) {
        return;
    }

    counter_ = other.counter_;
    stocks_ = other.stocks_;
}

void StockRepository::CreateStock(const Stock &new_stock)
{
    std::unique_lock lock(mutex_);
    auto ptr = std::shared_ptr<Stock>(new Stock(new_stock));
    ptr->id_ = counter_;
    stocks_[counter_++] = ptr;
}

std::vector<Stock> StockRepository::ReadStocks() {
    std::shared_lock lock(mutex_);
    std::vector<Stock> result;
    for (auto& [key, value] : stocks_){
        result.emplace_back(*value.get());
    }
    return result;
}

Stock StockRepository::ReadStock(uint64_t id) {
    return *stocks_[id].get();
}

bool StockRepository::FindStock(uint64_t id){
    std::shared_lock lock(mutex_);
    return stocks_.find(id) != stocks_.end();
}

void StockRepository::UpdateStock(const Stock& updated_stock) {
    std::unique_lock lock(mutex_);
    auto stock = stocks_[updated_stock.id_];
    stock->cost_ = updated_stock.cost_;
    stock->count_ = updated_stock.count_;
}

void StockRepository::DeleteStock(uint64_t id) {
    std::unique_lock lock(mutex_);
    stocks_.erase(id);
}