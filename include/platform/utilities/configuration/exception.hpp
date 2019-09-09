// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <string>
// platform
#include <platform/exception.hpp>
#include <platform/utilities.hpp>

namespace platform {
namespace configuration {

struct CONFIG_READ_ERROR_TAG {};
struct CONFIG_NOT_FOUND_ERROR_TAG {};

using ConfigReadError = Exception<CONFIG_READ_ERROR_TAG>;
using ConfigNotFoundError = Exception<CONFIG_NOT_FOUND_ERROR_TAG>;

}  // namespace configuration
}  // namespace platform
