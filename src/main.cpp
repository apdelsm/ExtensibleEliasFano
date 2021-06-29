#include <chrono>
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
#include "../include/x-fast-map.h"
#include "../include/y-fast.h"

using namespace std;
using namespace sdsl;

void manageBit(string testType, uint64_t bufferSize, float probabilitie, int zerosRunSize, int onesRunSize, int test, uint64_t bit, ExtensibleEliasFano<TuplesVector> *tupleLineal, ExtensibleEliasFano<TuplesVector> *tupleInterpolation, ExtensibleEliasFano<TuplesVector> *tupleBinary, ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *btreeMap, ExtensibleEliasFano<RedBlackTree> *redBlackTree, ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *avlMap, ExtensibleEliasFano<x_fast_map<uint64_t>> *xFastMap, ExtensibleEliasFano<y_fast<uint64_t>> *yFastMap, bool clear=false);

int main() {
  const int testCases = 1;
  const int bitsLimit = 100;
  const uint64_t uExp = ceil(log2(bitsLimit));
  uint64_t bit;
  vector<float> probabilities = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
  vector<uint64_t> bufferSizes = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
  srand(2015735042);

  for (vector<uint64_t>::iterator bufferIt = bufferSizes.begin(), bufferEnd = bufferSizes.end(); bufferIt != bufferEnd; ++bufferIt){
    ExtensibleEliasFano<TuplesVector> *tupleLineal;
    ExtensibleEliasFano<TuplesVector> *tupleInterpolation;
    ExtensibleEliasFano<TuplesVector> *tupleBinary;
    ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *btreeMap;
    ExtensibleEliasFano<RedBlackTree> *redBlackTree;
    ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *avlMap;
    ExtensibleEliasFano<x_fast_map<uint64_t>> *xFastMap;
    ExtensibleEliasFano<y_fast<uint64_t>> *yFastMap;
    
    // Random bits
    int randResult;
    for(vector<float>::iterator probabilitieIt = probabilities.begin(), probabilitieEnd = probabilities.end(); probabilitieIt != probabilitieEnd; ++probabilitieIt) {
      int probabilitie = (*probabilitieIt) * 10;
      for (int test = 0; test < testCases; ++test) {

        tupleLineal = new ExtensibleEliasFano<TuplesVector>((*bufferIt), linearSearch);
        tupleInterpolation = new ExtensibleEliasFano<TuplesVector>((*bufferIt), interpolationSearch);
        tupleBinary = new ExtensibleEliasFano<TuplesVector>((*bufferIt), binarySearch);
        btreeMap = new ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>>(*bufferIt);
        redBlackTree = new ExtensibleEliasFano<RedBlackTree>(*bufferIt);
        avlMap = new ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>>(*bufferIt);
        xFastMap = new ExtensibleEliasFano<x_fast_map<uint64_t>>(*bufferIt, uExp);
        yFastMap = new ExtensibleEliasFano<y_fast<uint64_t>>(*bufferIt, uExp);

        for (int bitCount = 0; bitCount < bitsLimit; ++bitCount) {

          randResult = rand() % 10;
          if (randResult < probabilitie) {
            bit = 1;
          } else {
            bit = 0; 
          }
          manageBit("Random", *bufferIt, *probabilitieIt, 1, 1, test, bit, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap);
        }

        manageBit("Random", *bufferIt, *probabilitieIt, 1, 1, test, bit, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap, true);
        delete tupleLineal;
        delete tupleInterpolation;
        delete tupleBinary;
        delete btreeMap;
        delete redBlackTree;
        delete avlMap;
        delete xFastMap;
        delete yFastMap;
      }
    }

    // Runs
    vector<int> zeros = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
    vector<int> ones = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
    int randZeros;
    int randOnes;
    int remainingBits;
    int addOnesRun;

    for (vector<int>::iterator zerosIt = zeros.begin(), zerosEnd = zeros.end(); zerosIt != zerosEnd; ++zerosIt) {
      for (vector<int>::iterator onesIt = ones.begin(), onesEnd = ones.end(); onesIt != onesEnd; ++onesIt) {
        for (vector<float>::iterator probabilitieIt = probabilities.begin(), probabilitieEnd = probabilities.end(); probabilitieIt != probabilitieEnd; ++probabilitieIt) {

          int probabilitie = (*probabilitieIt) * 10;
          remainingBits = bitsLimit;

          for (int test = 0; test < testCases; ++test) {
            tupleLineal = new ExtensibleEliasFano<TuplesVector>((*bufferIt), linearSearch);
            tupleInterpolation = new ExtensibleEliasFano<TuplesVector>((*bufferIt), interpolationSearch);
            tupleBinary = new ExtensibleEliasFano<TuplesVector>((*bufferIt), binarySearch);
            btreeMap = new ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>>(*bufferIt);
            redBlackTree = new ExtensibleEliasFano<RedBlackTree>(*bufferIt);
            avlMap = new ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>>(*bufferIt);
            xFastMap = new ExtensibleEliasFano<x_fast_map<uint64_t>>(*bufferIt, uExp);
            yFastMap = new ExtensibleEliasFano<y_fast<uint64_t>>(*bufferIt, uExp);

            while (remainingBits > 0) {

              randZeros = rand() % (*zerosIt) + 1;
              if (randZeros > remainingBits) {
                randZeros = remainingBits - 1;
              }
              remainingBits -= randZeros + 1;
              for (int zeroCount = 0; zeroCount < randZeros; ++zeroCount) {
                bit = 0;
                manageBit("Run", *bufferIt, *probabilitieIt, *zerosIt, *onesIt, test, bit, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap);
              }
              bit = 1;
              manageBit("Run", *bufferIt, *probabilitieIt, *zerosIt, *onesIt, test, bit, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap);

              addOnesRun = rand() % 10;
              if (addOnesRun < probabilitie) {
                randOnes = rand() % (*onesIt) + 1;
              } else {
                randOnes = 0;
              }
              if (randOnes > remainingBits) {
                randOnes = remainingBits;
              }
              remainingBits -= randOnes;
              for (int oneCount = 0; oneCount < randOnes; ++oneCount) {
                bit = 1;
                manageBit("Run", *bufferIt, *probabilitieIt, *zerosIt, *onesIt, test, bit, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap);
              }
            }
            manageBit("Run", *bufferIt, *probabilitieIt, *zerosIt, *onesIt, test, bit, tupleLineal, tupleInterpolation, tupleBinary, btreeMap, redBlackTree, avlMap, xFastMap, yFastMap, true);
            delete tupleLineal;
            delete tupleInterpolation;
            delete tupleBinary;
            delete btreeMap;
            delete redBlackTree;
            delete avlMap;
            delete xFastMap;
            delete yFastMap;
          }
        }
      }    
    }
  }
  return 0;
}

void manageBit(string testType, uint64_t bufferSize, float probabilitie, int zerosRunSize, int onesRunSize, int test, uint64_t bit, ExtensibleEliasFano<TuplesVector> *tupleLineal, ExtensibleEliasFano<TuplesVector> *tupleInterpolation, ExtensibleEliasFano<TuplesVector> *tupleBinary, ExtensibleEliasFano<btree::btree_map<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *btreeMap, ExtensibleEliasFano<RedBlackTree> *redBlackTree, ExtensibleEliasFano<avl_tree<uint64_t, tuple<uint64_t, uint64_t, sd_vector<>>*>> *avlMap, ExtensibleEliasFano<x_fast_map<uint64_t>> *xFastMap, ExtensibleEliasFano<y_fast<uint64_t>> *yFastMap, bool clear) {
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

  if (clear) {
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
  } else {

    uint64_t aux;
    uint64_t selectResult;
    uint64_t rankResult;
    uint64_t aux2;

    count += 1;

    ones += bit;
    cout << "insert" << endl;
    start = chrono::system_clock::now();
    aux = tupleLineal -> pushBit(bit);
    end = chrono::system_clock::now();
    tupleLinealInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(11);
    }

    start = chrono::system_clock::now();
    aux = tupleInterpolation -> pushBit(bit);
    end = chrono::system_clock::now();
    tupleInterpolationInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(12);
    }

    start = chrono::system_clock::now();
    aux = tupleBinary -> pushBit(bit);
    end = chrono::system_clock::now();
    tupleBinaryInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(13);
    }

    start = chrono::system_clock::now();
    aux = btreeMap -> pushBit(bit);
    end = chrono::system_clock::now();
    btreeMapInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(14);
    }

    start = chrono::system_clock::now();
    aux = redBlackTree -> pushBit(bit);
    end = chrono::system_clock::now();
    redBlackTreeInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(15);
    }

    start = chrono::system_clock::now();
    aux = avlMap -> pushBit(bit);
    end = chrono::system_clock::now();
    avlMapInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(16);
    }

    start = chrono::system_clock::now();
    aux = xFastMap -> pushBit(bit);
    end = chrono::system_clock::now();
    xFastMapInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(17);
    }

    start = chrono::system_clock::now();
    aux = yFastMap -> pushBit(bit);
    end = chrono::system_clock::now();
    yFastMapInsertTime += end - start;
    cout << aux << endl;
    if (aux != count) {
      exit(18);
    }

    int randResult = rand() % 10;
    if (randResult < 1) {
      randResult = rand() % 2;

      if (randResult == 0 && ones > 0) {
        selects += 1;
        uint64_t oneOccurrence = rand() % ones + 1;
        cout << "select: " << oneOccurrence << endl;
        start = chrono::system_clock::now();
        aux = tupleLineal -> select1(oneOccurrence, aux2);
        end = chrono::system_clock::now();
        tupleLinealSelectTime += end - start;
        cout << aux << endl;
        selectResult = aux2;

        start = chrono::system_clock::now();
        aux = tupleInterpolation -> select1(oneOccurrence, aux2);
        end = chrono::system_clock::now();
        tupleInterpolationSelectTime += end - start;
        cout << aux << endl;
        if (selectResult != aux2) {
          exit(21);
        }

        start = chrono::system_clock::now();
        aux = tupleBinary -> select1(oneOccurrence, aux2);
        end = chrono::system_clock::now();
        tupleBinarySelectTime += end - start;
        cout << aux << endl;
        if (selectResult != aux2) {
          exit(22);
        }

        start = chrono::system_clock::now();
        aux = btreeMap -> select1(oneOccurrence, aux2);
        end = chrono::system_clock::now();
        btreeMapSelectTime += end - start;
        cout << aux << endl;
        if (selectResult != aux2) {
          exit(23);
        }

        start = chrono::system_clock::now();
        aux = redBlackTree -> select1(oneOccurrence, aux2);
        end = chrono::system_clock::now();
        redBlackTreeSelectTime += end - start;
        cout << aux << endl;
        if (selectResult != aux2) {
          exit(24);
        }

        start = chrono::system_clock::now();
        aux = avlMap -> select1(oneOccurrence, aux2);
        end = chrono::system_clock::now();
        avlMapSelectTime += end - start;
        cout << aux << endl;
        if (selectResult != aux2) {
          exit(25);
        }

        start = chrono::system_clock::now();
        aux = xFastMap -> select1(oneOccurrence, aux2);
        end = chrono::system_clock::now();
        xFastMapSelectTime += end - start;
        cout << aux << endl;
        if (selectResult != aux2) {
          exit(26);
        }

        start = chrono::system_clock::now();
        aux = yFastMap -> select1(oneOccurrence, aux2);
        end = chrono::system_clock::now();
        yFastMapSelectTime += end - start;
        cout << aux << endl;
        if (selectResult != aux2) {
          exit(27);
        }

      } else {
        ranks += 1;
        uint64_t rankPosition = rand() % count;
        cout << "rank: " << rankPosition << endl;
        start = chrono::system_clock::now();
        aux2 = tupleLineal -> rank1(rankPosition);
        end = chrono::system_clock::now();
        tupleLinealRankTime += end - start;
        cout << aux2 << endl;
        rankResult = aux2;

        start = chrono::system_clock::now();
        aux2 = tupleInterpolation -> rank1(rankPosition);
        end = chrono::system_clock::now();
        tupleInterpolationRankTime += end - start;
        cout << aux2 << endl;
        if (rankResult != aux2) {
          exit(31);
        }

        start = chrono::system_clock::now();
        aux2 = tupleBinary -> rank1(rankPosition);
        end = chrono::system_clock::now();
        tupleBinaryRankTime += end - start;
        cout << aux2 << endl;
        if (rankResult != aux2) {
          exit(32);
        }

        start = chrono::system_clock::now();
        aux = btreeMap -> rank1(rankPosition);
        end = chrono::system_clock::now();
        btreeMapRankTime += end - start;
        cout << aux << endl;
        if (rankResult != aux2) {
          exit(33);
        }

        start = chrono::system_clock::now();
        aux = redBlackTree -> rank1(rankPosition);
        end = chrono::system_clock::now();
        redBlackTreeRankTime += end - start;
        cout << aux << endl;
        if (rankResult != aux2) {
          exit(34);
        }

        start = chrono::system_clock::now();
        aux = avlMap -> rank1(rankPosition);
        end = chrono::system_clock::now();
        avlMapRankTime += end - start;
        cout << aux << endl;
        if (rankResult != aux2) {
          exit(35);
        }

        start = chrono::system_clock::now();
        aux = xFastMap -> rank1(rankPosition);
        end = chrono::system_clock::now();
        xFastMapRankTime += end - start;
        cout << aux << endl;
        if (rankResult != aux2) {
          exit(36);
        }

        start = chrono::system_clock::now();
        aux = yFastMap -> rank1(rankPosition);
        end = chrono::system_clock::now();
        yFastMapRankTime += end - start;
        cout << aux << endl;
        if (rankResult != aux2) {
          exit(37);
        }
      }
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
  }
};
