#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

#ifndef BINARYSEARCH
#define BINARYSEARCH

tuple<uint32_t, sd_vector<>>* binarySearch(vector<tuple<uint32_t, sd_vector<>>> &orderedVector, uint32_t element, uint32_t *block) {
  uint32_t low = 0;
  uint32_t high = orderedVector.size() - 1;
  uint32_t mid;
  if (element < get<0>(orderedVector[0])) {
    return NULL;
  }
  while(low != high && low < high - 1) {
    mid = (low + high + 1) / 2;
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