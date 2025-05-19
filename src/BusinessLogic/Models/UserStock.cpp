#include "UserStock.h"

UserStock::UserStock(uint64_t stock_id, uint32_t count)
    : stock_id_(stock_id)
    , count_(count)
{ }

UserStock::UserStock(const UserStock &other)
    : UserStock(other.stock_id_, other.count_) 
{ }

UserStock &UserStock::operator=(const UserStock &other) {
    if (this == &other) {
        return *this;
    }

    stock_id_ = other.stock_id_;
    count_ = other.count_;
    return *this;
}
