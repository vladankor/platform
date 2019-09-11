// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <string>
#include <utility>
// platform
#include <platform/utilities.hpp>

namespace platform {

template<class TExceptionTag>
class Exception : public std::exception {
 public:
  Exception() = default;
  explicit Exception(const Exception<TExceptionTag>& other) = default;
  explicit Exception(Exception<TExceptionTag>&& other) = default;

  Exception<TExceptionTag>& operator=(const Exception<TExceptionTag>& other) = default;
  Exception<TExceptionTag>& operator=(Exception<TExceptionTag>&& other) = default;

  explicit Exception(const std::string& data)
    : m_data{data} {
  }
  explicit Exception(std::string&& data)
    : m_data{std::move(data)} {
  }

  const char* what() const noexcept override {
    return m_data.c_str();
  }

 private:
  std::string m_data;
};

}  // namespace platform
