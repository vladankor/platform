// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// std
#include <utility>
#include <string_view>
// boost
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
// platform
#include <platform/utilities/configuration/configuration.hpp>
#include <platform/utilities/configuration/exception.hpp>
#include <platform/utilities/configuration/reader.hpp>
#include <platform/utilities.hpp>

namespace platform {
namespace configuration {

namespace b_fs = ::boost::filesystem;

//
// Configuration
//

const std::string_view Configuration::CFG_FOLDER = "/configuration";
const std::string_view Configuration::CFG_EXT = ".config";

// public:

Configuration::~Configuration() {
}

// private:

Configuration::Configuration()
  : m_cfgFolderPath{b_fs::current_path().append(CFG_FOLDER)} {
}

void Configuration::checkOrCreateConfigurationFolder() const {
  if (!b_fs::exists(m_cfgFolderPath))
    b_fs::create_directory(m_cfgFolderPath);
  if (!b_fs::is_directory(m_cfgFolderPath))
    throw ConfigFolderIsNotDirectoryError{std::string{m_cfgFolderPath.string()}
                                          .append(" isn't a directory!")};
}

}  // namespace configuration
}  // namespace platform

