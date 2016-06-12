#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>


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

std::tuple<int, int> partition(std::vector<int>& vect, int low, int high) {
	int left_read_pos = low;
	int right_read_pos = high;
	int pivot = vect[(low + high) / 2];

	while (left_read_pos <= right_read_pos) {
		while (vect[left_read_pos] < pivot && left_read_pos <= right_read_pos) { left_read_pos++; };
		while (pivot < vect[right_read_pos] && right_read_pos >= 0) { right_read_pos--; };

		if (left_read_pos <= right_read_pos) {
			std::swap(vect[left_read_pos++], vect[right_read_pos--]);
		}
	}

	return std::tuple<int, int>(left_read_pos, right_read_pos);
}

void knuthShuffle(std::vector<int>& vect) {
	int subArraySize = vect.size();

	for (int i = 0; i < vect.size(); i++, subArraySize--) {
		int randIndex = std::rand() % subArraySize;

		if (i != randIndex) {
			std::swap(vect[i], vect[randIndex]);
		}
	}
}

int findOrderStatisticRecursive(std::vector<int>& vect, int k, int low, int high) {
	if (high <= low) {
		return vect[low];
	}

	std::tuple<int, int> pivot_pos = partition(vect, low, high);

	if (std::get<0>(pivot_pos) < k - 1) {
		findOrderStatisticRecursive(vect, k, std::get<0>(pivot_pos), high);
	}
	else {
		findOrderStatisticRecursive(vect, k, low, std::get<1>(pivot_pos));
	}
}

bool canFeedHamstrs(std::vector<int>& hamstrFoodRate, std::vector<int>& hamstrFoodGreed,
	std::vector<int>& hamstrFoodTotal, int foodSupply, int count) {

	for (int i = 0; i < hamstrFoodRate.size(); i++) {
		hamstrFoodTotal[i] = hamstrFoodRate[i] + (count - 1) * hamstrFoodGreed[i];
	}

	knuthShuffle(hamstrFoodTotal);
	findOrderStatisticRecursive(hamstrFoodTotal, count, 0, hamstrFoodTotal.size() - 1);

	unsigned long long sum = 0;
	for (int i = 0; i < count; i++) {
		sum += hamstrFoodTotal[i];
	}

	return sum <= foodSupply;
}

int solve(std::vector<int>& hamstrFoodRate, std::vector<int>& hamstrFoodGreed,
	std::vector<int>& hamstrFoodTotal, int foodSupply, int left, int right) {


	while (right - left > 1) {
		int currentHamstrCount = right - (right - left) / 2;
		bool canFeedCurrent = canFeedHamstrs(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, foodSupply, currentHamstrCount);

		if (canFeedCurrent) {
			left = currentHamstrCount;
		}
		else {
			right = currentHamstrCount;
		}
	}

	return left;
}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {
	std::vector<int> hamstrFoodTotal(inputData.hamstrCount);
	int currentHamstrCount = solve(inputData.hamstrFoodRate, inputData.hamstrFoodGreed, hamstrFoodTotal, inputData.foodSupply, 0, inputData.hamstrCount);

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
