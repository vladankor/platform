// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <string>
#include <utility>
// boost
#include <boost/utility/string_view.hpp>
// platform
#include <platform/singleton.hpp>
#include <platform/utilities/log/single_log.hpp>
#include <platform/container/type_map.hpp>
#include <platform/type_traits.hpp>

#ifdef P_REGISTER_LOG
  static_assert(false, "P_REGISTER_LOG already defined!");
#else
  #define P_REGISTER_LOG(LOG_TAG) \
    struct LOG_TAG {}; \
    namespace platform { \
      template<> \
      struct TypeName<LOG_TAG> { \
        static boost::string_view get() { \
          return #LOG_TAG; \
        } \
      }; \
    }
  #define P_TURN_ON_LOG(LOG_TAG) \
    platform::log::Log::instance()->addLog<LOG_TAG>();
#endif  // P_REGISTER_LOG

P_REGISTER_LOG(PlatformLog)

namespace platform {
namespace log {

P_PSINGLETON_B(Log)
  enum class level {
    fatal,
    error,
    warning,
    information,
    debug
  };  // enum class level

  boost::string_view toStringView(const level log_level) {
    switch (log_level) {
      case level::fatal: return "[FATAL]";
      case level::error: return "[ERROR]";
      case level::warning: return "[WARNING]";
      case level::information: return "[INFORMATION]";
      case level::debug: return "[DEBUG]";
      default : return "[UNKNOWN]";
    }
  }

  static const boost::string_view LOG_FOLDER;
  static const boost::string_view LOG_EXT;

 public:
    ~Log();

    template<class TLogTag>
    void addLog() {
      if (auto log_it = m_logs.find<TLogTag>(); log_it != m_logs.end()) {
        return;
      } else {
        auto log_file_path = boost::filesystem::path{m_logFolderPath}
                             .append(std::string{TypeName<TLogTag>::get()}
                                     .append(LOG_EXT.data()));
        auto new_log = SingleLog{log_file_path};
        m_logs.insert<TLogTag>(std::move(new_log));
      }
    }

    template<class TLogTag>
    inline bool addFatal(const std::string& message) noexcept {
      return add<TLogTag>(message, level::fatal);
    }
    template<class TLogTag>
    inline bool addError(const std::string& message) noexcept {
      return add<TLogTag>(message, level::error);
    }
    template<class TLogTag>
    inline bool addWarning(const std::string& message) noexcept {
      return add<TLogTag>(message, level::warning);
    }
    template<class TLogTag>
    inline bool addInformation(const std::string& message) noexcept {
      return add<TLogTag>(message, level::information);
    }
    template<class TLogTag>
    inline bool addDebug(const std::string& message) noexcept {
      return add<TLogTag>(message, level::debug);
    }

 private:
       container::TypeMap<SingleLog> m_logs;
             boost::filesystem::path m_logFolderPath;

    Log();

    template<class TLogTag>
    bool add(const std::string& message, const level log_level) noexcept {
      if (auto log_it = m_logs.find<TLogTag>(); log_it == m_logs.end()) {
        // if the log isn't found, add message to platform log
        add<PlatformLog>(message, log_level);
        return false;
      } else {
        log_it->second.add(generateLogMessage(message, log_level));
        return true;
      }
    }

    std::string generateLogMessage(const std::string& message, const level log_level);
P_PSINGLETON_E(Log)  // class Log

}  // namespace log
}  // namespace platform
