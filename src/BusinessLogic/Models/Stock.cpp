#include "Stock.h"

Stock::Stock(uint64_t id, uint32_t cost, uint32_t count, std::string_view company_name):
        id_(id)
        , cost_(cost)
        , count_(count)
        , company_name_(company_name)
    {}

Stock::Stock(const Stock& other): Stock(other.id_, other.cost_, other.count_, other.company_name_) {}

Stock& Stock::operator=(const Stock& other) {
    if (this == &other) {
        return *this;
    }
    id_ = other.id_;
    cost_ = other.cost_;
    count_ = other.count_;
    company_name_ = other.company_name_;
    return *this;
}

bool operator==(const Stock& lhs, const Stock& rhs) {
    return lhs.id_ == rhs.id_ && lhs.cost_ == rhs.cost_ && lhs.count_ == rhs.count_ && lhs.company_name_ == rhs.company_name_;
}