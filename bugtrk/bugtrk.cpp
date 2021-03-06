#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


struct INPUT_DATA {
	uint64_t count;
	uint64_t width;
	uint64_t height;
};

struct OUTPUT_DATA {
	uint64_t squareSideLen;
};

struct INPUT_DATA readInput(std::string fileName) {
	std::ifstream inputFile(fileName, std::ifstream::in);

	struct INPUT_DATA inputData;
	inputFile >> inputData.count;
	inputFile >> inputData.width;
	inputFile >> inputData.height;

	inputFile.close();
	return inputData;
}

void writeOutput(std::string fileName, struct OUTPUT_DATA outputData) {
	std::ofstream outputFile(fileName, std::ifstream::out);
	outputFile << outputData.squareSideLen;
	outputFile.close();
}

bool canHostAllCards(uint64_t squareSideLen, struct INPUT_DATA inputData) {
	uint64_t cardsPerRow = squareSideLen / inputData.width;
	uint64_t cardsPerCol = squareSideLen / inputData.height;
	uint64_t currentCapacity = cardsPerRow * cardsPerCol;
	return currentCapacity >= inputData.count;
}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {

	uint64_t left = 1;
	uint64_t right = inputData.count * std::max(inputData.height, inputData.width);

	while (right - left > 1) {
		uint64_t current = left + (right - left) / 2;
		if (canHostAllCards(current, inputData)) {
			right = current;
		}
		else {
			left = current;
		}
	}

	struct OUTPUT_DATA outputData;
	outputData.squareSideLen = right;

	return outputData;
}

int main(int argc, const char *argv[])
{
	auto inputFileName = argc == 1 ? "bugtrk.in" : argv[1];
	auto outputFileName = argc == 1 ? "bugtrk.out" : argv[2];

	auto inputData = readInput(inputFileName);
	auto outputData = solve(inputData);
	writeOutput(outputFileName, outputData);

	return 0;
}
