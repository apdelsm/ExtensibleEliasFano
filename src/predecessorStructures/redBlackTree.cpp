#include <map>
#include <tuple>

#include <sdsl/bit_vectors.hpp>

#include "../../include/redBlackTree.hpp"

using namespace std;
using namespace sdsl;

#ifndef REDBLACKTREE
#define REDBLACKTREE

RedBlackTree::RedBlackTree() {}

RedBlackTree::~RedBlackTree() {}

void RedBlackTree::push(tuple<uint64_t, uint64_t, sd_vector<>> *element) {
  (this -> redBlackTree)[get<1>(*element)] = element;
}

tuple<uint64_t, uint64_t, sd_vector<>>* RedBlackTree::getPredecessor(uint64_t value) {
  map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>::iterator lowerResult = redBlackTree.lower_bound(value);
  if (lowerResult == redBlackTree.begin() && lowerResult -> first > value) {
    return NULL;
  }
  if (lowerResult -> first > value || lowerResult == redBlackTree.end()) {
    --lowerResult;
  }
  return lowerResult -> second;
}

uint64_t RedBlackTree::eefsize() {
  uint64_t returnSize = 0;
  returnSize += sizeof(map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>);
  returnSize += (sizeof(uint64_t) + sizeof(tuple<uint64_t, uint64_t, sd_vector<>>*)) * redBlackTree.size();
  return returnSize;
}

#endif