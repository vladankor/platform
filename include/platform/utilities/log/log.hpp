// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <string>
#include <utility>
#include <string_view>
#include <memory>
#include <functional>
#include <iostream>
// platform
#include <platform/singleton.hpp>
#include <platform/utilities/log/single_log.hpp>
#include <platform/containers/type_map.hpp>
#include <platform/type_traits.hpp>

#ifdef PLATFORM_REGISTER_LOG
  static_assert(false, "PLATFORM_REGISTER_LOG already defined!");
#elif defined PLATFORM_TURN_ON_LOG
  static_assert(false, "PLATFORM_TURN_ON_LOG already defined!");
#else
  #define PLATFORM_REGISTER_LOG(LOG_TAG) \
    struct LOG_TAG {}; \
    namespace platform { \
      template<> \
      struct TypeName<LOG_TAG> { \
        static std::string_view get() { \
          return #LOG_TAG; \
        } \
      }; \
    }
  #define PLATFORM_TURN_ON_LOG(LOG_TAG) \
    platform::log::Log::instance()->addLog<LOG_TAG>();
#endif  // PLATFORM_REGISTER_LOG, PLATFORM_TURN_ON_LOG

PLATFORM_REGISTER_LOG(PlatformLog)

namespace platform {
namespace log {

/**
 * \brief Глобальное хранилище для доступа к системе логирования
 * 
 * Предоставялет доступ к пользовательским логам, а также к библиотечному
 * логу PlatformLog.
 */
PLATFORM_PSINGLETON_B(Log)
  enum class level {
    fatal,
    error,
    warning,
    information,
    debug
  };  // enum class level

  std::string_view toStringView(const level log_level) {
    switch (log_level) {
      case level::fatal: return "[FATAL]";
      case level::error: return "[ERROR]";
      case level::warning: return "[WARNING]";
      case level::information: return "[INFORMATION]";
      case level::debug: return "[DEBUG]";
      default : return "[UNKNOWN]";
    }
  }

  static const std::string_view LOG_FOLDER;
  static const std::string_view LOG_EXT;

 public:
  ~Log();

  /**
   * \brief Добавить лог с меткой TLogTag
   * \param TLogTag - метка, определяющая тип лога, с которым необходимо работать
   * \details Для использования необходимо наличие специализации шаблона platform::TypeName
   *          для типа TLogTag. Для удобства можно использовать макрос PLATFORM_REGISTER_LOG,
   *          определяющий простую метку и реализацю шаблона (использовать в глобальной области),
   *          совместно с макросом PLATFORM_TURN_ON_LOG, который следует применить в начале программы.
   *          Ошибки работы системы логирования выводятся в стандартный поток вывода (т.к. подразумевается,
   *          что невозможно делать записи в логи)
   * \todo Переработать систему обработки ошибок внутри логов, чтобы иметь возможность железобетонно
   *       работать с логом библиотеки
   */
  template<class TLogTag>
  void addLog() noexcept {
    callAndHandleErrorsVoid<std::function<void()>>([this]() {
      checkOrCreateLogFolder();
      std::lock_guard<decltype(m_logsLock)> lg_(m_logsLock);
      if (auto log_it = m_logs.find<TLogTag>(); log_it != m_logs.end()) {
        return;
      } else {
        auto log_file_name = std::string{TypeName<TLogTag>::get()}.append(LOG_EXT.data());
        auto log_file_path = boost::filesystem::path{m_logFolderPath}.append(log_file_name);
        m_logs.insert<TLogTag>(std::make_shared<SingleLog>(std::move(log_file_path)));
      }
    });
  }

  /**
   * \brief Извлечь лог с меткой TLogTag
   * \param TLogTag - метка типа лога
   */
  template<class TLogTag>
  void eraseLog() noexcept {
    callAndHandleErrorsVoid<std::function<void()>>([this]() {
      std::lock_guard<decltype(m_logsLock)> lg_(m_logsLock);
      m_logs.erase<TLogTag>();
    });
  }

  /**
   * \brief Добавить сообщение в лог с меткой FATAL
   * \param TLogTag - метка типа лога
   */
  template<class TLogTag>
  inline bool addFatal(const std::string& message) noexcept {
    return add<TLogTag>(message, level::fatal);
  }
  /**
   * \brief Добавить сообщение в лог с меткой ERROR
   * \param TLogTag - метка типа лога
   */
  template<class TLogTag>
  inline bool addError(const std::string& message) noexcept {
    return add<TLogTag>(message, level::error);
  }
  /**
   * \brief Добавить сообщение в лог с меткой WARNING
   * \param TLogTag - метка типа лога
   */
  template<class TLogTag>
  inline bool addWarning(const std::string& message) noexcept {
    return add<TLogTag>(message, level::warning);
  }
  /**
   * \brief Добавить сообщение в лог с меткой INFORMATION
   * \param TLogTag - метка типа лога
   */
  template<class TLogTag>
  inline bool addInformation(const std::string& message) noexcept {
    return add<TLogTag>(message, level::information);
  }
  /**
   * \brief Добавить сообщение в лог с меткой DEBUG, если существует определение
   *        PLATFORM_DEBUG_LOG
   * \param TLogTag - метка типа лога
   */
  template<class TLogTag>
  inline bool addDebug(const std::string& message) noexcept {
    #ifdef PLATFORM_DEBUG_LOG
      return add<TLogTag>(message, level::debug);
    #endif  // PLATFORM_DEBUG_LOG
  }

 private:
  containers::TypeMap<std::shared_ptr<SingleLog>> m_logs; /**< Хранилище логов */
                          boost::filesystem::path m_logFolderPath; /**< Путь к папке с логами */
                               mutable std::mutex m_logsLock; /**< Обеспечение безопасности хранилища */

  Log();

  /**
   * \brief Добавить сообщение в лог
   * \details Добавляет к переданному сообщению метку уровня лога и текущее время
   */
  template<class TLogTag>
  bool add(const std::string& message, const level log_level) noexcept {
    return callAndHandleErrors<bool, std::function<bool()>>([this, &message, log_level]() -> bool {
      checkOrCreateLogFolder();
      std::shared_ptr<SingleLog> current_log{nullptr};
      {
        std::lock_guard<decltype(m_logsLock)> lc_(m_logsLock);
        if (auto log_it = m_logs.find<TLogTag>(); log_it != m_logs.end()) {
          current_log = log_it->second;
        }
      }

      if (!current_log) {
        // Если искомый лог не найден, добавим warning в лог платформы
        addWarning<PlatformLog>(message);
        return false;
      } else {
        current_log->add(generateLogMessage(message, log_level));
        return true;
      }
    });
  }

  /**
   * \brief Сгенерировать полное сообщение для записи в лог (добавление метки уровня логировнаия и
   *        и текущего времени)
   */
  std::string generateLogMessage(const std::string& message, const level log_level);
  /**
   * \brief Проверить наличие директории для записи логов и, при необходимости, попытаться создать её
   * \details Директория с именем log располагается рядом с исполняемым файлом
   */
  void checkOrCreateLogFolder();
  /**
   * \brief Вызвать переданную функцию и вернуть её результат с учетом обработки ошибок
   */
  template<class TResult, class TFunction>
  TResult callAndHandleErrors(const TFunction& function) const noexcept {
    try {
      return function();
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
    } catch (...) {
      std::cout << "Unknown error in log system!" << std::endl;
    }
    return TResult{};
  }
  /**
   * \brief Вызвать переданную функцию и обработать возможные ошибки
   */
  template<class TFunction>
  void callAndHandleErrorsVoid(const TFunction& function) const noexcept {
    try {
      function();
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
    } catch (...) {
      std::cout << "Unknown error in log system!" << std::endl;
    }
  }
PLATFORM_PSINGLETON_E(Log)  // class Log

}  // namespace log
}  // namespace platform
