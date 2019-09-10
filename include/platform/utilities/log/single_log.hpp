// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <string>
#include <fstream>
#include <mutex>
// boost
#include <boost/filesystem.hpp>

namespace platform {
namespace log {

class SingleLog {
 public:
  SingleLog() = delete;
  SingleLog(const SingleLog& other) = delete;
  SingleLog(SingleLog&& other);

  SingleLog& operator=(const SingleLog& rhs) = delete;
  SingleLog& operator=(SingleLog&& rhs);

  explicit SingleLog(const boost::filesystem::path& log_file_path);
  explicit SingleLog(boost::filesystem::path&& log_file_path);
  ~SingleLog();

  void add(const std::string& message);

 private:
  boost::filesystem::path m_logFilePath;
            std::ofstream m_logFile;
               std::mutex m_writeLock;
                     bool m_owner = true;
};  // class SingleLog

}  // namespace log
}  // namespace platform
