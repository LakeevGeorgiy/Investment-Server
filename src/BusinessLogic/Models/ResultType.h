#pragma once

#include <memory>
#include <variant>

template<typename T>
class ResultType {
public:

    using success_type = T;
    using failed_type = std::shared_ptr<std::exception>;

private:

    std::variant<success_type, failed_type> result_;

public:

    ResultType() = default;
    ResultType(success_type& value): result_(value) {}
    ResultType(failed_type error): result_(error) {}

    bool IsSuccess() {
        return std::get_if<success_type>(&result_) != nullptr;
    }

    success_type GetResult() {
        return std::get<success_type>(result_);
    }

    failed_type GetError() {
        return std::get<failed_type>(result_);
    }
};

template<>
class ResultType<void> {
public:

    using success_type = bool;
    using failed_type = std::shared_ptr<std::exception>;

private:

    std::variant<success_type, failed_type> result_;

public:

    ResultType(): result_(false) {}
    ResultType(failed_type error): result_(error) {}

    bool IsSuccess() {
        return std::get_if<success_type>(&result_) != nullptr;
    }

    success_type GetResult() {
        return std::get<success_type>(result_);
    }

    failed_type GetError() {
        return std::get<failed_type>(result_);
    }
};