#include <vector>
#include <tuple>

#include <sdsl/bit_vectors.hpp>

#include "../../include/tuplesVector.hpp"

using namespace std;
using namespace sdsl;

#ifndef TUPLESVECTOR
#define TUPLESVECTOR

TuplesVector::TuplesVector(tuple<uint64_t, uint64_t, sd_vector<>>*(*predecessorFunction)(vector<tuple<uint64_t, uint64_t, sd_vector<>>>&, uint64_t)) {
  this -> predecessorFunction = predecessorFunction;
}

void TuplesVector::push(tuple<uint64_t, uint64_t, sd_vector<>> element) {
  this -> tuplesVector.push_back(element);
}

tuple<uint64_t, uint64_t, sd_vector<>>* TuplesVector::getPredecessor(uint64_t value) {
  return (this -> predecessorFunction)(this -> tuplesVector, value);
}

#endif