#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

tuple<uint64_t, uint64_t, sd_vector<>>* binarySearch(vector<tuple<uint64_t, uint64_t, sd_vector<>>> &orderedVector, uint64_t element);