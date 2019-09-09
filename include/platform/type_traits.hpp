// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// platform
#include <boost/utility/string_view.hpp>

namespace platform {

template<class TObject>
struct TypeName {
  static boost::string_view get() {
    static_assert(std::is_same<TObject, TObject>::value, "TypeName template not realized for this type!");
  }
};

}
