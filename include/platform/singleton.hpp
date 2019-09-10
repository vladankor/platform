// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

#include <memory>
#include <mutex>
#include <platform/spin_lock.hpp>

namespace platform {

template<class Object>
class PSingleton {
 public:
  ~PSingleton() {
  }

  static std::shared_ptr<Object>& instance() {
    static std::shared_ptr<Object> m_object{nullptr};
    if (!m_object) {
      static platform::SpinLock m_lock;
      if (std::lock_guard<decltype(m_lock)> lg_(m_lock); !m_object)
        m_object = Object::create();
    }
    return m_object;
  }
 protected:
  struct EMPTY_TAG {};

  PSingleton() = default;
  PSingleton(const PSingleton<Object>& other) = delete;
  PSingleton(PSingleton<Object>&& other) = delete;

  PSingleton<Object>& operator=(const PSingleton<Object>& rhs) = delete;
  PSingleton<Object>& operator=(PSingleton<Object>&& rhs) = delete;
};  // class PSingleton

}  // namespace platform

#ifdef PLATFORM_PSINGLETON_B
  static_assert(false, "PLATFORM_PSINGLETON_B already defined!");
#elif defined PLATFORM_PSINGLETON_E
  static_assert(false, "PLATFORM_PSINGLETON_E already defined!");
#else
  #define PLATFORM_PSINGLETON_B(CLASS_NAME) \
    class CLASS_NAME : public platform::PSingleton<CLASS_NAME> { \
      friend class platform::PSingleton<CLASS_NAME>; \
     private: \
        CLASS_NAME(const CLASS_NAME& other) = delete; \
        CLASS_NAME(CLASS_NAME&& other) = delete; \
        CLASS_NAME& operator=(const CLASS_NAME& rhs) = delete; \
        CLASS_NAME& operator=(CLASS_NAME&& rhs) = delete; \
     public: \
        explicit CLASS_NAME(platform::PSingleton<CLASS_NAME>::EMPTY_TAG) : CLASS_NAME{} {} \
        static std::shared_ptr<CLASS_NAME> create() { \
          return std::make_shared<CLASS_NAME>(platform::PSingleton<CLASS_NAME>::EMPTY_TAG{}); \
        } \
     private:
  #define PLATFORM_PSINGLETON_E(CLASS_NAME) };
#endif  // PLATFORM_PSINGLETON_B, PLATFORM_PSINGLETON_E
