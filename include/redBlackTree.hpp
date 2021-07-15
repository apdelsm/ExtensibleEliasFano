#include <map>
#include <tuple>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

class RedBlackTree {
  private:
    map<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*> redBlackTree;
  public:
    RedBlackTree();
    ~RedBlackTree();
    void push(tuple<uint32_t, uint32_t, sd_vector<>> *element, uint32_t bufferSize);
    tuple<uint32_t, uint32_t, sd_vector<>>* getPredecessor(uint32_t value, uint32_t bufferSize);
    uint32_t eefsize();
};