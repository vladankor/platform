// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// platform
#include <platform/exception.hpp>

namespace platform {
namespace log {

struct LOG_WRITE_ERROR_TAG {};
struct LOG_FOLDER_IS_NOT_DIRECTPORY_ERROR{};

using LogWriteError = Exception<LOG_WRITE_ERROR_TAG>;
using LogFolderIsNotDirectoryError = Exception<LOG_FOLDER_IS_NOT_DIRECTPORY_ERROR>;

}  // namespace log
}  // namespace platform
