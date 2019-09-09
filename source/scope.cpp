// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// platform
#include <platform/scope.hpp>

namespace platform {

//
// class Scope
//

Scope::Scope(std::function<void()>&& on_destruct)
  : m_onDestruct(std::move(on_destruct)) {
}

Scope::~Scope() {
  m_onDestruct();
}

}  // namespace platform
