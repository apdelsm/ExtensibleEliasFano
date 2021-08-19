/*
THIS FILE WAS EDITED BY ABDEL SANDOVAL
*/
//source: https://github.com/mateuszlewko/ads
#pragma once
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <sdsl/bit_vectors.hpp>
#include "x-fast.h"
template <typename T>
class y_fast_nur {
  const uint32_t u_exp;
  x_fast<T> top;
  std::unordered_map<T, std::map<T, void*>> bottom;
  public:
    y_fast_nur(uint32_t _u_exp)
        : u_exp(_u_exp), top(u_exp) {}
    void insert(T key, void *value) {
      auto repr = top.pred(key);
      if (!bottom.count(repr) || bottom[repr].size() > u_exp) {
        top.insert(key);
        repr = key;
      }
      bottom[repr].insert(std::make_pair(key, value));
    }

    void push(std::tuple<uint32_t, uint32_t, sdsl::sd_vector<>>* element, uint32_t bufferSize) {
      this -> insert(std::get<1>(*element), (void*)element);
    }

    void * getPredecessor(T key, uint32_t bufferSize) {
      T prev = top.pred(key + 1);
      if (bottom.count(prev)) {
        auto b = bottom[prev];
        auto it = b.lower_bound(key);
        if (it == b.begin() && it->first > key) {
          return nullptr;
        }
        if (it == b.end() || it->first > key) {
          --it;
        }
        return it->second; 
      }
      return nullptr;
    }
    uint32_t eefsize() {
      uint32_t returnSize = 0;
      returnSize += top.eefsize();
      returnSize += sizeof(u_exp);
      returnSize += sizeof(std::unordered_map<T, std::map<T, void*>>);
      returnSize += bottom.size() * (sizeof(T) + sizeof(std::map<T, void*>));
      for (auto un_map_it = bottom.begin(), un_map_end = bottom.end(); un_map_it != un_map_end; ++un_map_it) {
        returnSize += (un_map_it->second).size() * (sizeof(T) + sizeof(void*));
      }
      return returnSize;
    }
};