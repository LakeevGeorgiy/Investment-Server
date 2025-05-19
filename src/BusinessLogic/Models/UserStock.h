#pragma once

#include <cstdint>

class UserStock {
public:

    uint64_t stock_id_;
    uint32_t count_;

public:

    UserStock(uint64_t stock_id, uint32_t count);
    UserStock(const UserStock& other);
    UserStock& operator=(const UserStock& other);

    friend bool operator==(const UserStock& lhs, const UserStock& rhs);
};
