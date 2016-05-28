// bugtrk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


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
	int currentCapacity = squareSideLen / inputData.height  * squareSideLen / inputData.width;
	return currentCapacity >= inputData.count;
}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {
	struct OUTPUT_DATA outputData;

	int left = 0;
	int right = 10e9;

	while (left < right) {
		int current = (left + right + 1) / 2;
		if (!canHostAllCards(current, inputData)) {
			left = current;
		}
		else {
			right = current - 1;
		}
	}

	outputData.squareLen = right;

	return outputData;
}

int main(int argc, const char *argv[])
{
	clock_t begin = clock();

	auto inputFileName = argc == 1 ? "bugtrk.in" : argv[1];
	auto outputFileName = argc == 1 ? "bugtrk.out" : argv[2];

	auto inputData = readInput(inputFileName);
	auto outputData = solve(inputData);
	writeOutput(outputFileName, outputData);

	clock_t end = clock();
	double elapsed_ms = double(end - begin) / CLOCKS_PER_SEC * 1000;

	std::cout << elapsed_ms;

	std::getchar();

	return 0;
}
