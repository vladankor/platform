// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// std
#include <utility>
#include <sstream>
#include <iomanip>
// boost
#include <boost/filesystem.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/date_time.hpp>
// platform
#include <platform/utilities/log/log.hpp>
#include <platform/utilities/log/exception.hpp>
#include <platform/utilities.hpp>

namespace platform {
namespace log {

constexpr int LEVEL_IO_WIDTH = 13;

//
// Log
//

const std::string_view Log::LOG_FOLDER = "log";
const std::string_view Log::LOG_EXT = ".log";

// public:

Log::~Log() {
  // Данное действие обеспечит безопасное уничтожение лога, даже если в данный
  // кто-то производит запись в лог
  std::lock_guard<decltype(m_logsLock)> lc_(m_logsLock);
  m_logs.clear();
}

// private:

Log::Log()
  : m_logFolderPath{boost::filesystem::current_path().append(LOG_FOLDER)} {
  addLog<PlatformLog>();
}

std::string Log::generateLogMessage(const std::string& message, const level log_level) {
  std::stringstream formatted_message;
  boost::posix_time::ptime current_time = boost::posix_time::microsec_clock::local_time();
  formatted_message << std::setw(LEVEL_IO_WIDTH) << toStringView(log_level)
                    << " [" << current_time.date().year() << '.'
                    << std::setfill('0') << std::setw(2) << current_time.date().month().as_number() << '.'
                    << std::setfill('0') << std::setw(2) << current_time.date().day() << ' '
                    << std::setfill('0') << std::setw(2) << current_time.time_of_day().hours() << ':'
                    << std::setfill('0') << std::setw(2) << current_time.time_of_day().minutes() << ':'
                    << std::setfill('0') << std::setw(2) << current_time.time_of_day().seconds() << '.'
                    << std::setfill('0') << std::setw(3) << current_time.time_of_day().total_milliseconds() << "] "
                    << message;
  return formatted_message.str();
}

void Log::checkOrCreateLogFolder() {
  if (!boost::filesystem::exists(m_logFolderPath))
    boost::filesystem::create_directory(m_logFolderPath);
  if (!boost::filesystem::is_directory(m_logFolderPath))
    throw LogFolderIsNotDirectoryError{std::string{__FUNCTION__}
                                       .append(": ")
                                       .append(m_logFolderPath.string())
                                       .append(" isn't a directory!")};
}

}  // namespace log
}  // namespace platform
