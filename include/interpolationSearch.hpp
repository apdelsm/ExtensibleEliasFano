#include <utility>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

pair<uint64_t, sd_vector<>>* interpolationSearch(vector<pair<uint64_t, sd_vector<>>> &orderedVector, uint64_t element);