// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

#include <functional>

namespace platform {

class Scope {
 public:
    explicit Scope(std::function<void()>&& on_destruct);
    ~Scope();

 private:
    std::function<void()> m_onDestruct;
};  // class Scope

}  // namespace platform
