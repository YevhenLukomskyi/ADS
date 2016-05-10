#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct HAMSTR {
	int foodRate;
	int foodGreed;
	int foodTotal;
};

struct INPUT_DATA {
	int foodSupplay;
	int hamstrCount;
	std::vector<HAMSTR> hamstrs;
};

struct OUTPUT_DATA {
	double maxHamstrCount;
};

struct INPUT_DATA readInput(std::string fileName) {
	std::ifstream inputFile(fileName, std::ifstream::in);

	auto is_open = inputFile.is_open();

	struct INPUT_DATA inputData;

	inputFile >> inputData.foodSupplay;
	inputFile >> inputData.hamstrCount;

	inputData.hamstrs = std::vector<struct HAMSTR>(inputData.hamstrCount);
	int inputHamstrFoodRate, inputHamstrFoodGreed;

	for (int i = 0; i < inputData.hamstrCount; i++) {
		inputFile >> inputHamstrFoodRate >> inputHamstrFoodGreed;
		inputData.hamstrs[i].foodRate = inputHamstrFoodRate;
		inputData.hamstrs[i].foodGreed = inputHamstrFoodGreed;
		inputData.hamstrs[i].foodTotal = inputHamstrFoodRate;
	}

	return inputData;
}

int getSum(std::vector<struct HAMSTR>& vect, int n) {
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum += vect[i].foodTotal;
	}
	return sum;
}

void merge(std::vector<struct HAMSTR> &array, std::vector<struct HAMSTR> &aux, int left, int middle, int right) {
	// Sedgewick's implementation of 'merge'.

	// Back up array items to 'aux'.
	for (int k = left; k <= right; k++) {
		aux[k] = array[k];
	}

	// Merge 'aux' back to 'array'.
	int i = left, j = middle + 1;
	for (int k = left; k <= right; k++) {
		if (i > middle)            array[k] = aux[j++];
		else if (j > right)        array[k] = aux[i++];
		else if (aux[j].foodTotal > aux[i].foodTotal)  array[k] = aux[j++];
		else                       array[k] = aux[i++];
	}
}

void mergeSortBottomUp(std::vector<struct HAMSTR> &array) {
	int n = (int)array.size();
	std::vector<struct HAMSTR> aux(n);

	for (int stride = 1; stride < n; stride *= 2) {
		for (int i = 0; i < n - stride; i += stride * 2) {
			int left = i;
			int middle = i + stride - 1;
			int right = std::min(i + stride * 2 - 1, n - 1);
			merge(array, aux, left, middle, right);
		}
	}
}

void insertionSort(std::vector<struct HAMSTR> &array) {
	for (int i = 0; i < array.size(); i++) {
		int j = i - 1;

		while (j > 0 && array[j].foodTotal > array[i].foodTotal) {
			std::swap(array[i], array[j]);
		}
	}
}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {

	auto hamstrs = inputData.hamstrs;
	int currentHamstrCount = 1;
	int currentConsumedFood = 0;

	while (true) {
		for (int i = 0; i < inputData.hamstrCount; i++) {
			hamstrs[i].foodTotal = hamstrs[i].foodRate +
				(currentHamstrCount - 1) * hamstrs[i].foodGreed;
		}

		if (currentHamstrCount < 2) {
			mergeSortBottomUp(hamstrs);
		}
		else {
			insertionSort(hamstrs);
		}

		currentConsumedFood = getSum(hamstrs, currentHamstrCount);

		if (currentConsumedFood > inputData.foodSupplay) {
			currentHamstrCount--;
			break;
		}

		if (currentHamstrCount == inputData.hamstrCount) {
			break;
		}

		currentHamstrCount++;
	}

	struct OUTPUT_DATA outputData;
	outputData.maxHamstrCount = currentHamstrCount;

	return outputData;
}

void writeOutput(std::string fileName, struct OUTPUT_DATA outputData) {
	std::ofstream outputFile(fileName, std::ifstream::out);
	outputFile << outputData.maxHamstrCount;
	outputFile.close();
}

int main(int argc, const char *argv[]) {
	
	auto inputFileName = argc == 1 ? "hamstr.in" : argv[1];
	auto outputFileName = argc == 1 ? "hamstr.out" : argv[2];

	auto inputData = readInput(inputFileName);
	auto outputData = solve(inputData);
	writeOutput(outputFileName, outputData);

	return 0;
}
