#include <fstream>
#include <vector>
#include <string>


struct INPUT_DATA {
	int width;
	int height;
	std::vector<std::string> words;
};

struct OUTPUT_DATA {
	long long countOfWays;
};


struct INPUT_DATA readInput(std::string fileName) {
	std::ifstream inputFile(fileName, std::ifstream::in);

	struct INPUT_DATA inputData;
	inputFile >> inputData.width;
	inputFile >> inputData.height;

	inputFile.ignore();

	inputData.words = std::vector<std::string>(inputData.height);
	for (int i = 0; i < inputData.width; i++) {
		std::getline(inputFile, inputData.words[i]);
	}

	inputFile.close();
	return inputData;
}

void writeOutput(std::string fileName, struct OUTPUT_DATA outputData) {
	std::ofstream outputFile(fileName, std::ifstream::out);
	outputFile << outputData.countOfWays;
	outputFile.close();
}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {	
	auto colCount = inputData.width;
	auto rowCount = inputData.height;
	auto words = inputData.words;

	int** ways = new int*[rowCount];
	for (int i = 0; i < rowCount; ++i)
		ways[i] = new int[colCount];

	for (int i = 0; i < rowCount; i++) {
		ways[i][0] = 0;
	}

	for (int j = 1; j < colCount; j++) {
		for (int i = 0; i < rowCount; i++) {
			ways[i][j] = ways[i][j - 1] + 1;

			for (int k = 0; k < rowCount; k++) {
				if (k != i && words[k][j - 1] == words[i][j]) {
					ways[i][j] += ways[k][j - 1];
				}
			}
		}
	}

	struct OUTPUT_DATA outputData;
	outputData.countOfWays = ways[0][colCount - 1] + ways[rowCount - 1][colCount - 1];

	return outputData;
}

int main(int argc, const char *argv[])
{
	auto inputFileName = argc == 1 ? "ijones.in" : argv[1];
	auto outputFileName = argc == 1 ? "ijones.out" : argv[2];

	auto inputData = readInput(inputFileName);
	auto outputData = solve(inputData);
	writeOutput(outputFileName, outputData);
	return 0;
}

