// sigkey.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <ctime>

const std::string ENG_ALPHABET = "abcdefghijklmnopqrstuvwxyz";


struct INPUT_DATA {
	int keysCount;
	std::vector<std::string> keys;
};

struct OUTPUT_DATA {
	int keyPairsCount;
};

struct INPUT_DATA readInput(std::string fileName) {
	std::ifstream inputFile(fileName, std::ifstream::in);

	struct INPUT_DATA inputData;
	inputFile >> inputData.keysCount;

	inputFile.ignore();

	inputData.keys = std::vector<std::string>(inputData.keysCount);
	for (int i = 0; i < inputData.keysCount; i++) {
		std::getline(inputFile, inputData.keys[i]);	
	}

	return inputData;
}

void writeOutput(std::string fileName, struct OUTPUT_DATA outputData) {
	std::ofstream outputFile(fileName, std::ifstream::out);
	outputFile << outputData.keyPairsCount;
	outputFile.close();
}

int getKeyHashCode(std::string key) {
	int hashCode = 0;

	for (int i = 0; i < key.length(); i++) {
		hashCode += (int)key[i];
	}

	return hashCode;
}

std::vector<int> getValidHashCodesVector(){
	std::vector<int> validHashCodes = std::vector<int>(ENG_ALPHABET.length());

	for (int i = 0; i < ENG_ALPHABET.size(); i++) {
		std::string validKeysPair = ENG_ALPHABET.substr(0, i + 1);
		int validKeysPairHashCode = getKeyHashCode(validKeysPair);
		validHashCodes[i] = validKeysPairHashCode;
	}

	return validHashCodes;
}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {
	struct OUTPUT_DATA outputData;
	outputData.keyPairsCount = 0;

	std::vector<int> validHashCodes = getValidHashCodesVector();

	std::vector<std::string> keysWithA = std::vector<std::string>();
	std::vector<std::string> keysWithoutA = std::vector<std::string>();

	for (int i = 0; i < inputData.keysCount; i++) {
		auto key = inputData.keys[i];
		if (key.find("a") != key.npos) {
			keysWithA.push_back(key);
		}else{
			keysWithoutA.push_back(key);
		}
	}

	for (int i = 0; i < keysWithA.size(); i++) {
		for (int j = 0; j < keysWithoutA.size(); j++) {
			std::string key1 = keysWithA[i];
			std::string key2 = keysWithoutA[j];
			int key1HashCode = getKeyHashCode(key1);
			int key2HashCode = getKeyHashCode(key2);

			int keyPairHashCode = key1HashCode + key2HashCode;
			int keyPairLen = key1.length() + key2.length();

			if (validHashCodes[keyPairLen - 1] == keyPairHashCode) {
				auto expectedSymbols = ENG_ALPHABET.substr(0, keyPairLen);
				auto keyPair = key1 + key2;

				if (expectedSymbols.find_first_not_of(keyPair) == keyPair.npos) {
					outputData.keyPairsCount++;
				}
			}
		}
	}

	return outputData;
}

int main(int argc, const char *argv[])
{
	clock_t begin = clock();

	auto inputFileName = argc == 1 ? "sigkey.in" : argv[1];
	auto outputFileName = argc == 1 ? "sigkey.out" : argv[2];

	auto inputData = readInput(inputFileName);
	auto outputData = solve(inputData);
	writeOutput(outputFileName, outputData);

	clock_t end = clock();
	double elapsed_ms = double(end - begin) / CLOCKS_PER_SEC * 1000;

	std::cout << elapsed_ms;

	std::getchar();

	return 0;
}