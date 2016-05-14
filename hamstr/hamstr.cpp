#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


struct INPUT_DATA {
	int foodSupply;
	int hamstrCount;
	std::vector<int> hamstrFoodRate;
	std::vector<int> hamstrFoodGreed;
};

struct OUTPUT_DATA {
	double maxHamstrCount;
};


struct INPUT_DATA readInput(std::string fileName) {
	std::ifstream inputFile(fileName, std::ifstream::in);

	struct INPUT_DATA inputData;

	inputFile >> inputData.foodSupply;
	inputFile >> inputData.hamstrCount;

	inputData.hamstrFoodRate = std::vector<int>(inputData.hamstrCount);
	inputData.hamstrFoodGreed = std::vector<int>(inputData.hamstrCount);
	int inputHamstrFoodRate, inputHamstrFoodGreed;

	for (int i = 0; i < inputData.hamstrCount; i++) {
		inputFile >> inputHamstrFoodRate >> inputHamstrFoodGreed;
		inputData.hamstrFoodRate[i] = inputHamstrFoodRate;
		inputData.hamstrFoodGreed[i] = inputHamstrFoodGreed;
	}

	return inputData;
}

int getSum(std::vector<int>& vect, int n) {
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum += vect[i];
	}
	return sum;
}

int partition(std::vector<int>& vect, int low, int high) {
	int left = std::min(low + 1, high);
	int right = high;

	int pivot = vect[low];

	while (true) {
		while (vect[left] <= pivot) {
			if (left == high) {
				break;
			}
			left++;
		}

		while (pivot <= vect[right]) {
			if (right == low) {
				break;
			}
			right--;
		}

		if (left >= right) {
			break;
		}

		std::swap(vect[left], vect[right]);
	}

	std::swap(vect[low], vect[right]);
	return right;
}

int findOrderStatisticRecursive(std::vector<int>& vect, int k, int low, int high) {
	if (high < low) {
		return vect[low];
	}

	int pivot_pos = partition(vect, low, high);

	if (pivot_pos < k - 1) {
		findOrderStatisticRecursive(vect, k, pivot_pos + 1, high);
	}
	else {
		findOrderStatisticRecursive(vect, k, low, pivot_pos - 1);
	}
}

int getConsumedFood(std::vector<int>& hamstrFoodRate, std::vector<int>& hamstrFoodGreed,
	std::vector<int>& hamstrFoodTotal, int count) {

	for (int i = 0; i < hamstrFoodRate.size(); i++) {
		hamstrFoodTotal[i] = hamstrFoodRate[i] + (count - 1) * hamstrFoodGreed[i];
	}

	findOrderStatisticRecursive(hamstrFoodTotal, count, 0, hamstrFoodTotal.size() - 1);

	int currentConsumedFood = getSum(hamstrFoodTotal, count);
	return currentConsumedFood;
}

int solveRecurse(std::vector<int>& hamstrFoodRate, std::vector<int>& hamstrFoodGreed,
	std::vector<int>& hamstrFoodTotal, int foodSupply, int left, int right) {

	int currentHamstrCount = left + (right - left) / 2;

	int consumedFood1 = getConsumedFood(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, currentHamstrCount);
	int consumedFood2 = getConsumedFood(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, currentHamstrCount + 1);

	if (consumedFood1 > 0 && consumedFood2 > 0 && consumedFood1 <= foodSupply && consumedFood2 > foodSupply) {
		return currentHamstrCount;
	}
	else {
		if (consumedFood1 < 0 || consumedFood2 < 0 || consumedFood1 > foodSupply) {
			solveRecurse(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, foodSupply, left, currentHamstrCount - 1);
		}
		else {
			solveRecurse(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, foodSupply, currentHamstrCount + 1, right);
		}
	}

}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {

	std::vector<int> hamstrFoodTotal(inputData.hamstrCount);

	int currentHamstrCount = solveRecurse(inputData.hamstrFoodRate, inputData.hamstrFoodGreed, hamstrFoodTotal, inputData.foodSupply, 0, inputData.hamstrCount);

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
