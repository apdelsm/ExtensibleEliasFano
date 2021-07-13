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

void RedBlackTree::push(tuple<uint64_t, uint64_t, sd_vector<>> *element, uint64_t bufferSize) {
  (this -> redBlackTree)[get<1>(*element)] = element;
}

tuple<uint64_t, uint64_t, sd_vector<>>* RedBlackTree::getPredecessor(uint64_t value, uint64_t bufferSize) {
  map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>::iterator lowerResult = redBlackTree.lower_bound(value);
  if (lowerResult == redBlackTree.begin() && lowerResult -> first > value) {
    return NULL;
  }
  if (lowerResult == redBlackTree.end() || lowerResult -> first > value) {
    --lowerResult;
  }
  return lowerResult -> second;
}

uint64_t RedBlackTree::eefsize() {
  //every node use 32 bytes in pointers + memory of key and value
  uint64_t returnSize = 0;
  returnSize += sizeof(map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>);
  returnSize += (sizeof(uint64_t) + sizeof(tuple<uint64_t, uint64_t, sd_vector<>>*) + 32) * redBlackTree.size();
  return returnSize;
}

#endif