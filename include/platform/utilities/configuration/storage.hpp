// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <map>
#include <string>
// platform
#include <platform/singleton.hpp>
#include <platform/utilities/configuration/single_storage.hpp>

namespace platform {
namespace configuration {

P_PSINGLETON_B(Storage)
 public:
    ~Storage();

    bool initialize();

    const SingleStorage& getStorage(const std::string& config_name) const;

 private:
    std::map<std::string, SingleStorage> m_storages;
                 boost::filesystem::path m_configurationFolderPath;

    Storage();
P_PSINGLETON_E(Storage)

}  // namespace configuration
}  // namespace platform
