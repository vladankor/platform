// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <string>
#include <type_traits>
// boost
#include <boost/utility/string_view.hpp>

namespace platform {

template<class OBJECT>
struct toString {
  std::string get(const OBJECT& object) const {
    static_assert(std::is_same<OBJECT, OBJECT>::value, "toString template not realized for this type!");
  }
};

template<>
struct toString<std::string> {
  std::string get(const std::string& object) const {
    return object;
  }
};

template<class OBJECT>
struct toStringView {
  static boost::string_view get(const OBJECT& object) {
    static_assert(std::is_same<OBJECT, OBJECT>::value, "toStringView template not realized for this type!");
  }
};

template<>
struct toStringView<boost::string_view> {
  static boost::string_view get(const boost::string_view object) {
    return object;
  }
};

}  // namespace platform

#ifdef P_TO_STRING
  static_assert("P_TO_STRING already defined!");
#else
  #define P_TO_STRING(CLASS_NAME, MESSAGE) \
    template<> struct toString<CLASS_NAME> { \
      std::string operator()(const CLASS_NAME& object) const { \
        return std::string((MESSAGE)); \
      } \
    };
#endif
