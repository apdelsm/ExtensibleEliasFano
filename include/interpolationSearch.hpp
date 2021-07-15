#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

tuple<uint32_t, uint32_t, sd_vector<>>* interpolationSearch(vector<tuple<uint32_t, uint32_t, sd_vector<>>*> &orderedVector, uint32_t element);