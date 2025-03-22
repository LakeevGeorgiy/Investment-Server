#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <gtest/gtest.h>

#include "../src/Presentation/Server.h"

class RepositoryTests : public testing::Test{
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

public:

    std::vector<Stock> stocks_;
    std::vector<User> users_;

    pointer<StockRepository> stock_repository_;
    pointer<UserRepository> user_repository_;

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

    RepositoryTests() {
        stock_repository_ = std::make_shared<StockRepository>();
        user_repository_ = std::make_shared<UserRepository>();

        InsertStocks();
        InsertUsers();
    }

};

TEST_F(RepositoryTests, ConstructStocks){
    ASSERT_EQ(stocks_.size(), 5);
}

TEST_F(RepositoryTests, ConstructUsers) {
    ASSERT_EQ(users_.size(), 5);
}

TEST_F(RepositoryTests, InitializationStockRepository) {
    auto stocks = stock_repository_->ReadStocks();
    ASSERT_EQ(stocks.size(), 5);
}

TEST_F(RepositoryTests, InitializationUserRepository) {
    auto users = user_repository_->ReadUsers();
    ASSERT_EQ(users.size(), 5);
}

TEST_F(RepositoryTests, EqualOfStocksAndRepository) {
    auto stocks = stock_repository_->ReadStocks();
    std::sort(stocks.begin(), stocks.end(), [](Stock& lhs, Stock& rhs) {
        return lhs.id_ < rhs.id_;
    });
    for (int i = 0; i < stocks_.size(); ++i) {
        ASSERT_EQ(stocks_[i], stocks[i]);
    }
}

TEST_F(RepositoryTests, EqualOfUserAndRepository) {
    auto users = user_repository_->ReadUsers();
    std::sort(users.begin(), users.end(), [](User& lhs, User& rhs) {
        return lhs.id_ < rhs.id_;
    });
    for (size_t i = 0; i < users_.size(); ++i) {
        ASSERT_EQ(users[i], users_[i]);
    }
}

TEST_F(RepositoryTests, ReadOneStockRepository) {
    for (size_t i = 0; i < stocks_.size(); ++i) {
        ASSERT_EQ(stocks_[i], stock_repository_->ReadStock(stocks_[i].id_));
    }   
}

TEST_F(RepositoryTests, ReadOneUserRepository) {
    for (size_t i = 0; i < stocks_.size(); ++i) {
        ASSERT_EQ(users_[i], user_repository_->ReadUser(users_[i].id_));
    }
}

TEST_F(RepositoryTests, CreateStockRepository) {
    Stock stock(6, 100, 100, "t-bank");
    stock_repository_->CreateStock(stock);
    ASSERT_EQ(stock, stock_repository_->ReadStock(6));
}

TEST_F(RepositoryTests, CreateUserRepository) {
    User user(6, "Maga", "wrestling");
    user_repository_->CreateUser(user);
    ASSERT_EQ(user, user_repository_->ReadUser(6));
}

TEST_F(RepositoryTests, FindExistingStockRepository) {
    ASSERT_TRUE(stock_repository_->FindStock(3));
}

TEST_F(RepositoryTests, FindNotExistingStockRepository) {
    ASSERT_FALSE(stock_repository_->FindStock(7));
}

TEST_F(RepositoryTests, FindExistingUserRepository) {
    ASSERT_TRUE(user_repository_->FindUser(3));
}

TEST_F(RepositoryTests, FindNotExistingUserRepository) {
    ASSERT_FALSE(user_repository_->FindUser(7));
}

TEST_F(RepositoryTests, UpdateStockRepository) {
    stocks_[1].cost_ = 120;
    stock_repository_->UpdateStock(stocks_[1]);
    ASSERT_EQ(stocks_[1], stock_repository_->ReadStock(stocks_[1].id_));
}

TEST_F(RepositoryTests, UpdateUserRepository) {
    users_[1].password_ = "abc";
    user_repository_->UpdateUser(users_[1]);
    ASSERT_EQ(users_[1], user_repository_->ReadUser(users_[1].id_));
}

TEST_F(RepositoryTests, DeleteStockRepository) {
    stock_repository_->DeleteStock(2);
    ASSERT_FALSE(stock_repository_->FindStock(2));
}

TEST_F(RepositoryTests, DeleteUserRepository) {
    user_repository_->DeleteUser(2);
    ASSERT_FALSE(user_repository_->FindUser(2));
}