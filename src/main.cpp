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

void manageBit(string testType, uint32_t bufferSize, float probabilitie, int zerosRunSize, int onesRunSize, int test, ExtensibleEliasFano<TuplesVector> *tupleLineal, ExtensibleEliasFano<TuplesVector> *tupleInterpolation, ExtensibleEliasFano<TuplesVector> *tupleBinary, ExtensibleEliasFano<btree::btree_map<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>> *btreeMap, ExtensibleEliasFano<RedBlackTree> *redBlackTree, ExtensibleEliasFano<avl_tree<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>> *avlMap, ExtensibleEliasFano<x_fast_map<uint32_t>> *xFastMap, ExtensibleEliasFano<y_fast<uint32_t>> *yFastMap, char action, uint32_t actionNumber = 0, int confId = -1);
void runTest(string testFilePath, uint32_t bufferSize, string testType, float probabilitie, int zerosRunSize, int onesRunSize, int test, int confId, char struc);
int main(int argc, char *argv[]) {
  std::ios::sync_with_stdio(false);
  vector<uint32_t> bufferSizes = {256, 512, 1024, 2048};
  ofstream confIds;
  int idCount = 0;
  for (vector<uint32_t>::iterator bufferIt = bufferSizes.begin(), bufferEnd = bufferSizes.end(); bufferIt != bufferEnd; ++bufferIt){
    
    // Random bits

    string path = "./tests/randomBits/";
    for (const auto & entry : filesystem::directory_iterator(path)) {
      string filePath = entry.path();
      string filename = filePath.substr(filePath.find_last_of("/") + 1);
      float prob = (float)stoi(filename.substr(1,3)) / 100;
      int test = stoi(filename.substr(4, filename.find('.')));
      cout << filePath << endl;
      runTest(filePath, *bufferIt, "Random", prob, 1, 1, test, idCount, *argv[1]);
      ++idCount;
    }

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
      runTest(filePath, *bufferIt, "Random Runs", prob, zeros, ones, test, idCount, *argv[1]);
      ++idCount;
    }
        
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
      runTest(filePath, *bufferIt, "Random Runs", prob, zeros, ones, test, idCount, *argv[1]);
      ++idCount;
    }    
  }
  return 0;
}

void runTest(string testFilePath, uint32_t bufferSize, string testType, float probabilitie, int zerosRunSize, int onesRunSize, int test, int confId, char struc) {
  ExtensibleEliasFano<TuplesVector> *tupleLineal = NULL;
  ExtensibleEliasFano<TuplesVector> *tupleInterpolation = NULL;
  ExtensibleEliasFano<TuplesVector> *tupleBinary = NULL;
  ExtensibleEliasFano<btree::btree_map<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>> *btreeMap = NULL;
  ExtensibleEliasFano<RedBlackTree> *redBlackTree = NULL;
  ExtensibleEliasFano<avl_tree<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>> *avlMap = NULL;
  ExtensibleEliasFano<x_fast_map<uint32_t>> *xFastMap = NULL;
  ExtensibleEliasFano<y_fast<uint32_t>> *yFastMap = NULL;
  int bitsLimit;
  ifstream testFile(testFilePath);
  if (!testFile.is_open()) {
    cout << "error opening file "<< testFilePath << endl;
    exit(50);
  }
  testFile >> bitsLimit;
  const uint32_t uExp = ceil(log2(bitsLimit/bufferSize));

  if (struc == '0') {
    tupleLineal = new ExtensibleEliasFano<TuplesVector>(bufferSize, linearSearch);
  }
  else if (struc == '1') {
    tupleInterpolation = new ExtensibleEliasFano<TuplesVector>(bufferSize, interpolationSearch);
  }
  else if (struc == '2') {
    tupleBinary = new ExtensibleEliasFano<TuplesVector>(bufferSize, binarySearch);
  }
  else if (struc == '3') {
    btreeMap = new ExtensibleEliasFano<btree::btree_map<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>>(bufferSize);
  }
  else if (struc == '4') {
    redBlackTree = new ExtensibleEliasFano<RedBlackTree>(bufferSize);
  }
  else if (struc == '5') {
    avlMap = new ExtensibleEliasFano<avl_tree<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>>(bufferSize);
  }
  else if (struc == '6') {
    xFastMap = new ExtensibleEliasFano<x_fast_map<uint32_t>>(bufferSize, uExp);
  }
  else if (struc == '7') {
    yFastMap = new ExtensibleEliasFano<y_fast<uint32_t>>(bufferSize, uExp);
  }

  //iterate over file, for every bit
  char fileRead;
  testFile.get(fileRead);
  char action;
  uint32_t actionNumber;
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
      manageBit(testType, bufferSize, probabilitie, zerosRunSize, onesRunSize, test, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap, action, actionNumber, confId);
    }
  }
  testFile.close();

  manageBit("", 0, 0, 1, 1, 0, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap, 'c');
  if (struc == '0') {
    delete tupleLineal;
  }
  else if (struc == '1') {
    delete tupleInterpolation;
  }
  else if (struc == '2') {
    delete tupleBinary;
  }
  else if (struc == '3') {
    delete btreeMap;
  }
  else if (struc == '4') {
    delete redBlackTree;
  }
  else if (struc == '5') {
    delete avlMap;
  }
  else if (struc == '6') {
    delete xFastMap;
  }
  else if (struc == '7') {
    delete yFastMap;
  }
}

void manageBit(string testType, uint32_t bufferSize, float probabilitie, int zerosRunSize, int onesRunSize, int test, ExtensibleEliasFano<TuplesVector> *tupleLineal, ExtensibleEliasFano<TuplesVector> *tupleInterpolation, ExtensibleEliasFano<TuplesVector> *tupleBinary, ExtensibleEliasFano<btree::btree_map<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>> *btreeMap, ExtensibleEliasFano<RedBlackTree> *redBlackTree, ExtensibleEliasFano<avl_tree<uint32_t, tuple<uint32_t, uint32_t, sd_vector<>>*>> *avlMap, ExtensibleEliasFano<x_fast_map<uint32_t>> *xFastMap, ExtensibleEliasFano<y_fast<uint32_t>> *yFastMap, char action, uint32_t actionNumber, int confId) {
  static uint32_t count = 0;
  static uint32_t ones = 0;
  static uint32_t selects = 0;
  static uint32_t ranks = 0;

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

    uint32_t aux;

    count += 1;

    ones += actionNumber;
    cout << "insert | count: " << count << " | ones: " << ones << endl;

    if (tupleLineal) {
      start = chrono::system_clock::now();
      aux = tupleLineal -> pushBit(actionNumber);
      end = chrono::system_clock::now();
      tupleLinealInsertTime += end - start;
      cout << aux << endl;
    }

    if (tupleInterpolation) {
      start = chrono::system_clock::now();
      aux = tupleInterpolation -> pushBit(actionNumber);
      end = chrono::system_clock::now();
      tupleInterpolationInsertTime += end - start;
      cout << aux << endl;
    }

    if (tupleBinary) {
      start = chrono::system_clock::now();
      aux = tupleBinary -> pushBit(actionNumber);
      end = chrono::system_clock::now();
      tupleBinaryInsertTime += end - start;
      cout << aux << endl;
    }

    if (btreeMap) {
      start = chrono::system_clock::now();
      aux = btreeMap -> pushBit(actionNumber);
      end = chrono::system_clock::now();
      btreeMapInsertTime += end - start;
      cout << aux << endl;
    }

    if (redBlackTree) {
      start = chrono::system_clock::now();
      aux = redBlackTree -> pushBit(actionNumber);
      end = chrono::system_clock::now();
      redBlackTreeInsertTime += end - start;
      cout << aux << endl;
    }

    if (avlMap) {
      start = chrono::system_clock::now();
      aux = avlMap -> pushBit(actionNumber);
      end = chrono::system_clock::now();
      avlMapInsertTime += end - start;
      cout << aux << endl;
    }

    if (xFastMap) {
      start = chrono::system_clock::now();
      aux = xFastMap -> pushBit(actionNumber);
      end = chrono::system_clock::now();
      xFastMapInsertTime += end - start;
      cout << aux << endl;
    }

    if (yFastMap) {
      start = chrono::system_clock::now();
      aux = yFastMap -> pushBit(actionNumber);
      end = chrono::system_clock::now();
      yFastMapInsertTime += end - start;
      cout << aux << endl;
    }

    if (count % 10000 == 0) {
      cout << "register" << endl;
      static bool header = true;
      ofstream results;
      if (header) {
        if (tupleLineal) {
          results.open("tupleLineal.csv", ofstream::trunc);
        }
        if (tupleInterpolation) {
          results.open("tupleInterpolation.csv", ofstream::trunc);
        }
        if (tupleBinary) {
          results.open("tupleBinary.csv", ofstream::trunc);
        }
        if (btreeMap) {
          results.open("btreeMap.csv", ofstream::trunc);
        }
        if (redBlackTree) {
          results.open("redBlackTree.csv", ofstream::trunc);
        }
        if (avlMap) {
          results.open("avlMap.csv", ofstream::trunc);
        }
        if (xFastMap) {
          results.open("xFastMap.csv", ofstream::trunc);
        }
        if (yFastMap) {
          results.open("yFastMap.csv", ofstream::trunc);
        }
        results << "confID;Ones;Size;Selects;Ranks;Insert Time;Random Select Time;Random Rank Time" << endl;
        header = false;
        results.close();
      }
      if (tupleLineal) {
        results.open("tupleLineal.csv", ofstream::app);
        results << confId << ";" << ones << ";" << tupleLineal -> size() << ";" << selects << ";" << ranks << ";"
                << tupleLinealInsertTime.count() << ";" << tupleLinealSelectTime.count() << ";" << tupleLinealRankTime.count() << endl;
      }
      if (tupleInterpolation) {
        results.open("tupleInterpolation.csv", ofstream::app);
        results << confId << ";" << ones << ";" << tupleInterpolation -> size() << ";" << selects << ";" << ranks << ";"
                << tupleInterpolationInsertTime.count() << ";" << tupleInterpolationSelectTime.count() << ";" << tupleInterpolationRankTime.count() << endl;
      }
      if (tupleBinary) {
        results.open("tupleBinary.csv", ofstream::app);
        results << confId << ";" << ones << ";" << tupleBinary -> size() << ";" << selects << ";" << ranks << ";"
                << tupleBinaryInsertTime.count() << ";" << tupleBinarySelectTime.count() << ";" << tupleBinaryRankTime.count() << endl;
      }
      if (btreeMap) {
        results.open("btreeMap.csv", ofstream::app);
        results << confId << ";" << ones << ";" << btreeMap -> size() << ";" << selects << ";" << ranks << ";"
                << btreeMapInsertTime.count() << ";" << btreeMapSelectTime.count() << ";" << btreeMapRankTime.count() << endl;
      }
      if (redBlackTree) {
        results.open("redBlackTree.csv", ofstream::app);
        results << confId << ";" << ones << ";" << redBlackTree -> size() << ";" << selects << ";" << ranks << ";"
                << redBlackTreeInsertTime.count() << ";" << redBlackTreeSelectTime.count() << ";" << redBlackTreeRankTime.count() << endl;
      }
      if (avlMap) {
        results.open("avlMap.csv", ofstream::app);
        results << confId << ";" << ones << ";" << avlMap -> size() << ";" << selects << ";" << ranks << ";"
                << avlMapInsertTime.count() << ";" << avlMapSelectTime.count() << ";" << avlMapRankTime.count() << endl;
      }
      if (xFastMap) {
        results.open("xFastMap.csv", ofstream::app);
        results << confId << ";" << ones << ";" << xFastMap -> size() << ";" << selects << ";" << ranks << ";"
                << xFastMapInsertTime.count() << ";" << xFastMapSelectTime.count() << ";" << xFastMapRankTime.count() << endl;
      }
      if (yFastMap) {
        results.open("yFastMap.csv", ofstream::app);
        results << confId << ";" << ones << ";" << yFastMap -> size() << ";" << selects << ";" << ranks << ";"
                << yFastMapInsertTime.count() << ";" << yFastMapSelectTime.count() << ";" << yFastMapRankTime.count() << endl;
      }
      results.close();
    }
  } else if (action == 's') {
    uint32_t aux;
    uint32_t aux2;

    selects += 1;
    cout << "select: " << actionNumber << endl;

    if (tupleLineal) {
      start = chrono::system_clock::now();
      aux = tupleLineal -> select1(actionNumber,aux2);
      end = chrono::system_clock::now();
      tupleLinealSelectTime += end - start;
      cout << aux << endl;
    }

    if (tupleInterpolation) {
      start = chrono::system_clock::now();
      aux = tupleInterpolation -> select1(actionNumber,aux2);
      end = chrono::system_clock::now();
      tupleInterpolationSelectTime += end - start;
      cout << aux << endl;
    }

    if (tupleBinary) {
      start = chrono::system_clock::now();
      aux = tupleBinary -> select1(actionNumber,aux2);
      end = chrono::system_clock::now();
      tupleBinarySelectTime += end - start;
      cout << aux << endl;
    }

    if (btreeMap) {
      start = chrono::system_clock::now();
      aux = btreeMap -> select1(actionNumber,aux2);
      end = chrono::system_clock::now();
      btreeMapSelectTime += end - start;
      cout << aux << endl;
    }

    if (redBlackTree) {
      start = chrono::system_clock::now();
      aux = redBlackTree -> select1(actionNumber,aux2);
      end = chrono::system_clock::now();
      redBlackTreeSelectTime += end - start;
      cout << aux << endl;
    }

    if (avlMap) {
      start = chrono::system_clock::now();
      aux = avlMap -> select1(actionNumber,aux2);
      end = chrono::system_clock::now();
      avlMapSelectTime += end - start;
      cout << aux << endl;
    }

    if (xFastMap) {
      start = chrono::system_clock::now();
      aux = xFastMap -> select1(actionNumber,aux2);
      end = chrono::system_clock::now();
      xFastMapSelectTime += end - start;
      cout << aux << endl;
    }

    if (yFastMap) {
      start = chrono::system_clock::now();
      aux = yFastMap -> select1(actionNumber,aux2);
      end = chrono::system_clock::now();
      yFastMapSelectTime += end - start;
      cout << aux << endl;
    }
  } else {

    uint32_t aux;

    ranks += 1;
    cout << "rank: " << actionNumber << endl;

    if (tupleLineal) {
      start = chrono::system_clock::now();
      aux = tupleLineal -> rank1(actionNumber);
      end = chrono::system_clock::now();
      tupleLinealRankTime += end - start;
      cout << aux << endl;
    }

    if (tupleInterpolation) {
      start = chrono::system_clock::now();
      aux = tupleInterpolation -> rank1(actionNumber);
      end = chrono::system_clock::now();
      tupleInterpolationRankTime += end - start;
      cout << aux << endl;
    }

    if (tupleBinary) {
      start = chrono::system_clock::now();
      aux = tupleBinary -> rank1(actionNumber);
      end = chrono::system_clock::now();
      tupleBinaryRankTime += end - start;
      cout << aux << endl;
    }

    if (btreeMap) {
      start = chrono::system_clock::now();
      aux = btreeMap -> rank1(actionNumber);
      end = chrono::system_clock::now();
      btreeMapRankTime += end - start;
      cout << aux << endl;
    }

    if (redBlackTree) {
      start = chrono::system_clock::now();
      aux = redBlackTree -> rank1(actionNumber);
      end = chrono::system_clock::now();
      redBlackTreeRankTime += end - start;
      cout << aux << endl;
    }

    if (avlMap) {
      start = chrono::system_clock::now();
      aux = avlMap -> rank1(actionNumber);
      end = chrono::system_clock::now();
      avlMapRankTime += end - start;
      cout << aux << endl;
    }

    if (xFastMap) {
      start = chrono::system_clock::now();
      aux = xFastMap -> rank1(actionNumber);
      end = chrono::system_clock::now();
      xFastMapRankTime += end - start;
      cout << aux << endl;
    }

    if (yFastMap) {
      start = chrono::system_clock::now();
      aux = yFastMap -> rank1(actionNumber);
      end = chrono::system_clock::now();
      yFastMapRankTime += end - start;
      cout << aux << endl;
    }
  }
};
