#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

#ifndef INTERPOLATIONSEARCH
#define INTERPOLATIONSEARCH

tuple<uint32_t, sd_vector<>>* interpolationSearch(vector<tuple<uint32_t, sd_vector<>>> &orderedVector, uint32_t element, uint32_t *block) {
  uint32_t low = 0;
  uint32_t high = orderedVector.size() - 1;
  uint32_t mid;
  if (element < get<0>(orderedVector[0])) {
    return NULL;
  }
  while(low != high && low < high - 1) {
    mid = low + ((element - get<0>(orderedVector[low])) * (high - low) / (get<0>((orderedVector[high])) - get<0>(orderedVector[low])));
    if (mid == low) {
      ++mid;
    }
    if (mid > high) {
      mid = high;
    }
    if (element < get<0>(orderedVector[mid])) {
      high = mid - 1;
    } else {
      low = mid;
    }
  }
  if (element < get<0>(orderedVector[high])) {
    *block = low;
    return &(orderedVector[low]);
  } else {
    *block = high;
    return &(orderedVector[high]);
  }
}

#endif