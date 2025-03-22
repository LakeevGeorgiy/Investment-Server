#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <gtest/gtest.h>

#include "../src/Presentation/Server.h"

class ServiceTests : public testing::Test{
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

public:

    std::vector<Stock> stocks_;
    std::vector<User> users_;

    pointer<StockRepository> stock_repository_;
    pointer<StockService> stock_service_;
    pointer<UserRepository> user_repository_;
    pointer<UserService> user_service_;
    pointer<Server> server_;

    void InsertStocks() {
        stocks_ = {
            Stock(1, 2000, 1000, "Yandex"),
            Stock(2, 4500, 10'000, "Google"),
            Stock(3, 300, 1'000'000, "Sber"),
            Stock(4, 54000, 100, "Tesla"),
            Stock(5, 320, 10, "Ozon")
        };

        for (std::size_t i = 0; i < stocks_.size(); ++i) {
            stock_repository_->CreateStock(stocks_[i]);
        }
    }

    void InsertUsers() {
        users_ = {
            User(1, "Georgiy", "123"),
            User(2, "Vlad", "456"),
            User(3, "Andrew", "789"),
            User(4, "Achmed", "1011"),
            User(5, "Khabib", "1213")
        };

        for (std::size_t i = 0; i < users_.size(); ++i) {
            user_repository_->CreateUser(users_[i]);
        }
    }

public:

    ServiceTests() {
        stock_repository_ = std::make_shared<StockRepository>();
        stock_service_ = std::make_shared<StockService>(stock_repository_);
        user_repository_ = std::make_shared<UserRepository>();
        user_service_ = std::make_shared<UserService>(user_repository_);

        InsertStocks();
        InsertUsers();
    }

};

TEST_F(ServiceTests, ListStocksService) {
    auto stocks = stock_service_->ListStocks();
    std::sort(stocks.begin(), stocks.end(), [](Stock& lhs, Stock& rhs){
        return lhs.id_ < rhs.id_;
    });
    for (size_t i = 0; i < stocks_.size(); ++i) {
        ASSERT_EQ(stocks_[i], stocks[i]);
    }
}

TEST_F(ServiceTests, GetStockService) {
    for (size_t i = 0; i < stocks_.size(); ++i) {
        ASSERT_EQ(stocks_[i], stock_service_->GetStock(stocks_[i].id_));
    }
}

TEST_F(ServiceTests, BuyStockService) {
    stocks_[2].count_ -= 100;
    stock_service_->BuyStocks(stocks_[2].id_, 100);
    ASSERT_EQ(stocks_[2], stock_service_->GetStock(stocks_[2].id_));
}

TEST_F(ServiceTests, SellStockService) {
    stocks_[3].count_ += 100;
    stock_service_->SellStocks(stocks_[3].id_, 100);
    ASSERT_EQ(stocks_[3], stock_service_->GetStock(stocks_[3].id_));
}

TEST_F(ServiceTests, BuyUserService) {
    user_service_->BuyStocks(1, stocks_[2].id_, 100);

    auto stocks = user_service_->ListUsersStock(1);
    ASSERT_EQ(stocks.size(), 1);
    ASSERT_EQ(stocks[0], stocks_[2].id_);
}

TEST_F(ServiceTests, SellUserService) {
    user_service_->BuyStocks(1, stocks_[1].id_, 10);
    user_service_->BuyStocks(1, stocks_[2].id_, 100);
    user_service_->SellStocks(1, stocks_[2].id_, 100);

    auto stocks = user_service_->ListUsersStock(1);
    ASSERT_EQ(stocks.size(), 1);
    ASSERT_EQ(stocks[0], stocks_[1].id_);
}

TEST_F(ServiceTests, CreateUserService) {
    User user(6, "Voin", "acd");
    user_service_->CreateNewUser(user);
    ASSERT_EQ(user, user_repository_->ReadUser(6));
}

TEST_F(ServiceTests, ChangePasswordService) {
    users_[4].password_ = "user";
    user_service_->ChangePassword(users_[4].id_, "user");
    ASSERT_EQ(users_[4], user_repository_->ReadUser(users_[4].id_));
}