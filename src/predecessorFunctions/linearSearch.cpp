#include <utility>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

#ifndef LINEARSEARCH
#define LINEARSEARCH

pair<uint64_t, sd_vector<>>* linearSearch(vector<pair<uint64_t, sd_vector<>>> &orderedVector, uint64_t element) {
  for(vector<pair<uint64_t, sd_vector<>>>::iterator it = (orderedVector).begin(), end = (orderedVector).end(); it != end; ++it) {
    if(element < (*it).first) {
      if ((*it).first == (*(orderedVector).begin()).first) {
        return NULL;
      }
      --it;
      return &(*it);
    }
  }
  return &(*((orderedVector).end()-1));
}

#endif