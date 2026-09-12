#pragma once

#include <unordered_map>
#include <unordered_set>

namespace owlet {
template <class Key, class Value>
using hashmap = std::unordered_map<Key, Value>;

template <class T>
using hashset = std::unordered_set<T>;

}    // namespace owlet
