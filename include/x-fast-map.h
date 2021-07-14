/*
THIS FILE WAS EDITED BY ABDEL SANDOVAL
*/
//source: https://github.com/mateuszlewko/ads
#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <sdsl/bit_vectors.hpp>


template <typename T>
struct x_fast_node {
  x_fast_node<T> *prev, *next;
  T key;
  bool leaf;
  void *value;
  x_fast_node(T k = 0, void *v = nullptr, bool _leaf = false) {
    key = k;
    value = v;
    leaf = _leaf;
    prev = next = NULL;
  }
};

template <typename T>
class x_fast_map {
  const uint64_t u_exp;

  typedef std::unordered_map<T, x_fast_node<T>*> level;

  std::vector<level> levels;

  int lcp_level(T key) {
    int beg = 0;
    int end = u_exp + 1;

    while (beg != end - 1) {
      int mid = (beg + end) / 2;
      T prefix = key >> (u_exp - mid);

      if (levels[mid].count(prefix))
        beg = mid;
      else
        end = mid;
    }

    return beg;
  }

  x_fast_node<T>* pred_node(T key) {
    auto node = find_leaf_node(key);
    if (node && node->key >= key) return node->prev;

    return node;
  }

  x_fast_node<T>* succ_node(T key) {
    auto node = find_leaf_node(key);
    if (node && node->key <= key) return node->next;

    return node;
  }

  x_fast_node<T>* find_leaf_node(T key) {
    auto lvl = lcp_level(key);
    
    auto node = levels[lvl][key >> (u_exp - lvl)];
    if (!node) return nullptr;

    if (!node->leaf) {
      if (!node->prev && !node->next) return nullptr;

      bool right_child = key & ((T)1 << (u_exp - lvl - 1));
      node = right_child ? node->next : node->prev;
    }

    return node;
  }

  public:
    x_fast_map(uint64_t _u_exp) : u_exp(_u_exp) {
      levels.resize(u_exp + 1);
      levels[0][0] = new x_fast_node<T>();
    }

    bool lookup(T key) {
      auto& leafs = levels[u_exp];
      return leafs.count(key) != 0;
    }

    void insert(T key, void *value) {
      if (lookup(key)) return;

      auto prev = pred_node(key);
      auto next = succ_node(key);

      auto curr = new x_fast_node<T>(key, value, true);
      levels[u_exp][key] = curr;

      if (prev) {
        curr->prev = prev;
        prev->next = curr;
      }

      if (next) {
        curr->next = next;
        next->prev = curr;
      }

      for (uint64_t i = 1; i <= u_exp; i++) {
        auto prefix = key >> (u_exp - i);

        auto parent = levels[i - 1][prefix >> 1];
        if (!levels[i].count(prefix)) {
          auto lvl_node = new x_fast_node<T>(key);
          levels[i][prefix] = lvl_node;

          if (prefix & 1) {
            parent->next = lvl_node;
          } else {
            parent->prev = lvl_node;
          }
        }

        if (prefix & 1) {
          // update left descendant pointer
          if (!parent->prev || parent->prev->key > key) {
            parent->prev = curr;
          }
        } else {
          // update right descendant pointer
          if (!parent->next || parent->next->key < key) {
            parent->next = curr;
          }
        }
      }
    }

    T succ(T key) {
      auto node = succ_node(key);
      return node ? node->key : ((T)1 << u_exp);
    }

    T pred(T key) {
      auto node = pred_node(key);
      return node ? node->key : -((T)1 << u_exp);
    }

    void * getPredecessor(T key, uint64_t bufferSize) {
      auto node = pred_node(key/bufferSize + 1);
      if (node && std::get<1>(*(std::tuple<uint64_t, uint64_t, sdsl::sd_vector<>> *)(node->value)) <= key) {
        return node->value;
      } else if(node->prev) {
        return node->prev->value;
      }
      return nullptr;
    }

    void push(std::tuple<uint64_t, uint64_t, sdsl::sd_vector<>> *element, uint64_t bufferSize) {
      insert(std::get<1>(*element)/bufferSize, (void*)element);
    }

    uint64_t eefsize() {
      uint64_t returnSize = 0;
      returnSize += sizeof(u_exp);
      returnSize += sizeof(std::vector<level>);
      returnSize += sizeof(level) * (u_exp + 1);
      for (auto it = levels.begin(), end = levels.end(); it != end; ++it) {
        returnSize += (*it).size() * (sizeof(T) + sizeof(x_fast_node<T>*) + sizeof(x_fast_node<T>));
      }
      return returnSize;
    }

    ~x_fast_map() {
      for (auto it = levels.begin(), end = levels.end(); it != end; ++it) {
        for (auto un_map_it = (*it).begin(), un_map_end = (*it).end(); un_map_it != un_map_end; ++un_map_it) {
          delete un_map_it->second;
        }
      }
    }
};