// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// platform
#include <platform/utilities/configuration/single_storage.hpp>
#include <platform/utilities/configuration/exception.hpp>
#include <platform/utilities/configuration/reader.hpp>

namespace platform {
namespace configuration {

//
// SingleStorage
//

// public:

SingleStorage::SingleStorage(boost::filesystem::path&& configuration_file_path)
  : m_configurationFilePath{std::move(configuration_file_path)} {
  readConfiguration();
  if (!checkInternalState())
    throw ConfigReadError{};
}

// private:

bool SingleStorage::checkInternalState() const noexcept {
  return true;
}

void SingleStorage::readConfiguration() {
  m_internalStorage = Reader<SingleStorage>::readConfiguration(m_configurationFilePath);
}

}  // namespace configuration
}  // namespace platform
