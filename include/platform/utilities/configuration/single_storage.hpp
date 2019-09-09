// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// boost
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

namespace platform {
namespace configuration {

class SingleStorage {
 public:
    explicit SingleStorage(boost::filesystem::path&& configuration_file_path);

    template<typename VALUE_TYPE>
    VALUE_TYPE getValue(const boost::property_tree::path& field_name) const {
      return m_internalStorage.get<VALUE_TYPE>(field_name);
    }

 private:
    boost::property_tree::ptree m_internalStorage;
        boost::filesystem::path m_configurationFilePath;

    virtual bool checkInternalState() const noexcept;
    virtual void readConfiguration();
};  // class SingleStorage

}  // namespace configuration
}  // namespace platform
