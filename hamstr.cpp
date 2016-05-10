// hamstr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;


struct INPUT_DATA {
	int foodSupplay;
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

	inputFile >> inputData.foodSupplay;
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

std::tuple<int, int> partition3(std::vector<int> vect, int low, int high) {
	int left, eq_left;
	int right, eq_right;

	left = eq_left = low;
	right = eq_right = high - 1;

	int pivot = vect[high];

	while (true) {
		while (pivot > vect[left]) {
			if (left == high) {
				break;
			}
			left++;
		}

		while (pivot < vect[right]) {
			if (right == low) {
				break;
			}
			right--;
		}

		if (left >= right) {
			break;
		}

		if (eq_left == high) {
			break;
		}

		if (eq_right < 0) {
			break;
		}

		if (vect[left] == pivot) {
			std::swap(vect[eq_left], vect[left]);
			eq_left++;
		}
		else if (vect[right] == pivot) {
			std::swap(vect[eq_right], vect[right]);
			eq_right--;
		}
		else {
			std::swap(vect[left], vect[right]);
		}
	}

	std::swap(vect[high], vect[left]);

	left = right - 1;
	for (int t = 0; t > eq_left; t++) {
		std::swap(vect[t], vect[left]);
		left--;
	}

	right = left + 1;
	for (int t = high - 1; t > eq_right; t--) {
		std::swap(vect[t], vect[right]);
		right++;
	}

	return std::tuple<int, int>{ left, right };
}

int findOrderStatisticRecursive3(std::vector<int>& vect, int k, int low, int high) {
	auto range = partition3(vect, low, high);
	int left = std::get<0>(range);
	int right = std::get<1>(range);

	if (left > right || left == right) {
		return vect[k - 1];
	}

	if (left <= k - 1 && right >= k - 1) {
		return vect[left];
	}

	if (left < k) {
		findOrderStatisticRecursive3(vect, k, right, high);
	}
	else {
		findOrderStatisticRecursive3(vect, k, low, left);
	}
}

void sortPartial(std::vector<int>& vect, int k) {
	findOrderStatisticRecursive3(vect, k, 0, vect.size() - 1);
}

int getConsumedFood(std::vector<int>& hamstrFoodRate, std::vector<int>& hamstrFoodGreed,
	std::vector<int>& hamstrFoodTotal, int count) {

	for (int i = 0; i < hamstrFoodRate.size(); i++) {
		hamstrFoodTotal[i] = hamstrFoodRate[i] + (count - 1) * hamstrFoodGreed[i];
	}

	sortPartial(hamstrFoodTotal, count);

	int currentConsumedFood = getSum(hamstrFoodTotal, count);
	return currentConsumedFood;
}

int solveRecurse(std::vector<int>& hamstrFoodRate, std::vector<int>& hamstrFoodGreed,
	std::vector<int>& hamstrFoodTotal, int foodSupplay, int left, int right) {

	int currentHamstrCount = left + (right - left) / 2;

	if (currentHamstrCount > right - 1) {
		return hamstrFoodTotal.size();
	}

	int consumedFood1 = getConsumedFood(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, currentHamstrCount);
	int consumedFood2 = getConsumedFood(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, currentHamstrCount + 1);

	if (consumedFood1 > 0 && consumedFood2 > 0 && consumedFood1 <= foodSupplay && consumedFood2 > foodSupplay) {
		return currentHamstrCount;
	}
	else {
		if (consumedFood1 < 0 || consumedFood2 < 0 || consumedFood1 > foodSupplay) {
			solveRecurse(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, foodSupplay, left, currentHamstrCount - 1);
		}
		else {
			solveRecurse(hamstrFoodRate, hamstrFoodGreed, hamstrFoodTotal, foodSupplay, currentHamstrCount + 1, right);
		}
	}
}

struct OUTPUT_DATA solve(struct INPUT_DATA inputData) {

	std::vector<int> hamstrFoodTotal(inputData.hamstrCount);

	int currentHamstrCount = solveRecurse(inputData.hamstrFoodRate, inputData.hamstrFoodGreed, hamstrFoodTotal, inputData.foodSupplay, 0, inputData.hamstrCount);

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

//
//
//
///* This function partitions a[] in three parts
//a) a[l..i] contains all elements smaller than pivot
//b) a[i+1..j-1] contains all occurrences of pivot
//c) a[j..r] contains all elements greater than pivot */
//void partition(int a[], int l, int r, int &i, int &j)
//{
//	i = l - 1, j = r;
//	int p = l - 1, q = r;
//	int v = a[r];
//
//	while (true)
//	{
//		// From left, find the first element greater than
//		// or equal to v. This loop will definitely terminate
//		// as v is last element
//		while (a[++i] < v);
//
//		// From right, find the first element smaller than or
//		// equal to v
//		while (v < a[--j])
//			if (j == l)
//				break;
//
//		// If i and j cross, then we are done
//		if (i >= j) break;
//
//		// Swap, so that smaller goes on left greater goes on right
//		swap(a[i], a[j]);
//
//		// Move all same left occurrence of pivot to beginning of
//		// array and keep count using p
//		if (a[i] == v)
//		{
//			p++;
//			swap(a[p], a[i]);
//		}
//
//		// Move all same right occurrence of pivot to end of array
//		// and keep count using q
//		if (a[j] == v)
//		{
//			q--;
//			swap(a[j], a[q]);
//		}
//	}
//
//	// Move pivot element to its correct index
//	swap(a[i], a[r]);
//
//	// Move all left same occurrences from beginning
//	// to adjacent to arr[i]
//	j = i - 1;
//	for (int k = l; k < p; k++, j--)
//		swap(a[k], a[j]);
//
//	// Move all right same occurrences from end
//	// to adjacent to arr[i]
//	i = i + 1;
//	for (int k = r - 1; k > q; k--, i++)
//		swap(a[i], a[k]);
//}
//
//// 3-way partition based quick sort
//void quicksort(int a[], int l, int r)
//{
//	if (r <= l) return;
//
//	int i, j;
//
//	// Note that i and j are passed as reference
//	partition(a, l, r, i, j);
//
//	// Recur
//	quicksort(a, l, j);
//	quicksort(a, i, r);
//}
//
//// A utility function to print an array
//void printarr(int a[], int n)
//{
//	for (int i = 0; i < n; ++i)
//		printf("%d  ", a[i]);
//	printf("\n");
//}
//
//// Driver program
//int main()
//{
//	int a[] = { 1, 2, 3 };
//	int size = sizeof(a) / sizeof(int);
//	printarr(a, size);
//	quicksort(a, 0, size - 1);
//	printarr(a, size);
//	return 0;
//}