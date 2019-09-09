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

template<class TAG>
class Exception : public std::exception {
 public:
  Exception() = default;
  explicit Exception(const Exception<TAG>& other) = default;
  explicit Exception(Exception<TAG>&& other) = default;

  Exception<TAG>& operator=(const Exception<TAG>& other) = default;
  Exception<TAG>& operator=(Exception<TAG>&& other) = default;

  explicit Exception(const std::string& data)
    : m_data(data) {
  }
  explicit Exception(std::string&& data)
    : m_data(std::move(data)) {
  }

  const char* what() const noexcept override {
    return m_data.c_str();
  }

 private:
  std::string m_data;
};

}  // namespace platform
