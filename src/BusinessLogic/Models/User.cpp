#include "User.h"

User::User(uint64_t id, std::string_view name, std::string_view email, std::string_view password, uint64_t balance)
    : id_(id)
    , name_(name)
    , email_(email)
    , password_(password)
    , balance_(balance)
{}

User::User(const User& other)
    : User(other.id_, other.name_, other.email_, other.password_, other.balance_) 
{ }

User& User::operator=(const User& other) {
    if (this == &other) {
        return *this;
    }

    id_ = other.id_;
    name_ = other.name_;
    email_ = other.email_;
    password_ = other.password_;
    balance_ = other.balance_;
    return *this;
}

bool operator==(const User& lhs, const User& rhs) {
    return lhs.email_ == rhs.email_;
}
