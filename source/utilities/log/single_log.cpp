// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// platform
#include <platform/utilities/log/single_log.hpp>
#include <platform/utilities/log/exception.hpp>
#include <platform/scope.hpp>

namespace platform {
namespace log {

constexpr std::size_t LOG_FILE_BUFFER_SIZE = 10 * 1024;

//
// SingleLog
//

// public:

SingleLog::SingleLog(SingleLog&& other)
  : m_logFilePath{std::move(other.m_logFilePath)}
  , m_buffer{std::move(other.m_buffer)}
  , m_logFile{std::move(other.m_logFile)}
  , m_owner{true} {
  other.m_owner = false;
}

SingleLog& SingleLog::operator=(SingleLog&& rhs) {
  if (this == &rhs)
    return *this;
  m_logFilePath = std::move(rhs.m_logFilePath);
  m_buffer = std::move(rhs.m_buffer);
  m_logFile = std::move(rhs.m_logFile);
  m_owner = true;
  rhs.m_owner = false;
}

SingleLog::SingleLog(const boost::filesystem::path& log_file_path)
  : m_logFilePath{log_file_path} {
  m_buffer.reserve(LOG_FILE_BUFFER_SIZE);
}

SingleLog::SingleLog(boost::filesystem::path&& log_file_path)
  : m_logFilePath{std::move(log_file_path)} {
  m_buffer.reserve(LOG_FILE_BUFFER_SIZE);
}

SingleLog::~SingleLog() {
  if (m_owner)
    flush();
}

void SingleLog::add(const std::string& message) {
  if (!m_owner)
    return;
  // if the buffer is full, write it to the file
  if (message.length() + m_buffer.length() > LOG_FILE_BUFFER_SIZE)
    flush();
  m_buffer.append(message);
}

void SingleLog::flush() {
  m_logFile.open(m_logFilePath.c_str(), std::ios::out|std::ios::app);
  Scope sc_([this](){ this->m_logFile.close(); });
  if (!m_logFile.good())
    throw LogWriteError{std::string("Unable write data to log file ").append(m_logFilePath.string())};
  m_logFile << m_buffer << std::endl;
  m_buffer.resize(0);
}

// private:

}  // namespace log
}  // namespace platform
