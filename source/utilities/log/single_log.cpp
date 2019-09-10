// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// platform
#include <platform/utilities/log/single_log.hpp>
#include <platform/utilities/log/exception.hpp>
#include <platform/scope.hpp>

namespace platform {
namespace log {

//
// SingleLog
//

// public:

SingleLog::SingleLog(SingleLog&& other)
  : m_logFilePath{std::move(other.m_logFilePath)}
  , m_logFile{std::move(other.m_logFile)}
  , m_writeLock{}
  , m_owner{true} {
  other.m_owner = false;
}

SingleLog& SingleLog::operator=(SingleLog&& rhs) {
  if (this == &rhs)
    return *this;
  m_logFilePath = std::move(rhs.m_logFilePath);
  m_logFile = std::move(rhs.m_logFile);
  m_owner = true;
  rhs.m_owner = false;
}

SingleLog::SingleLog(const boost::filesystem::path& log_file_path)
  : m_logFilePath{log_file_path}
  , m_owner{true} {
}

SingleLog::SingleLog(boost::filesystem::path&& log_file_path)
  : m_logFilePath{std::move(log_file_path)}
  , m_owner{true} {
}

SingleLog::~SingleLog() {
}

void SingleLog::add(const std::string& message) {
  if (!m_owner)
    return;
  std::lock_guard<decltype(m_writeLock)> lg_(m_writeLock);
  m_logFile.open(m_logFilePath.c_str(), std::ios::out|std::ios::app);
  Scope sc_([this](){ this->m_logFile.close(); });
  if (!m_logFile.good())
    throw LogWriteError{std::string("Unable write data to log file ").append(m_logFilePath.string())};
  m_logFile << message << std::endl;
}

// private:

}  // namespace log
}  // namespace platform
