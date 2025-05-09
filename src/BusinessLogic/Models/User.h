#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

class User {
public:

    uint64_t id_;
    std::string name_;
    std::string password_;
    uint64_t balance_;
    std::unordered_map<uint64_t, uint32_t> stocks_;

public:

    User(uint64_t id, std::string name, std::string password, uint64_t balance);

    User(const User& other);

    User& operator=(const User& other);

    friend bool operator==(const User& lhs, const User& rhs);
};