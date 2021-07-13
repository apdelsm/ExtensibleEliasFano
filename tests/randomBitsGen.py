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

testCases = 5
bitLimit = 100000000
probabilities = [10, 20, 30, 40, 50, 60, 70, 80, 90]

random.seed(2015735042)

#randombits
for test in range(testCases):
  for probabilitie in probabilities:
    onesCount = 0
    testFile = open(f'./randomBits/p{probabilitie}t{test}.txt', 'w')
    testFile.write(f'{bitLimit}\n')
    for bitPos in range(0, bitLimit):
      randResult = random.randint(1, 100)
      if randResult <= probabilitie:
        bit = 1
      else:
        bit = 0
      onesCount += bit
      testFile.write(f'{bit}')
      operation(onesCount, bitPos, testFile)
    testFile.close()
    print(f'./randomBits/p{probabilitie}t{test}.txt')
