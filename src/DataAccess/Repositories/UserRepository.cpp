#include <mutex>
#include <sstream>
#include <optional>

#include "UserRepository.h"

UserRepository::UserRepository(std::string_view connection_string)
    : mutex_()
    , db_(DatabaseConnection::GetInstance(connection_string))
{}

UserRepository::UserRepository(UserRepository&& other)
    : db_(other.db_) 
{}

UserRepository& UserRepository::operator=(UserRepository&& other) {
    if (this == &other) {
        return *this;
    }
    db_ = std::move(other.db_);
    return *this;
}

ResultType<uint64_t> UserRepository::CreateUser(const User &user){
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        INSERT INTO users (name, email, password, balance) 
        VALUES ($1, $2, crypt($3, gen_salt('md5')), $4) 
        RETURNING id;
    )"};

    pqxx::params params{user.name_, user.email_, user.password_, user.balance_};
    auto result = db_->ExecQuery(query, params);
    uint64_t id = result[0][0].as<uint64_t>();
    return ResultType<uint64_t>(id);
}

ResultType<User> UserRepository::ReadUser(uint64_t id) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        SELECT id, name, email, password, balance
        FROM users
        WHERE id = $1;
    )"};

    pqxx::params params{id};
    auto result = db_->ExecQuery(query, params);
    std::optional<User> user = pqxx::string_traits<std::optional<User>>::from_string(result[0]);
    if (user) {
        return ResultType(*user);
    }
    return ResultType<User>(
        std::make_shared<std::runtime_error>("There is no user with this id")
    );
}

ResultType<User> UserRepository::LoginUser(std::string_view email, std::string_view password) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        SELECT id, name, email, password, balance
        FROM users
        WHERE email = $1 AND crypt($2, password);
    )"};

    pqxx::params params{email};
    auto result = db_->ExecQuery(query, params);
    std::optional<User> user = pqxx::string_traits<std::optional<User>>::from_string(result[0]);
    if (user) {
        return ResultType(*user);
    }
    return ResultType<User>(
        std::make_shared<std::runtime_error>("There is no user with this email and password")
    );
}

ResultType<void> UserRepository::UpdateUser(const User& user){
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        UPDATE users 
        SET name = $1, email = $2, 
        password = crypt($3, gen_salt('md5')), balance = $4
        WHERE id = $5
    )"};

    pqxx::params params{user.name_,user.email_,  user.password_, user.balance_, user.id_};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}

ResultType<void> UserRepository::DeleteUser(uint64_t id) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        DELETE FROM users WHERE email = $1;
    )"};

    pqxx::params params{id};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}