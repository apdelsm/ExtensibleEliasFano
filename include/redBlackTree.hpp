#include <map>
#include <tuple>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

class RedBlackTree {
  private:
    map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*> redBlackTree;
  public:
    RedBlackTree();
    ~RedBlackTree();
    void push(tuple<uint64_t, uint64_t, sd_vector<>> *element, uint64_t bufferSize);
    tuple<uint64_t, uint64_t, sd_vector<>>* getPredecessor(uint64_t value, uint64_t bufferSize);
    uint64_t eefsize();
};