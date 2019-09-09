// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// std
#include <utility>
// boost
#include <boost/filesystem.hpp>
#include <boost/utility/string_view.hpp>
// platform
#include <platform/utilities/configuration/storage.hpp>
#include <platform/utilities/configuration/exception.hpp>
#include <platform/utilities/configuration/reader.hpp>
#include <platform/utilities.hpp>

namespace platform {
namespace configuration {

static const boost::string_view CFG_FOLDER = "/configuration";
static const boost::string_view CFG_EXT = ".config";

//
// Storage
//

// public:

Storage::~Storage() {
}

bool Storage::initialize() {
  namespace b_fs = boost::filesystem;
  try {
    if (!b_fs::exists(m_configurationFolderPath) || !b_fs::is_directory(m_configurationFolderPath))
      return false;
    for (b_fs::directory_entry& configuration_file : b_fs::directory_iterator(m_configurationFolderPath)) {
      if (!b_fs::is_regular_file(configuration_file))
        continue;
      m_storages.emplace(std::make_pair(configuration_file.path().stem().string()
                                        , SingleStorage{b_fs::path{configuration_file.path()}}));
    }
  } catch(const b_fs::filesystem_error& ex) {
    throw ConfigReadError(std::string{"Configuration read error with message:"}.append(ex.what()));
  }
  return true;
}

const SingleStorage& Storage::getStorage(const std::string& config_name) const {
  if (auto search_it = m_storages.find(config_name); search_it == m_storages.end()) {
    throw ConfigNotFoundError(std::string{"Configuration not found:"}.append(config_name));
  } else {
    return search_it->second;
  }
}

// private:

Storage::Storage()
  : m_configurationFolderPath{boost::filesystem::current_path().append(CFG_FOLDER)} {
}

}  // namespace configuration
}  // namespace platform

