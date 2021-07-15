#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

#ifndef LINEARSEARCH
#define LINEARSEARCH

tuple<uint32_t, uint32_t, sd_vector<>>* linearSearch(vector<tuple<uint32_t, uint32_t, sd_vector<>>*> &orderedVector, uint32_t element) {
  for(vector<tuple<uint32_t, uint32_t, sd_vector<>>*>::iterator it = orderedVector.begin(), end = orderedVector.end(); it != end; ++it) {
    if(element < get<1>(**it)) {
      if (get<1>(**it) == get<1>(**orderedVector.begin())) {
        return NULL;
      }
      --it;
      return *it;
    }
  }
  return *((orderedVector).end()-1);
}

#endif