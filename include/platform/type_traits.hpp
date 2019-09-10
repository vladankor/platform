// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <string_view>

namespace platform {

template<class TObject>
struct TypeName {
  static std::string_view get() {
    static_assert(std::is_same<TObject, TObject>::value, "TypeName template not realized for this type!");
  }
};

}
