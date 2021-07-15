#include <vector>
#include <tuple>

#include <sdsl/bit_vectors.hpp>

#include "../../include/tuplesVector.hpp"

using namespace std;
using namespace sdsl;

#ifndef TUPLESVECTOR
#define TUPLESVECTOR

TuplesVector::TuplesVector(tuple<uint32_t, uint32_t, sd_vector<>>*(*predecessorFunction)(vector<tuple<uint32_t, uint32_t, sd_vector<>>*>&, uint32_t)) {
  this -> predecessorFunction = predecessorFunction;
}

TuplesVector::~TuplesVector(){}

void TuplesVector::push(tuple<uint32_t, uint32_t, sd_vector<>> *element, uint32_t bufferSize) {
  this -> tuplesVector.push_back(element);
}

tuple<uint32_t, uint32_t, sd_vector<>>* TuplesVector::getPredecessor(uint32_t value, uint32_t bufferSize) {
  return (this -> predecessorFunction)(this -> tuplesVector, value);
}

uint32_t TuplesVector::eefsize() {
  uint32_t returnSize = 0;
  returnSize += sizeof(vector<tuple<uint32_t, uint32_t, sd_vector<>>*>);
  returnSize += sizeof(tuple<uint32_t, uint32_t, sd_vector<>>*) * tuplesVector.size();
  returnSize += sizeof(predecessorFunction);
  return returnSize;
}

#endif