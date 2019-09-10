// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

#include <atomic>

namespace platform {

class SpinLock {
 public:
  void lock();
  bool try_lock();
  void unlock();

 private:
  std::atomic_bool m_lock = false;
};  // class SpinLock

}  // namespace platform
