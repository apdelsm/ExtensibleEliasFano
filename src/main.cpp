#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include <sdsl/bit_vectors.hpp>

#include "../include/avlMap.h"
#include "../include/binarySearch.hpp"
#include "../include/btree_map.h"
#include "../include/extensibleEliasFano.hpp"
#include "../include/interpolationSearch.hpp"
#include "../include/linearSearch.hpp"
#include "../include/redBlackTree.hpp"
#include "../include/tuplesVector.hpp"
#include "../include/vEB.h"
#include "../include/x-fast-map.h"
#include "../include/y-fast.h"

using namespace std;
using namespace sdsl;

void manageBit(string testType, uint64_t bufferSize, float probabilitie, int zerosRunSize, int onesRunSize, int test, ExtensibleEliasFano<TuplesVector> *tupleLineal, ExtensibleEliasFano<TuplesVector> *tupleInterpolation, ExtensibleEliasFano<TuplesVector> *tupleBinary, ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *btreeMap, ExtensibleEliasFano<RedBlackTree> *redBlackTree, ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *avlMap, ExtensibleEliasFano<x_fast_map<uint64_t>> *xFastMap, ExtensibleEliasFano<y_fast<uint64_t>> *yFastMap, char action, uint64_t actionNumber = 0);
void runTest(string testFilePath, uint64_t bufferSize, string testType, float probabilitie, int zerosRunSize, int onesRunSize, int test);
int main() {

  vector<uint64_t> bufferSizes = {64, 128, 256, 512, 1024, 2048};

  for (vector<uint64_t>::iterator bufferIt = bufferSizes.begin(), bufferEnd = bufferSizes.end(); bufferIt != bufferEnd; ++bufferIt){
    
    // Random bits

    string path = "./tests/randomBits/";
    for (const auto & entry : filesystem::directory_iterator(path)) {
      string filePath = entry.path();
      string filename = filePath.substr(filePath.find_last_of("/") + 1);
      float prob = (float)stoi(filename.substr(1,3)) / 100;
      int test = stoi(filename.substr(4, filename.find('.')));
      cout << filePath << endl;
      runTest(filePath, *bufferIt, "Random", prob, 1, 1, test);
    }

    //runTest("./tests/randomBits/p60t0.txt", *bufferIt, "Random", 0.6, 1, 1, 0);

    // Random Runs

    path = "./tests/randomRuns/";
    for (const auto & entry : filesystem::directory_iterator(path)) {
      string filePath = entry.path();
      string filename = filePath.substr(filePath.find_last_of("/") + 1);
      int zPos = filename.find('z');
      int oPos = filename.find('o');
      int pPos = filename.find('p');
      int zeros = stoi(filename.substr(zPos + 1, oPos));
      int ones = stoi(filename.substr(oPos + 1, pPos));
      float prob = (float)stoi(filename.substr(pPos + 1, pPos + 3)) / 100;
      int test = stoi(filename.substr(pPos + 4, filename.find('.')));
      cout << filePath << endl;
      runTest(filePath, *bufferIt, "Random Runs", prob, zeros, ones, test);
    }
    
    //runTest("./tests/randomRuns/z1024o256p60t0.txt", *bufferIt, "Random Runs", 0.4, 1024, 16384, 0);
    
    // Poisson Runs
    path = "./tests/poissonRuns/";
    for (const auto & entry : filesystem::directory_iterator(path)) {
      string filePath = entry.path();
      string filename = filePath.substr(filePath.find_last_of("/") + 1);
      int zPos = filename.find('z');
      int oPos = filename.find('o');
      int pPos = filename.find('p');
      int zeros = stoi(filename.substr(zPos + 1, oPos));
      int ones = stoi(filename.substr(oPos + 1, pPos));
      float prob = (float)stoi(filename.substr(pPos + 1, pPos + 3)) / 100;
      int test = stoi(filename.substr(pPos + 4, filename.find('.')));
      cout << filePath << endl;
      runTest(filePath, *bufferIt, "Random Runs", prob, zeros, ones, test);
    }

    //runTest("./tests/randomRuns/z1024o256p60t0.txt", *bufferIt, "Random Runs", 0.4, 1024, 16384, 0);
    
  }
  return 0;
}

void runTest(string testFilePath, uint64_t bufferSize, string testType, float probabilitie, int zerosRunSize, int onesRunSize, int test) {
  int bitsLimit;
  ifstream testFile(testFilePath);
  if (!testFile.is_open()) {
    cout << "error opening file "<< testFilePath << endl;
    exit(50);
  }
  testFile >> bitsLimit;
  const uint64_t uExp = ceil(log2(bitsLimit/bufferSize));
  ExtensibleEliasFano<TuplesVector> *tupleLineal = new ExtensibleEliasFano<TuplesVector>(bufferSize, linearSearch);
  ExtensibleEliasFano<TuplesVector> *tupleInterpolation = new ExtensibleEliasFano<TuplesVector>(bufferSize, interpolationSearch);
  ExtensibleEliasFano<TuplesVector> *tupleBinary = new ExtensibleEliasFano<TuplesVector>(bufferSize, binarySearch);
  ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *btreeMap = new ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>>(bufferSize);
  ExtensibleEliasFano<RedBlackTree> *redBlackTree = new ExtensibleEliasFano<RedBlackTree>(bufferSize);
  ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *avlMap = new ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>>(bufferSize);
  ExtensibleEliasFano<x_fast_map<uint64_t>> *xFastMap = new ExtensibleEliasFano<x_fast_map<uint64_t>>(bufferSize, uExp);
  ExtensibleEliasFano<y_fast<uint64_t>> *yFastMap = new ExtensibleEliasFano<y_fast<uint64_t>>(bufferSize, uExp);

  //iterate over file, for every bit
  char fileRead;
  testFile.get(fileRead);
  char action;
  uint64_t actionNumber;
  while (testFile.get(fileRead)) {
    if (fileRead == 's') {
      testFile >> actionNumber;
      action = 's';
      testFile.get(fileRead);
      manageBit(testType, bufferSize, probabilitie, zerosRunSize, onesRunSize, test, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap, action, actionNumber);
    } else if (fileRead == 'r') {
      testFile >> actionNumber;
      action = 'r';
      testFile.get(fileRead);
      manageBit(testType, bufferSize, probabilitie, zerosRunSize, onesRunSize, test, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap, action, actionNumber);
    } else if (fileRead == '0' || fileRead == '1') {
      actionNumber = fileRead - '0';
      action = 'i';
      manageBit(testType, bufferSize, probabilitie, zerosRunSize, onesRunSize, test, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap, action, actionNumber);
    }
  }
  testFile.close();

  manageBit("", 0, 0, 1, 1, 0, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap, 'c');
  delete tupleLineal;
  delete tupleInterpolation;
  delete tupleBinary;
  delete btreeMap;
  delete redBlackTree;
  delete avlMap;
  delete xFastMap;
  delete yFastMap;
}

void manageBit(string testType, uint64_t bufferSize, float probabilitie, int zerosRunSize, int onesRunSize, int test, ExtensibleEliasFano<TuplesVector> *tupleLineal, ExtensibleEliasFano<TuplesVector> *tupleInterpolation, ExtensibleEliasFano<TuplesVector> *tupleBinary, ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *btreeMap, ExtensibleEliasFano<RedBlackTree> *redBlackTree, ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *avlMap, ExtensibleEliasFano<x_fast_map<uint64_t>> *xFastMap, ExtensibleEliasFano<y_fast<uint64_t>> *yFastMap, char action, uint64_t actionNumber) {
  static uint64_t count = 0;
  static uint64_t ones = 0;
  static uint64_t selects = 0;
  static uint64_t ranks = 0;

  auto start = chrono::system_clock::now();
  auto end = chrono::system_clock::now();
  static chrono::duration<float, milli> tupleLinealInsertTime = start - start;
  static chrono::duration<float, milli> tupleLinealSelectTime = start - start;
  static chrono::duration<float, milli> tupleLinealRankTime = start - start;
  static chrono::duration<float, milli> tupleInterpolationInsertTime = start - start;
  static chrono::duration<float, milli> tupleInterpolationSelectTime = start - start;
  static chrono::duration<float, milli> tupleInterpolationRankTime = start - start;
  static chrono::duration<float, milli> tupleBinaryInsertTime = start - start;
  static chrono::duration<float, milli> tupleBinarySelectTime = start - start;
  static chrono::duration<float, milli> tupleBinaryRankTime = start - start;
  static chrono::duration<float, milli> btreeMapInsertTime = start - start;
  static chrono::duration<float, milli> btreeMapSelectTime = start - start;
  static chrono::duration<float, milli> btreeMapRankTime = start - start;
  static chrono::duration<float, milli> redBlackTreeInsertTime = start - start;
  static chrono::duration<float, milli> redBlackTreeSelectTime = start - start;
  static chrono::duration<float, milli> redBlackTreeRankTime = start - start;
  static chrono::duration<float, milli> avlMapInsertTime = start - start;
  static chrono::duration<float, milli> avlMapSelectTime = start - start;
  static chrono::duration<float, milli> avlMapRankTime = start - start;
  static chrono::duration<float, milli> xFastMapInsertTime = start - start;
  static chrono::duration<float, milli> xFastMapSelectTime = start - start;
  static chrono::duration<float, milli> xFastMapRankTime = start - start;
  static chrono::duration<float, milli> yFastMapInsertTime = start - start;
  static chrono::duration<float, milli> yFastMapSelectTime = start - start;
  static chrono::duration<float, milli> yFastMapRankTime = start - start;

  if (action == 'c') {
    count = 0;
    ones = 0;
    selects = 0;
    ranks = 0;
    tupleLinealInsertTime = start - start;
    tupleLinealSelectTime = start - start;
    tupleLinealRankTime = start - start;
    tupleInterpolationInsertTime = start - start;
    tupleInterpolationSelectTime = start - start;
    tupleInterpolationRankTime = start - start;
    tupleBinaryInsertTime = start - start;
    tupleBinarySelectTime = start - start;
    tupleBinaryRankTime = start - start;
    btreeMapInsertTime = start - start;
    btreeMapSelectTime = start - start;
    btreeMapRankTime = start - start;
    redBlackTreeInsertTime = start - start;
    redBlackTreeSelectTime = start - start;
    redBlackTreeRankTime = start - start;
    avlMapInsertTime = start - start;
    avlMapSelectTime = start - start;
    avlMapRankTime = start - start;
    xFastMapInsertTime = start - start;
    xFastMapSelectTime = start - start;
    xFastMapRankTime = start - start;
    yFastMapInsertTime = start - start;
    yFastMapSelectTime = start - start;
    yFastMapRankTime = start - start;
  } else if (action == 'i') {

    uint64_t aux;

    count += 1;

    ones += actionNumber;
    cout << "insert | count: " << count << " | ones: " << ones << endl;
    start = chrono::system_clock::now();
    aux = tupleLineal -> pushBit(actionNumber);
    end = chrono::system_clock::now();
    tupleLinealInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(11);
    }

    start = chrono::system_clock::now();
    aux = tupleInterpolation -> pushBit(actionNumber);
    end = chrono::system_clock::now();
    tupleInterpolationInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(12);
    }

    start = chrono::system_clock::now();
    aux = tupleBinary -> pushBit(actionNumber);
    end = chrono::system_clock::now();
    tupleBinaryInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(13);
    }

    start = chrono::system_clock::now();
    aux = btreeMap -> pushBit(actionNumber);
    end = chrono::system_clock::now();
    btreeMapInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(14);
    }

    start = chrono::system_clock::now();
    aux = redBlackTree -> pushBit(actionNumber);
    end = chrono::system_clock::now();
    redBlackTreeInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(15);
    }

    start = chrono::system_clock::now();
    aux = avlMap -> pushBit(actionNumber);
    end = chrono::system_clock::now();
    avlMapInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(16);
    }

    start = chrono::system_clock::now();
    aux = xFastMap -> pushBit(actionNumber);
    end = chrono::system_clock::now();
    xFastMapInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(17);
    }

    start = chrono::system_clock::now();
    aux = yFastMap -> pushBit(actionNumber);
    end = chrono::system_clock::now();
    yFastMapInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(18);
    }

    if (count % 100 == 0) {
      cout << "register" << endl;
      static bool header = true;
      ofstream results;
      if (header) {
        results.open("results.csv", ofstream::trunc);
        results << "TestType;BufferSize;Probabilitie;ZerosRunSize;OnesRunSize;TestCount;Structure;Bits;Ones;Size;Selects;Ranks;Insert Time;Random Select Time;Random Rank Time" << endl;;
        header = false;
        results.close();
      }
      results.open("results.csv", ofstream::app);
      results << testType << ";" << bufferSize << ";" << probabilitie << ";" << zerosRunSize << ";" << onesRunSize << ";" << test << ";"
              << "Tuples Lineal;" << count << ";" << ones << ";" << tupleLineal -> size() << ";" << selects << ";" << ranks << ";"
              << tupleLinealInsertTime.count() << ";" << tupleLinealSelectTime.count() << ";" << tupleLinealRankTime.count() << endl;
      results << testType << ";" << bufferSize << ";" << probabilitie << ";" << zerosRunSize << ";" << onesRunSize << ";" << test << ";"
              << "Tuples Interpolation;" << count << ";" << ones << ";" << tupleInterpolation -> size() << ";" << selects << ";" << ranks << ";"
              << tupleInterpolationInsertTime.count() << ";" << tupleInterpolationSelectTime.count() << ";" << tupleInterpolationRankTime.count() << endl;
      results << testType << ";" << bufferSize << ";" << probabilitie << ";" << zerosRunSize << ";" << onesRunSize << ";" << test << ";"
              << "Tuples Binary;" << count << ";" << ones << ";" << tupleBinary -> size() << ";" << selects << ";" << ranks << ";"
              << tupleBinaryInsertTime.count() << ";" << tupleBinarySelectTime.count() << ";" << tupleBinaryRankTime.count() << endl;
      results << testType << ";" << bufferSize << ";" << probabilitie << ";" << zerosRunSize << ";" << onesRunSize << ";" << test << ";"
              << "Btree map;" << count << ";" << ones << ";" << btreeMap -> size() << ";" << selects << ";" << ranks << ";"
              << btreeMapInsertTime.count() << ";" << btreeMapSelectTime.count() << ";" << btreeMapRankTime.count() << endl;
      results << testType << ";" << bufferSize << ";" << probabilitie << ";" << zerosRunSize << ";" << onesRunSize << ";" << test << ";"
              << "Red Black Tree;" << count << ";" << ones << ";" << redBlackTree -> size() << ";" << selects << ";" << ranks << ";"
              << redBlackTreeInsertTime.count() << ";" << redBlackTreeSelectTime.count() << ";" << redBlackTreeRankTime.count() << endl;
      results << testType << ";" << bufferSize << ";" << probabilitie << ";" << zerosRunSize << ";" << onesRunSize << ";" << test << ";"
              << "Avl Map;" << count << ";" << ones << ";" << avlMap -> size() << ";" << selects << ";" << ranks << ";"
              << avlMapInsertTime.count() << ";" << avlMapSelectTime.count() << ";" << avlMapRankTime.count() << endl;
      results << testType << ";" << bufferSize << ";" << probabilitie << ";" << zerosRunSize << ";" << onesRunSize << ";" << test << ";"
              << "X-fast-trie;" << count << ";" << ones << ";" << xFastMap -> size() << ";" << selects << ";" << ranks << ";"
              << xFastMapInsertTime.count() << ";" << xFastMapSelectTime.count() << ";" << xFastMapRankTime.count() << endl;
      results << testType << ";" << bufferSize << ";" << probabilitie << ";" << zerosRunSize << ";" << onesRunSize << ";" << test << ";"
              << "Y-fast-trie;" << count << ";" << ones << ";" << yFastMap -> size() << ";" << selects << ";" << ranks << ";"
              << yFastMapInsertTime.count() << ";" << yFastMapSelectTime.count() << ";" << yFastMapRankTime.count() << endl;
      results.close();
    }
  } else if (action == 's') {
    uint64_t aux;
    uint64_t selectResult;
    uint64_t aux2;

    selects += 1;
    cout << "select: " << actionNumber << endl;

    start = chrono::system_clock::now();
    aux = tupleLineal -> select1(actionNumber,aux2);
    end = chrono::system_clock::now();
    tupleLinealSelectTime += end - start;
    cout << aux << endl;
    selectResult = aux2;

    start = chrono::system_clock::now();
    aux = tupleInterpolation -> select1(actionNumber,aux2);
    end = chrono::system_clock::now();
    tupleInterpolationSelectTime += end - start;
    cout << aux << endl;
    if (selectResult != aux2) {
      exit(21);
    }

    start = chrono::system_clock::now();
    aux = tupleBinary -> select1(actionNumber,aux2);
    end = chrono::system_clock::now();
    tupleBinarySelectTime += end - start;
    cout << aux << endl;
    if (selectResult != aux2) {
      exit(22);
    }

    start = chrono::system_clock::now();
    aux = btreeMap -> select1(actionNumber,aux2);
    end = chrono::system_clock::now();
    btreeMapSelectTime += end - start;
    cout << aux << endl;
    if (selectResult != aux2) {
      exit(23);
    }

    start = chrono::system_clock::now();
    aux = redBlackTree -> select1(actionNumber,aux2);
    end = chrono::system_clock::now();
    redBlackTreeSelectTime += end - start;
    cout << aux << endl;
    if (selectResult != aux2) {
      exit(24);
    }

    start = chrono::system_clock::now();
    aux = avlMap -> select1(actionNumber,aux2);
    end = chrono::system_clock::now();
    avlMapSelectTime += end - start;
    cout << aux << endl;
    if (selectResult != aux2) {
      exit(25);
    }

    start = chrono::system_clock::now();
    aux = xFastMap -> select1(actionNumber,aux2);
    end = chrono::system_clock::now();
    xFastMapSelectTime += end - start;
    cout << aux << endl;
    if (selectResult != aux2) {
      exit(26);
    }

    start = chrono::system_clock::now();
    aux = yFastMap -> select1(actionNumber,aux2);
    end = chrono::system_clock::now();
    yFastMapSelectTime += end - start;
    cout << aux << endl;
    if (selectResult != aux2) {
      exit(27);
    }
  } else {

    uint64_t rankResult;
    uint64_t aux;

    ranks += 1;
    cout << "rank: " << actionNumber << endl;
    start = chrono::system_clock::now();
    aux = tupleLineal -> rank1(actionNumber);
    end = chrono::system_clock::now();
    tupleLinealRankTime += end - start;
    cout << aux << endl;
    rankResult = aux;

    start = chrono::system_clock::now();
    aux = tupleInterpolation -> rank1(actionNumber);
    end = chrono::system_clock::now();
    tupleInterpolationRankTime += end - start;
    cout << aux << endl;
    if (rankResult != aux) {
      exit(31);
    }

    start = chrono::system_clock::now();
    aux = tupleBinary -> rank1(actionNumber);
    end = chrono::system_clock::now();
    tupleBinaryRankTime += end - start;
    cout << aux << endl;
    if (rankResult != aux) {
      exit(32);
    }

    start = chrono::system_clock::now();
    aux = btreeMap -> rank1(actionNumber);
    end = chrono::system_clock::now();
    btreeMapRankTime += end - start;
    cout << aux << endl;
    if (rankResult != aux) {
      exit(33);
    }

    start = chrono::system_clock::now();
    aux = redBlackTree -> rank1(actionNumber);
    end = chrono::system_clock::now();
    redBlackTreeRankTime += end - start;
    cout << aux << endl;
    if (rankResult != aux) {
      exit(34);
    }

    start = chrono::system_clock::now();
    aux = avlMap -> rank1(actionNumber);
    end = chrono::system_clock::now();
    avlMapRankTime += end - start;
    cout << aux << endl;
    if (rankResult != aux) {
      exit(35);
    }

    start = chrono::system_clock::now();
    aux = xFastMap -> rank1(actionNumber);
    end = chrono::system_clock::now();
    xFastMapRankTime += end - start;
    cout << aux << endl;
    if (rankResult != aux) {
      exit(36);
    }

    start = chrono::system_clock::now();
    aux = yFastMap -> rank1(actionNumber);
    end = chrono::system_clock::now();
    yFastMapRankTime += end - start;
    cout << aux << endl;
    if (rankResult != aux) {
      exit(37);
    }
  }
};
