from enum import Enum
import random
import statistics
import sys

numGenerations = 50
mutateChance = 0.02
operationMutateChance = 0.30
populationSize = 10
maxOperations = 20
fitnessWeights = [0.6, 0.2, 0.2]


class GeneType(Enum):
    BYTE_DIVIDE = 1
    CHUNK_OPS = 2
    HASH_OPS = 3
    REMAINING_OPS = 4
    NEW_HASH_OPS = 5


class Operation:
    def __init__(self):
        self.op = random.randint(0, 7)
        self.value = random.randint(0, sys.maxsize)

    def write(self, outfile):
        outfile.write(str(self.op) + ' ' + str(self.value) + ' ')

    def mutate(self):
        if random.random() < operationMutateChance:
            self.op = random.randint(0, 7)
            self.value = random.randint(0, sys.maxsize)
        return self


class OperationList:
    def __init__(self, size):
        self.operations = [Operation() for i in range(size)]

    def write(self, outfile):
        outfile.write(str(len(self.operations)) + ' ')
        for operation in self.operations:
            operation.write(outfile)
        outfile.write('\n')

    def merge(self, other):
        if len(self.operations) == len(other.operations):
            self.operations = [self.operations[i] if random.randint(0, 1) else other.operations[i] for i in range(len(self.operations))]
        else:
            averageLength = int((len(self.operations) + len(other.operations)) / 2)
            minOperations = [self.operations[i] if random.randint(0, 1) else other.operations[i]
                                 for i in range(min(len(self.operations), len(other.operations)))]
            lengthDifference = averageLength - len(minOperations)
            self.operations = minOperations + (self.operations[-lengthDifference:] if len(self.operations) > len(other.operations) else other.operations[-lengthDifference:])
        return self

    def mutateLength(self):
        mutation = random.randint(-2, 2)
        originalLength = len(self.operations)
        newLength = min(mutation + originalLength, maxOperations) if mutation >= 0 else max(1,
                                                                                            mutation + originalLength)
        if newLength < originalLength:
            self.operations = self.operations[0:newLength - 1]
        else:
            self.operations += [Operation() for i in range(newLength - originalLength)]

    def mutateOperations(self):
        self.operations = [op.mutate() for op in self.operations]

    def mutate(self):
        self.mutateLength()
        self.mutateOperations()


class Chromosome:
    def __init__(self, father=None, mother=None):
        if father and mother:
            self.genes = {
                GeneType.BYTE_DIVIDE: int((father.genes[GeneType.BYTE_DIVIDE] + mother.genes[GeneType.BYTE_DIVIDE]) / 2),
                GeneType.CHUNK_OPS: father.genes[GeneType.CHUNK_OPS].merge(mother.genes[GeneType.CHUNK_OPS]),
                GeneType.HASH_OPS: father.genes[GeneType.HASH_OPS].merge(mother.genes[GeneType.HASH_OPS]),
                GeneType.REMAINING_OPS: father.genes[GeneType.REMAINING_OPS].merge(mother.genes[GeneType.REMAINING_OPS]),
                GeneType.NEW_HASH_OPS: father.genes[GeneType.NEW_HASH_OPS].merge(mother.genes[GeneType.NEW_HASH_OPS]),
            }

            # byte divide mutation
            if random.random() < mutateChance:
                mutation = random.randint(-7, 7)
                original = self.genes[GeneType.BYTE_DIVIDE]
                self.genes[GeneType.BYTE_DIVIDE] += min(original+mutation, 7) if mutation>=0 else max(original+mutation, 0)

            # chunk operations length mutation
            if random.random() < mutateChance:
                self.genes[GeneType.CHUNK_OPS].mutate()
            if random.random() < mutateChance:
                self.genes[GeneType.HASH_OPS].mutate()
            if random.random() < mutateChance:
                self.genes[GeneType.REMAINING_OPS].mutate()
            if random.random() < mutateChance:
                self.genes[GeneType.NEW_HASH_OPS].mutate()

        else:
            self.genes = {
                GeneType.BYTE_DIVIDE: random.randint(1, 10),
                GeneType.CHUNK_OPS: OperationList(random.randint(1, maxOperations)),
                GeneType.HASH_OPS: OperationList(random.randint(1, maxOperations)),
                GeneType.REMAINING_OPS: OperationList(random.randint(1, maxOperations)),
                GeneType.NEW_HASH_OPS: OperationList(random.randint(1, maxOperations)),
            }

    def print(self):
        print(self.genes[GeneType.BYTE_DIVIDE])
        print(self.genes[GeneType.CHUNK_OPS])
        print(self.genes[GeneType.HASH_OPS])
        print(self.genes[GeneType.REMAINING_OPS])
        print(self.genes[GeneType.NEW_HASH_OPS])

    def write(self, outfile):
        outfile.write(str(self.genes[GeneType.BYTE_DIVIDE]) + '\n')
        self.genes[GeneType.CHUNK_OPS].write(outfile)
        self.genes[GeneType.HASH_OPS].write(outfile)
        self.genes[GeneType.REMAINING_OPS].write(outfile)
        self.genes[GeneType.NEW_HASH_OPS].write(outfile)


def createChromosomes(amount):
    return [Chromosome() for i in range(amount)]


def breedChromosomes(chromosomes):
    return [Chromosome(chromosomes[i], chromosomes[i+1]) for i in range(0, len(chromosomes)-1, 2)]


def getResults():
    with open("infile.txt", 'r') as infile:
        results = [[float(x) for x in line.strip().split(' ')] for line in infile.readlines()]
    infile.close()

    return results


def calculateFitness(results):
    return [fitnessWeights[0]*result[0] + fitnessWeights[1]*result[1] + fitnessWeights[2]*result[2] for result in results]


def cullPopulation(population, fitnesses):
    medianFitness = statistics.median(fitnesses)
    remainingPopulation = []
    for i in range(len(population)):
        if fitnesses[i] >= medianFitness:
            remainingPopulation.append(population[i])
    return remainingPopulation


def main():
    population = createChromosomes(populationSize)
    for i in range(2, numGenerations):
        print("Simulating Generation " + str(i+1))
        with open('outfile.txt', 'w') as out:
            for chromosome in population:
                chromosome.write(out)
        out.close()

        # subprocess call here

        nextBredGeneration = breedChromosomes(cullPopulation(population, calculateFitness(getResults())))
        population = nextBredGeneration + createChromosomes(populationSize - len(nextBredGeneration))


if __name__ == "__main__":
    main()
