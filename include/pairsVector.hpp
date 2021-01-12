#include <vector>
#include <utility>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

class PairsVector {
  private:
    vector<pair<uint64_t, sd_vector<>>> pairsVector;
    pair<uint64_t, sd_vector<>>*(*predecessorFunction)(vector<pair<uint64_t, sd_vector<>>>&, uint64_t);
  public:
    PairsVector(pair<uint64_t, sd_vector<>>*(*predecessorFunction)(vector<pair<uint64_t, sd_vector<>>>&, uint64_t));
    void push(pair<uint64_t, sd_vector<>> element);
    pair<uint64_t, sd_vector<>>* getPredecessor(uint64_t value);
};