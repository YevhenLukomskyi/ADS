#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <ctime>
#include <iomanip>

//merge sort
void copy_merge_results(std::vector<int>& vector, std::vector<int>& merge_results, int left, int right) {
	for (int i = left; i <= right; i++) {
		vector[i] = merge_results[i];
	}
}

void merge(std::vector<int>& vector, int left, int middle, int right, std::vector<int>& merge_result) {
	int left_read_index = left;
	int right_read_index = middle + 1;
	int result_write_index = left;

	while (left_read_index <= middle && right_read_index <= right) {
		if (vector[left_read_index] > vector[right_read_index]) {
			merge_result[result_write_index] = vector[left_read_index];
			left_read_index++;
		}
		else {
			merge_result[result_write_index] = vector[right_read_index];
			right_read_index++;
		}
		result_write_index++;
	}

	if (left_read_index >= middle) {
		for (int i = right_read_index; i <= right; i++) {
			merge_result[result_write_index] = vector[i];
			result_write_index++;
		}
	}

	if (right_read_index >= right) {
		for (int i = left_read_index; i <= middle; i++) {
			merge_result[result_write_index] = vector[i];
			result_write_index++;
		}
	}

	copy_merge_results(vector, merge_result, left, right);
}

void merge_recursive(std::vector<int>& vector, int left, int right, std::vector<int>& merge_result) {
	if (left < right) {
		int middle = (left + right) / 2;
		merge_recursive(vector, left, middle, merge_result);
		merge_recursive(vector, middle + 1, right, merge_result);
		merge(vector, left, middle, right, merge_result);
	}
}

void merge_sort(std::vector<int>& vector) {
	std::vector<int> merge_result(vector.size());
	merge_recursive(vector, 0, vector.size() - 1, merge_result);
}

//Radix sort
void copy_vector(std::vector<int>& src_vector, std::vector<int>& dest_vector, int src_start_index, int dest_start_index, int len) {
	for (int i = src_start_index; i < src_start_index + len; i++) {
		dest_vector[dest_start_index++] = src_vector[i];
	}
}

void radix_sort(std::vector<int>& vector) {
	std::vector<int> temp_vector(vector.size());

	for (int shift = 31; shift >= 1; shift--) {
		int j = 0;
		int i = 0;
		for (i = 0; i < vector.size(); i++) {
			if (vector[i] << shift) {
				temp_vector[j] = vector[i];
				j++;
			}
			else {
				vector[i - j] = vector[i];
			}
		}
		copy_vector(temp_vector, vector, 0, vector.size() - j, j);
	}
}

int main() {
	std::ifstream infile("08.in");
	std::ofstream outfile("discnt.out");

	std::string sLine;
	getline(infile, sLine);
	std::stringstream iss(sLine);

	int number;
	std::vector<int> prices;
	while (iss >> number)
		prices.push_back(number);

	getline(infile, sLine);
	int discount_percent = std::stoi(sLine);
	double discount = 1 - discount_percent / 100.0;

	radix_sort(prices);

	double sum_total = 0;
	for (int i = 0; i < prices.size() / 3; i++) {
		sum_total += prices[i] * discount;
	}

	for (int i = prices.size() / 3; i < prices.size(); i++) {
		sum_total += prices[i];
	}

	outfile << std::fixed << std::setprecision(2) << sum_total;

	infile.close();
	outfile.close();

	return 0;
}