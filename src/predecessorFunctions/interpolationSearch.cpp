#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

#ifndef INTERPOLATIONSEARCH
#define INTERPOLATIONSEARCH

tuple<uint64_t, uint64_t, sd_vector<>>* interpolationSearch(vector<tuple<uint64_t, uint64_t, sd_vector<>>> &orderedVector, uint64_t element) {
  uint64_t low = 0;
  uint64_t high = orderedVector.size() - 1;
  uint64_t mid;
  if (element < get<1>(orderedVector[0])) {
    return NULL;
  }
  while(low < high - 1) {
    mid = low + ((element - get<1>(orderedVector[low])) * (high - low) / (get<1>(orderedVector[high]) - get<1>(orderedVector[low])));
    if (mid > high) {
      mid = high;
    }
    if (element < get<1>(orderedVector[mid])) {
      high = mid - 1;
    } else if (element > get<1>(orderedVector[mid])) {
      low = mid;
    } else {
      return &(orderedVector[mid]);
    }
  }
  if (element < get<1>(orderedVector[high])) {
    return &(orderedVector[low]);
  } else {
    return &(orderedVector[high]);
  }
}

#endif