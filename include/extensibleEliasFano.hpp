#include <sdsl/bit_vectors.hpp>
#include <utility>
#include <vector>

using namespace std;
using namespace sdsl;

#ifndef EXTENSIBLEELIASFANO
#define EXTENSIBLEELIASFANO

template <class T>
class ExtensibleEliasFano {
  private:
    vector<uint64_t> buffer;
    uint64_t bufferFill = 0;
    uint64_t count = 0;
    T predecessorStructure;

  public:
    ExtensibleEliasFano(uint64_t bufferSize, T predecessorStructure);
    void pushBit(uint64_t bit);
};

template <class T>
ExtensibleEliasFano<T>::ExtensibleEliasFano(uint64_t bufferSize, T predecessorStructure)
  : predecessorStructure(predecessorStructure)
{
  this->buffer = vector<uint64_t>(bufferSize);
}

template <class T>
void ExtensibleEliasFano<T>::pushBit(uint64_t bit) {
  if(bit) {
    (this->buffer)[bufferFill] = count;
    ++(this->bufferFill);
    if (this->bufferFill == (this->buffer).capacity()) {
      this->bufferFill = 0;
      pair<int, sd_vector<>> bufferSDVector;
      bufferSDVector.first = (this->buffer)[0];
      for (vector<uint64_t>::iterator it = (this->buffer).begin(), end = (this->buffer).end(); it != end; ++it) {
        *it -= bufferSDVector.first;
      }
      bufferSDVector.second = sd_vector<>((this->buffer).begin(), (this->buffer).end());
      this->predecessorStructure.push(bufferSDVector);
    }
  }
  count++;
}

#endif