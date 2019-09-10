// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <string>
#include <utility>
#include <string_view>
#include <memory>
#include <optional>
#include <iostream>
// boost
#include <boost/property_tree/ptree.hpp>
// platform
#include <platform/singleton.hpp>
#include <platform/type_traits.hpp>
#include <platform/containers/type_map.hpp>
#include <platform/utilities/configuration/single_configuration.hpp>

#ifdef PLATFORM_REGISTER_CONFIGURATION
  static_assert(false, "PLATFORM_REGISTER_CONFIGURATION already defined!");
#elif defined PLATFORM_TURN_ON_CONFIGURATION
  static_assert(false, "PLATFORM_TURN_ON_CONFIGURATION already defined!");
#else
  #define PLATFORM_REGISTER_CONFIGURATION(CONFIG_TAG) \
    struct CONFIG_TAG {}; \
    namespace platform { \
      template<> \
      struct TypeName<CONFIG_TAG> { \
        static std::string_view get() { \
          return #CONFIG_TAG; \
        } \
      }; \
    }
  #define PLATFORM_TURN_ON_CONFIGURATION(CONFIG_TAG) \
    platform::configuration::Configuration::instance()->addConfiguration<CONFIG_TAG>();
#endif  // PLATFORM_REGISTER_CONFIGURATION, PLATFORM_TURN_ON_CONFIGURATION

namespace platform {
namespace configuration {

PLATFORM_PSINGLETON_B(Configuration)
  static const std::string_view CFG_FOLDER;
  static const std::string_view CFG_EXT;

 public:
  ~Configuration();

  template<class TConfigTag>
  void addConfiguration() noexcept {
    callAndHandleErrorsVoid<std::function<void()>>([this]() {
      checkOrCreateConfigurationFolder();
      std::lock_guard<decltype(m_cfgsLock)> lg_(m_cfgsLock);
      if (auto cfg_it = m_cfgs.find<TConfigTag>(); cfg_it != m_cfgs.end()) {
        return;
      } else {
        auto cfg_file_name = std::string{TypeName<TConfigTag>::get()}.append(CFG_EXT.data());
        auto cfg_file_path = boost::filesystem::path{m_cfgFolderPath}.append(cfg_file_name);
        m_cfgs.insert<TConfigTag>(std::make_shared<SingleConfiguration>(std::move(cfg_file_path)));
      }
    });
  }

  template<class TConfigTag, class TResult>
  std::optional<TResult> getValue(const boost::property_tree::path& value_path) const noexcept {
    return callAndHandleErrors<std::optional<TResult>, std::function<std::optional<TResult>()>>(
      [this, &value_path]() -> std::optional<TResult> {
      checkOrCreateConfigurationFolder();
      std::shared_ptr<SingleConfiguration> current_cfg{nullptr};
      {
        std::lock_guard<decltype(m_cfgsLock)> lc_(m_cfgsLock);
        if (auto cfg_it = m_cfgs.find<TConfigTag>(); cfg_it != m_cfgs.end()) {
          current_cfg = cfg_it->second;
        }
      }

      if (!current_cfg) {
        return std::nullopt;
      } else {
        return {current_cfg->getValue<TResult>(value_path)};
      }
    });
  }

 private:
  containers::TypeMap<std::shared_ptr<SingleConfiguration>> m_cfgs; /**< Хранилище конфигураций */
                                    boost::filesystem::path m_cfgFolderPath; /**< Путь к папке с конфигурациями */
                                         mutable std::mutex m_cfgsLock; /**< Обеспечение безопасности хранилища */

  Configuration();

  void checkOrCreateConfigurationFolder() const;
  template<class TResult, class TFunction>
  TResult callAndHandleErrors(const TFunction& function) const {
    try {
      return function();
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
    } catch (...) {
      std::cout << "Unknown error in configuration system!" << std::endl;
    }
    return TResult{};
  }
  template<class TFunction>
  void callAndHandleErrorsVoid(const TFunction& function) const {
    try {
      function();
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
    } catch (...) {
      std::cout << "Unknown error in configuration system!" << std::endl;
    }
  }

PLATFORM_PSINGLETON_E(Configuration)  // class Configuration

}  // namespace configuration
}  // namespace platform
