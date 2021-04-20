#include <vector>
#include <tuple>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

class TuplesVector {
  private:
    vector<tuple<uint64_t, uint64_t, sd_vector<>>*> tuplesVector;
    tuple<uint64_t, uint64_t, sd_vector<>>*(*predecessorFunction)(vector<tuple<uint64_t, uint64_t, sd_vector<>>*>&, uint64_t);
  public:
    TuplesVector(tuple<uint64_t, uint64_t, sd_vector<>>*(*predecessorFunction)(vector<tuple<uint64_t, uint64_t, sd_vector<>>*>&, uint64_t));
    ~TuplesVector();
    void push(tuple<uint64_t, uint64_t, sd_vector<>> *element);
    tuple<uint64_t, uint64_t, sd_vector<>>* getPredecessor(uint64_t value);
    uint64_t size();
};