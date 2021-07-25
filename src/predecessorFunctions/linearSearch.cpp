#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

#ifndef LINEARSEARCH
#define LINEARSEARCH

tuple<uint32_t, sd_vector<>>* linearSearch(vector<tuple<uint32_t, sd_vector<>>> &orderedVector, uint32_t element, uint32_t *block) {
  for(vector<tuple<uint32_t, sd_vector<>>>::iterator it = orderedVector.begin(), end = orderedVector.end(); it != end; ++it) {
    if(element < get<0>(*it)) {
      if (get<0>(*it) == get<0>(*orderedVector.begin())) {
        return NULL;
      }
      --it;
      *block = it - orderedVector.begin();
      return &(*it);
    }
  }
  *block = orderedVector.end() - 1 - orderedVector.begin();
  return &(*((orderedVector).end()-1));
}

#endif