#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

#ifndef LINEARSEARCH
#define LINEARSEARCH

tuple<uint64_t, uint64_t, sd_vector<>>* linearSearch(vector<tuple<uint64_t, uint64_t, sd_vector<>>*> &orderedVector, uint64_t element) {
  for(vector<tuple<uint64_t, uint64_t, sd_vector<>>*>::iterator it = orderedVector.begin(), end = orderedVector.end(); it != end; ++it) {
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