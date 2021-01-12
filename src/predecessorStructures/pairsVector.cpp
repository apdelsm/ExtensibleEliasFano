#include <vector>
#include <utility>

#include <sdsl/bit_vectors.hpp>

#include "../../include/pairsVector.hpp"

using namespace std;
using namespace sdsl;

#ifndef PAIRSVECTOR
#define PAIRSVECTOR

PairsVector::PairsVector(pair<uint64_t, sd_vector<>>*(*predecessorFunction)(vector<pair<uint64_t, sd_vector<>>>&, uint64_t)) {
  this -> predecessorFunction = predecessorFunction;
}

void PairsVector::push(pair<uint64_t, sd_vector<>> element) {
  this -> pairsVector.push_back(element);
}

pair<uint64_t, sd_vector<>>* PairsVector::getPredecessor(uint64_t value) {
  return (this -> predecessorFunction)(this -> pairsVector, value);
}

#endif