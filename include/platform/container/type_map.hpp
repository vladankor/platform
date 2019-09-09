// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace platform {
namespace container {

template<class TValue>
class TypeMap {
  using Storage = std::unordered_map<std::type_index, TValue>;

 public:
  using value_type = TValue;
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;
  using size_type = typename Storage::size_type;

  iterator begin() {
    return m_storage.begin();
  }
  const_iterator begin() const {
    return m_storage.begin();
  }

  iterator end() {
    return m_storage.end();
  }
  const_iterator end() const {
    return m_storage.end();
  }

  template<class TKey>
  iterator find() {
    return m_storage.find(std::type_index{typeid(TKey)});
  }
  template<class TKey>
  const_iterator find() const {
    return m_storage.find(std::type_index{typeid(TKey)});
  }

  template<class TKey>
  std::pair<iterator, bool> insert(TValue&& value) {
    return m_storage.insert(std::make_pair(std::type_index{typeid(TKey)}
                            , std::forward<TValue>(value)));
  }

  template<class TKey>
  size_type erase() {
    return m_storage.erase(std::type_index{typeid(TKey)});
  }

 private:
  Storage m_storage;
};  // class TypeMap

}  // namespace container
}  // namespace platform
