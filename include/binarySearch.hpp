#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

tuple<uint32_t, sd_vector<>>* binarySearch(vector<tuple<uint32_t, sd_vector<>>> &orderedVector, uint32_t element, uint32_t *block);