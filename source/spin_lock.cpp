// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------

// platform
#include <platform/spin_lock.hpp>

namespace platform {

//
// class SpinLock
//

// public:

void SpinLock::lock() {
  bool value = false;
  while (!m_lock.compare_exchange_weak(value, true, std::memory_order_acquire)) {
    value = false;
  }
}

bool SpinLock::try_lock() {
  bool value = false;
  return m_lock.compare_exchange_weak( value, true, std::memory_order_acquire );
}

void SpinLock::unlock() {
  m_lock.store(false , std::memory_order_release);
}

}  // namespace platform
