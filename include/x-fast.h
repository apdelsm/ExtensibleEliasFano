/*
THIS FILE WAS EDITED BY ABDEL SANDOVAL
*/
//source: https://github.com/mateuszlewko/ads
#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

template <typename T>
struct xnode {
  xnode<T> *prev, *next;
  T key;
  bool leaf;

  xnode(T k = 0, bool _leaf = false) : key(k), leaf(_leaf) {
    prev = next = NULL;
  }
};

template <typename T>
class x_fast {
  const int u_exp;

  typedef std::unordered_map<T, xnode<T>*> level;

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

  xnode<T>* pred_node(T key) {
    auto node = find_leaf_node(key);
    if (node && node->key >= key) return node->prev;

    return node;
  }

  xnode<T>* succ_node(T key) {
    auto node = find_leaf_node(key);
    if (node && node->key <= key) return node->next;

    return node;
  }

  xnode<T>* find_leaf_node(T key) {
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
  x_fast(int _u_exp) : u_exp(_u_exp) {
    levels.resize(u_exp + 1);
    levels[0][0] = new xnode<T>();
  }

  bool lookup(T key) {
    auto& leafs = levels[u_exp];
    return leafs.count(key) != 0;
  }

  void insert(T key) {
    if (lookup(key)) return;

    auto prev = pred_node(key);
    auto next = succ_node(key);

    auto curr = new xnode<T>(key, true);
    levels[u_exp][key] = curr;

    if (prev) {
      curr->prev = prev;
      prev->next = curr;
    }

    if (next) {
      curr->next = next;
      next->prev = curr;
    }

    for (int i = 1; i <= u_exp; i++) {
      auto prefix = key >> (u_exp - i);

      auto parent = levels[i - 1][prefix >> 1];
      if (!levels[i].count(prefix)) {
        auto lvl_node = new xnode<T>(key);
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

  uint32_t eefsize() {
    uint32_t returnSize = 0;
    returnSize += sizeof(u_exp);
    returnSize += sizeof(std::vector<level>);
    returnSize += sizeof(level) * (u_exp + 1);
    for (auto it = levels.begin(), end = levels.end(); it != end; ++it) {
      returnSize += (*it).size() * (sizeof(T) + sizeof(xnode<T>*) + sizeof(xnode<T>));
    }
    return returnSize;
  }

  ~x_fast() {
    for (auto it = levels.begin(), end = levels.end(); it != end; ++it) {
      for (auto un_map_it = (*it).begin(), un_map_end = (*it).end(); un_map_it != un_map_end; ++un_map_it) {
        delete un_map_it->second;
      }
    }
  }
};