// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// platform
#include <platform/exception.hpp>

namespace platform {
namespace log {

struct LOG_WRITE_ERROR_TAG {};
struct LOG_NOT_FOUND_ERROR_TAG {};
struct LOG_READ_ERROR_TAG {};

using LogWriteError = Exception<LOG_WRITE_ERROR_TAG>;
using LogNotFoundError = Exception<LOG_NOT_FOUND_ERROR_TAG>;
using LogReadError = Exception<LOG_READ_ERROR_TAG>;

}  // namespace log
}  // namespace platform
