#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>


struct INPUT_DATA {
	int count;
	int width;
	int height;
};

struct OUTPUT_DATA {
	int squareLen;
};

struct INPUT_DATA readInput(std::string fileName) {
	std::ifstream inputFile(fileName, std::ifstream::in);

	struct INPUT_DATA inputData;
	inputFile >> inputData.count;
	inputFile >> inputData.width;
	inputFile >> inputData.height;

	return inputData;
}

void writeOutput(std::string fileName, struct OUTPUT_DATA outputData) {
	std::ofstream outputFile(fileName, std::ifstream::out);
	outputFile << outputData.squareLen;
	outputFile.close();
}

bool canHostAllCards(int squareSideLen, struct INPUT_DATA inputData) {
	int currentCapacity = (double)squareSideLen / inputData.height * (double)squareSideLen / inputData.width;
	return currentCapacity >= inputData.count;
}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {
	struct OUTPUT_DATA outputData;

	int left = 0;
	int max_side_len = inputData.width > inputData.height ? inputData.width : inputData.height;
	int right = max_side_len * inputData.count;

	while (right - left > 1) {
		int current = left + (right - left) / 2;
		if (!canHostAllCards(current, inputData)) {
			left = current;
		}
		else {
			right = current;
		}
	}

	outputData.squareLen = right;

	return outputData;
}

int main(int argc, const char *argv[])
{
	//clock_t begin = clock();

	auto inputFileName = argc == 1 ? "bugtrk.in" : argv[1];
	auto outputFileName = argc == 1 ? "bugtrk.out" : argv[2];

	auto inputData = readInput(inputFileName);
	auto outputData = solve(inputData);
	writeOutput(outputFileName, outputData);

	//clock_t end = clock();
	//double elapsed_ms = double(end - begin) / CLOCKS_PER_SEC * 1000;

	//std::cout << elapsed_ms;

	//std::getchar();

	return 0;
}
