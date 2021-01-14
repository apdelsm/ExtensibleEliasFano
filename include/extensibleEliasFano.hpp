#include <tuple>
#include <vector>

#include <sdsl/bit_vectors.hpp>

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
    uint64_t bufferCount = 0;
    T predecessorStructure;
    char whereIsPredecessor(uint64_t element);
    vector<tuple<uint64_t, uint64_t, sd_vector<>>*> blocks;

  public:
    ExtensibleEliasFano(uint64_t bufferSize, T predecessorStructure);
    void pushBit(uint64_t bit);
    uint64_t select1(uint64_t occurrence);
};

template <class T>
ExtensibleEliasFano<T>::ExtensibleEliasFano(uint64_t bufferSize, T predecessorStructure)
  : predecessorStructure(predecessorStructure)
{
  this->buffer = vector<uint64_t>(bufferSize, 0);
}

template <class T>
void ExtensibleEliasFano<T>::pushBit(uint64_t bit) {
  if(bit) {
    (this->buffer)[bufferFill] = count;
    ++(this->bufferFill);
    if (this->bufferFill == (this->buffer).capacity()) {
      this->bufferFill = 0;
      uint64_t first = (this->buffer)[0];
      for (vector<uint64_t>::iterator it = (this->buffer).begin(), end = (this->buffer).end(); it != end; ++it) {
        *it -= first;
      }
      tuple<uint64_t, uint64_t, sd_vector<>> bufferSDVector = make_tuple(this->bufferCount, first, sd_vector<>((this->buffer).begin(), (this->buffer).end()));
      blocks.push_back(&bufferSDVector);
      this->predecessorStructure.push(bufferSDVector);
      ++this->bufferCount;
    }
  }
  count++;
}

template <class T>
char ExtensibleEliasFano<T>::whereIsPredecessor(uint64_t element) {
  if (bufferFill == 0 && buffer.back() == 0) {
    return 'n';
  } else if (buffer.back() == 0) {
    if (buffer[0] < element) {
      return 'b';
    } else {
      return 'n';
    }
  } else {
    return 's';
  }
}

template <class T>
uint64_t ExtensibleEliasFano<T>::select1(uint64_t occurrence) {
  return 0;
}

#endif