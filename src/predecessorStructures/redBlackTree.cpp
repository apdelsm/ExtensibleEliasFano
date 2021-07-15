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

void RedBlackTree::push(tuple<uint32_t, uint32_t, sd_vector<>> *element, uint32_t bufferSize) {
  (this -> redBlackTree)[get<1>(*element)] = element;
}

tuple<uint32_t, uint32_t, sd_vector<>>* RedBlackTree::getPredecessor(uint32_t value, uint32_t bufferSize) {
  map<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>::iterator lowerResult = redBlackTree.lower_bound(value);
  if (lowerResult == redBlackTree.begin() && lowerResult -> first > value) {
    return NULL;
  }
  if (lowerResult == redBlackTree.end() || lowerResult -> first > value) {
    --lowerResult;
  }
  return lowerResult -> second;
}

uint32_t RedBlackTree::eefsize() {
  //every node use 32 bytes in pointers + memory of key and value
  uint32_t returnSize = 0;
  returnSize += sizeof(map<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>);
  returnSize += (sizeof(uint32_t) + sizeof(tuple<uint32_t, uint32_t, sd_vector<>>*) + 32) * redBlackTree.size();
  return returnSize;
}

#endif