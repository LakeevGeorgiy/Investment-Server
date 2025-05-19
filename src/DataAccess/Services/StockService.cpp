#include <stdexcept>

#include "StockService.h"

StockService::StockService(std::shared_ptr<StockRepositoryInterface>& repository): repository_(repository) {}

StockService::StockService(StockService&& other)
    : repository_(other.repository_) 
{}

StockService& StockService::operator=(StockService&& other) {
    if (this == &other) {
        return *this;
    }

    repository_ = other.repository_;
    return *this;
}

ResultType<std::vector<Stock>> StockService::ListStocks(){
    return repository_->ReadStocks();
}


ResultType<Stock> StockService::GetStock(uint64_t id) {
    return repository_->ReadStock(id);
}