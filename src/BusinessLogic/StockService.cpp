#include <stdexcept>

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


ResultType<Stock> StockService::GetStock(uint64_t id) {
    if (!stock_repository_->FindStock(id)) {
        return ResultType<Stock>(
            std::make_shared<std::runtime_error>("no such stock"));
    }
    auto stock = stock_repository_->ReadStock(id);
    return ResultType<Stock>(stock);
}

ResultType<void> StockService::BuyStocks(uint64_t stock_id, uint32_t count){
    if (!stock_repository_->FindStock(stock_id)) {
        return ResultType<void>(
            std::make_shared<std::runtime_error>("no such stocks in exchange glass"));
    }

    Stock stock = stock_repository_->ReadStock(stock_id);
    if (stock.count_ < count) {
        return ResultType<void>(
            std::make_shared<std::runtime_error>("you can't buy more than glass have"));

    }
    stock.count_ -= count;
    stock_repository_->UpdateStock(stock);
    return ResultType<void>();
}

ResultType<void> StockService::SellStocks(uint64_t stock_id, uint32_t count) {
    if (!stock_repository_->FindStock(stock_id)) {
        return ResultType<void>(
            std::make_shared<std::runtime_error>("Stocks are not found"));
    }

    Stock stock = stock_repository_->ReadStock(stock_id);
    stock.count_ += count;
    stock_repository_->UpdateStock(stock);
    return ResultType<void>();
}