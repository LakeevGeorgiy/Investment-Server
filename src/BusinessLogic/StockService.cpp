#include "StockService.h"

StockService::StockService(pointer<StockRepository>& stock_repository): stock_repository_(stock_repository) {}

StockService::StockService(const StockService &other){
    if (this == &other) {
        return;
    }

    stock_repository_ = other.stock_repository_;
}

void StockService::operator=(const StockService &other) {
    if (this == &other) {
        return;
    }

    stock_repository_ = other.stock_repository_;
}

std::vector<Stock> StockService::ListStocks(){
    return stock_repository_->ReadStocks();
}


Stock StockService::GetStock(uint64_t id) {
    return stock_repository_->ReadStock(id);
}

void StockService::BuyStocks(uint64_t stock_id, uint32_t count){
    if (!stock_repository_->FindStock(stock_id)) {
        throw std::exception();
    }

    Stock stock = stock_repository_->ReadStock(stock_id);
    if (stock.count_ < count) {
        throw std::exception();
    }
    stock.count_ -= count;
    stock_repository_->UpdateStock(stock);
}

void StockService::SellStocks(uint64_t stock_id, uint32_t count) {
    if (!stock_repository_->FindStock(stock_id)) {
        throw std::exception();
    }

    Stock stock = stock_repository_->ReadStock(stock_id);
    stock.count_ += count;
    stock_repository_->UpdateStock(stock);
}