import random

def operation(ones, writedBits, testFile):
  retList = [False, False, False]
  randResult = random.randint(1, 100)
  if randResult <= 10:
    retList[0] = True
    randResult = random.randint(0, 1)
    if randResult == 0 and ones > 0:
      retList[1] = 's'
      retList[2] = random.randint(1, ones)
    else:
      retList[1] = 'r'
      retList[2] = random.randint(0, writedBits)
  if retList[0]:
    testFile.write(f'{retList[1]}{retList[2]}{retList[1]}')

testCases = 1
bitLimit = 100000000
probabilities = [5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120]
run = [2048, 4096, 8192]

random.seed(201573140)
#randomRuns
for test in range(testCases):
  for runSize in run:
    for probabilitie in probabilities:

      testFile = open(f'./randomRuns/z{runSize}o{runSize}p{probabilitie}t{test}.txt', 'w')
      testFile.write(f'{bitLimit}\n')
      remainingBits = bitLimit
      bitsCount = 0
      onesCount = 0

      while remainingBits > 0:

        randZeros = random.randint(1, runSize)

        if (randZeros > remainingBits):
          randZeros = remainingBits - 1
        remainingBits -= randZeros + 1

        for i in range(randZeros):
          testFile.write('0')
          bitsCount += 1
          operation(onesCount, bitsCount, testFile)

        testFile.write('1')
        bitsCount += 1
        onesCount += 1
        operation(onesCount, bitsCount, testFile)
        
        randOnes = 0
        randResult = random.randint(1, 1000)
        if randResult <= probabilitie:
          randOnes = random.randint(1, runSize)
        if randOnes > remainingBits:
          randOnes = remainingBits
        remainingBits -= randOnes
        for i in range(randOnes):
          testFile.write('1')
          bitsCount += 1
          onesCount += 1
          operation(onesCount, bitsCount, testFile)
      testFile.close()
      print(f'./randomRuns/z{runSize}o{runSize}p{probabilitie}t{test}.txt')