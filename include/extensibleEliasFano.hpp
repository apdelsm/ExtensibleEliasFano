#include <tuple>
#include <vector>
#include <algorithm>

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
    uint64_t blocksCount = 0;
    T predecessorStructure;
    char whereIsPredecessor(uint64_t position);
    vector<tuple<uint64_t, uint64_t, sd_vector<>>*> blocks;

  public:
    ExtensibleEliasFano(uint64_t bufferSize, T predecessorStructure);
    ~ExtensibleEliasFano();
    void pushBit(uint64_t bit);
    int select1(uint64_t occurrence, uint64_t &result);
    uint64_t rank1(uint64_t position);
    uint64_t size();
};

template <class T>
ExtensibleEliasFano<T>::ExtensibleEliasFano(uint64_t bufferSize, T predecessorStructure)
  : predecessorStructure(predecessorStructure)
{
  this->buffer = vector<uint64_t>(bufferSize, 0);
}

template <class T>
ExtensibleEliasFano<T>::~ExtensibleEliasFano() {
  for (vector<tuple<uint64_t, uint64_t, sd_vector<>>*>::iterator it = blocks.begin(), end = blocks.end(); it != end; ++it) {
    delete *it;
  }
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
      tuple<uint64_t, uint64_t, sd_vector<>>* bufferSDVector = new tuple<uint64_t, uint64_t, sd_vector<>>(this->blocksCount, first, sd_vector<>((this->buffer).begin(), (this->buffer).end()));
      blocks.push_back(bufferSDVector);
      this->predecessorStructure.push(bufferSDVector);
      ++this->blocksCount;
    }
  }
  count++;
}

template <class T>
char ExtensibleEliasFano<T>::whereIsPredecessor(uint64_t position) {
  if (buffer[0] < position && bufferFill != 0) {
    return 'b';
  } else {
    if (blocksCount == 0) {
      return 'n';
    } else {
      return 's';
    }
  }
}

template <class T>
int ExtensibleEliasFano<T>::select1(uint64_t occurrence, uint64_t &result) {
  uint64_t block = (occurrence-1)/buffer.size();
  if (block > blocksCount) {
    return 0;
  } else if (block < blocksCount) {
    tuple<uint64_t, uint64_t, sd_vector<>>* containerBlock = blocks[block];
    uint64_t subOccurrence = occurrence - get<0>(*containerBlock) * uint64_t(buffer.size());
    sd_vector<>::select_1_type selectSD(&(get<2>(*containerBlock)));
    result = selectSD(subOccurrence) + get<1>(*containerBlock);
    return 1;
  } else {
    if (occurrence <= blocksCount*buffer.size() + bufferFill) {
      result = buffer[occurrence - blocksCount*buffer.size() - 1];
      return 1;
    }
  }
  return 0;
}

template <class T>
uint64_t ExtensibleEliasFano<T>::rank1(uint64_t position) {
  char predecessorPlace = whereIsPredecessor(position);
  if (predecessorPlace == 'n') {
    return 0;
  } else if (predecessorPlace == 'b') {
    for (uint64_t i = 0; i < bufferFill; ++i) {
      if (buffer[i] > position) {
        return blocksCount * buffer.size() + i;
      }
    }
    return blocksCount * buffer.size() + bufferFill;
  } else {
    tuple<uint64_t, uint64_t, sd_vector<>>* predecessorBlock = predecessorStructure.getPredecessor(position);
    if (predecessorBlock) {
      uint64_t relativePosition = position - get<1>(*predecessorBlock) + 1;
      uint64_t ones = sd_vector<>::rank_1_type(&get<2>(*predecessorBlock))(relativePosition);
      return get<0>(*predecessorBlock) * buffer.size() + ones;
    } else {
      return 0;
    }
  }
}

template <class T>
uint64_t ExtensibleEliasFano<T>::size() {
  uint64_t returnSize = 0;
  returnSize += sizeof(vector<uint64_t>);
  returnSize += sizeof(uint64_t) * buffer.size();
  returnSize += sizeof(uint64_t) * 3;
  returnSize += predecessorStructure.size();
  returnSize += sizeof(vector<tuple<uint64_t, uint64_t, sd_vector<>>*>);
  returnSize += sizeof(tuple<uint64_t, uint64_t, sd_vector<>>*) * blocks.size();
  for (vector<tuple<uint64_t, uint64_t, sd_vector<>>*>::iterator it = blocks.begin(), end = blocks.end(); it != end; ++it) {
    returnSize += size_in_bytes(get<2>(**it));
  }
  return returnSize;
}
#endif