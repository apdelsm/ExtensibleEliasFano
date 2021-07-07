/*
THIS FILE WAS EDITED BY ABDEL SANDOVAL
*/
//source: https://github.com/mateuszlewko/ads
#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

template <typename T>
class v_eb {
  /// expononent of universe size
  const int u_exp;
  const int u_exp_low;
  const int u_exp_high;
  const T u;

  std::vector<v_eb<T>> buckets;
  std::unique_ptr<v_eb<T>> non_empty_high;

  T min, max;
  void *minValue, *maxValue;

  T get_high(T key) { return key >> u_exp_low; }

  T get_low(T key) {
    T low_bits = ((T)1 << u_exp_low) - 1;
    return key & low_bits;
  }

 public:
  v_eb(int _u_exp)
      : u_exp(std::max(_u_exp, 1)),
        u_exp_low((u_exp + 1) / 2),
        u_exp_high(u_exp / 2),
        u(1 << u_exp),
        min(u),
        max(0) {
    if (u_exp == 1) return;

    non_empty_high = std::make_unique<v_eb<T>>(u_exp_high);
    for (int i = 0; i < (1 << u_exp_high); i++) {
      buckets.emplace_back(u_exp_low);
    }
  }

  bool lookup(T key) {
    if (key == 0) return min == key;
    return succ(key - 1) == key;
  }

  void insert(T key, void *value=nullptr) {
    if (lookup(key)) return;
    void *auxValue = value;
    if (min > max) {
      min = max = key;
      minValue = maxValue = value;
      return;
    }

    if (key < min) {
      std::swap(key, min);
      auxValue = minValue;
      minValue = value;
    }

    max = std::max(key, max);
    if (key == max) {
      maxValue = auxValue;
    }

    if (u_exp == 1) return;

    auto high = get_high(key);
    auto low = get_low(key);
    buckets[high].insert(low, auxValue);

    if (buckets[high].min == buckets[high].max) {
      non_empty_high->insert(high);
    }
  }

  T succ(T key) {
    if (key < min) return min;
    if (u == 2 && max > key) return max;
    if (key >= max || u == 2) return u;

    auto high = get_high(key);
    auto low = get_low(key);
    
    if (low < buckets[high].max) {
      return (high << u_exp_low) + buckets[high].succ(low);
    }

    auto next_high = non_empty_high->succ(high);
    return (next_high << u_exp_low) + buckets[next_high].min;
  }

  T pred(T key) {
    if (key < min) return u;
    if (u == 2 && max > key) return min;
    if (key >= max || u == 2) return max;

    auto high = get_high(key);
    auto low = get_low(key);

    if (low >= buckets[high].min) {
      return (high << u_exp_low) + buckets[high].pred(low);
    }

    auto prev_high = non_empty_high->pred(high);
    return (prev_high << u_exp_low) + buckets[prev_high].max;
  }

  void push(std::tuple<uint64_t, uint64_t, sdsl::sd_vector<>>* element) {
    this -> insert(std::get<1>(*element), (void*)element);
  }

  void * getPredecessor(T key) {
    if (key < min) return nullptr;
    if (u == 2 && max > key) return minValue;
    if (key >= max || u == 2) return maxValue;

    auto high = get_high(key);
    auto low = get_low(key);

    if (low >= buckets[high].min) {
      return buckets[high].getPredecessor(low);
    }

    auto prev_high = non_empty_high->succ(high);
    return buckets[prev_high].maxValue;
  }

  uint64_t eefsize() {
    uint64_t returnSize = 0;
    returnSize += 3 * sizeof(const int);
    returnSize += 3 * sizeof(T);
    returnSize += sizeof(std::vector<v_eb<T>>);
    returnSize += sizeof(v_eb<T> *);
    returnSize += sizeof(void *) * 2;
    if (non_empty_high) {
      returnSize += non_empty_high -> eefsize();
    }
    for (auto it = buckets.begin(), end = buckets.end(); it != end; ++it) {
      returnSize += (*it).eefsize();
    }
    return returnSize;
  }
};