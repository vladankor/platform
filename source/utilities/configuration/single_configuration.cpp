// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// platform
#include <platform/utilities/configuration/single_configuration.hpp>
#include <platform/utilities/configuration/exception.hpp>
#include <platform/utilities/configuration/reader.hpp>

namespace platform {
namespace configuration {

namespace b_fs = ::boost::filesystem;

//
// SingleConfiguration
//

// public:

SingleConfiguration::SingleConfiguration(b_fs::path&& configuration_file_path)
  : m_configurationFilePath{std::move(configuration_file_path)} {
  readConfiguration();
  if (!checkInternalState())
    throw ConfigReadError{};
}

// private:

bool SingleConfiguration::checkInternalState() const noexcept {
  return true;
}

void SingleConfiguration::readConfiguration() {
  m_internalStorage = Reader<SingleConfiguration>::readConfiguration(m_configurationFilePath);
}

}  // namespace configuration
}  // namespace platform
