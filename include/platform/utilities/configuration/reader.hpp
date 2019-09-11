// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <iostream>
#include <utility>
#include <locale>
#include <string>
// boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
// platform
#include <platform/utilities/configuration/exception.hpp>

namespace platform {
namespace configuration {

template<class TTagName>
class Reader {
 public:
  static boost::property_tree::ptree readConfiguration(const boost::filesystem::path& config_path) {
    if (!boost::filesystem::exists(config_path))
      throw ConfigReadError{std::string{"Configuration "}.append(config_path.string()).append(" doesn't exists!")};
    boost::property_tree::ptree configuration;
    boost::property_tree::read_json(config_path.string(), configuration);

    return std::move(configuration);
  }
};  // class Reader

}  // namespace configuration
}  // namespace platform
