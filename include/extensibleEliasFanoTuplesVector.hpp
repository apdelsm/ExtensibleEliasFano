#include <tuple>
#include <vector>
#include <algorithm>

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

#ifndef EXTENSIBLEELIASFANOTUPLESVECTOR
#define EXTENSIBLEELIASFANOTUPLESVECTOR

class ExtensibleEliasFanoTuplesVector {
  private:
    vector<uint32_t> buffer;
    uint32_t bufferFill = 0;
    uint32_t count = 0;
    uint32_t blocksCount = 0;
    vector<tuple<uint32_t, sd_vector<>>> blocks; //key, vector
    char whereIsPredecessor(uint32_t position);
    tuple<uint32_t, sd_vector<>>*(*predecessorFunction)(vector<tuple<uint32_t, sd_vector<>>>&, uint32_t, uint32_t*);

  public:
    ExtensibleEliasFanoTuplesVector(uint32_t bufferSize, tuple<uint32_t, sd_vector<>>*(*predecessorFunction)(vector<tuple<uint32_t, sd_vector<>>>&, uint32_t, uint32_t*));
    int pushBit(uint32_t bit);
    int select1(uint32_t occurrence, uint32_t &result);
    uint32_t rank1(uint32_t position);
    uint32_t size();
};

ExtensibleEliasFanoTuplesVector::ExtensibleEliasFanoTuplesVector(uint32_t bufferSize, tuple<uint32_t, sd_vector<>>*(*predecessorFunction)(vector<tuple<uint32_t, sd_vector<>>>&, uint32_t, uint32_t*)) {
  this->buffer = vector<uint32_t>(bufferSize, 0);
  this -> predecessorFunction = predecessorFunction;
}

int ExtensibleEliasFanoTuplesVector::pushBit(uint32_t bit) {
  if(bit) {
    (this->buffer)[bufferFill] = count;
    ++(this->bufferFill);
    if (this->bufferFill == (this->buffer).capacity()) {
      this->bufferFill = 0;
      uint32_t first = (this->buffer)[0];
      for (vector<uint32_t>::iterator it = (this->buffer).begin(), end = (this->buffer).end(); it != end; ++it) {
        *it -= first;
      }
      blocks.emplace_back(first, sd_vector<>((this->buffer).begin(), (this->buffer).end()));
      ++this->blocksCount;
    }
  }
  ++count;
  return count;
}

char ExtensibleEliasFanoTuplesVector::whereIsPredecessor(uint32_t position) {
  if (buffer[0] <= position && bufferFill != 0) {
    return 'b';
  } else {
    if (blocksCount == 0) {
      return 'n';
    } else {
      return 's';
    }
  }
}

int ExtensibleEliasFanoTuplesVector::select1(uint32_t occurrence, uint32_t &result) {
  uint32_t block = (occurrence-1)/buffer.size();
  if (block > blocksCount) {
    return 0;
  } else if (block < blocksCount) {
    tuple<uint32_t, sd_vector<>>* containerBlock = &(blocks[block]);
    uint32_t subOccurrence = occurrence - block * uint32_t(buffer.size());
    sd_vector<>::select_1_type selectSD(&(get<1>(*containerBlock)));
    result = selectSD(subOccurrence) + get<0>(*containerBlock);
    return 1;
  } else {
    if (occurrence <= blocksCount*buffer.size() + bufferFill) {
      result = buffer[occurrence - blocksCount*buffer.size() - 1];
      return 1;
    }
  }
  return 0;
}

uint32_t ExtensibleEliasFanoTuplesVector::rank1(uint32_t position) {
  char predecessorPlace = whereIsPredecessor(position);
  if (predecessorPlace == 'n') {
    return 0;
  } else if (predecessorPlace == 'b') {
    vector<uint32_t>::iterator low = upper_bound(buffer.begin(), buffer.begin() + bufferFill, position);
    return blocksCount * buffer.size() + (low - buffer.begin());
  } else {
    uint32_t block;
    tuple<uint32_t, sd_vector<>>* predecessorBlock = predecessorFunction(blocks, position, &block);
    if (predecessorBlock) {
      uint64_t relativePosition = position - get<0>(*predecessorBlock) + 1;
      uint32_t ones = sd_vector<>::rank_1_type(&get<1>(*predecessorBlock))(min(relativePosition, get<1>(*predecessorBlock).size()));
      return block * buffer.size() + ones;
    } else {
      return 0;
    }
  }
}

uint32_t ExtensibleEliasFanoTuplesVector::size() {
  uint32_t returnSize = 0;
  returnSize += sizeof(vector<uint32_t>);
  returnSize += sizeof(uint32_t) * buffer.size();
  returnSize += sizeof(uint32_t) * 3;
  returnSize += sizeof(vector<tuple<uint32_t, sd_vector<>>>);
  returnSize += sizeof(tuple<uint32_t, sd_vector<>>) * blocks.size();
  returnSize += sizeof(predecessorFunction);
  for (vector<tuple<uint32_t, sd_vector<>>>::iterator it = blocks.begin(), end = blocks.end(); it != end; ++it) {
    returnSize += size_in_bytes(get<1>(*it));
  }
  return returnSize;
}
#endif