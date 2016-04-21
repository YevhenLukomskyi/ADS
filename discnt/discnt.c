#define N 10000 + 1

//merge sort
void copy_merge_results(int* array, int* merge_results, int left, int right) {
	for (int i = left; i <= right; i++) {
		array[i] = merge_results[i];
	}
}

void merge(int* array, int left, int middle, int right, int* merge_result) {
	int left_read_index = left;
	int right_read_index = middle + 1;
	int result_write_index = left;

	while (left_read_index <= middle && right_read_index <= right) {
		if (array[left_read_index] > array[right_read_index]) {
			merge_result[result_write_index] = array[left_read_index];
			left_read_index++;
		}
		else {
			merge_result[result_write_index] = array[right_read_index];
			right_read_index++;
		}
		result_write_index++;
	}

	if (left_read_index >= middle) {
		for (int i = right_read_index; i <= right; i++) {
			merge_result[result_write_index] = array[i];
			result_write_index++;
		}
	}

	if (right_read_index >= right) {
		for (int i = left_read_index; i <= middle; i++) {
			merge_result[result_write_index] = array[i];
			result_write_index++;
		}
	}

	copy_merge_results(array, merge_result, left, right);
}

void merge_recursive(int* vector, int left, int right, int* merge_result) {
	if (left < right) {
		int middle = (left + right) >> 1;
		merge_recursive(vector, left, middle, merge_result);
		merge_recursive(vector, middle + 1, right, merge_result);

		if (vector[middle] < vector[middle + 1]) {
			merge(vector, left, middle, right, merge_result);
		}
	}
}

void merge_sort(int* array, int size) {
	int merge_result[N];
	merge_recursive(array, 0, size - 1, merge_result);
}

int main() {
	FILE* file = fopen("discnt.in", "r");
	int array[N];
	int i = 0;

	while (!feof(file))
	{
		fscanf(file, "%d", &array[i]);
		i++;
	}
	fclose(file);

	int size = i - 2;
	double discnt = 1 - array[i - 2] / 100.0;
	double sum_total = 0;

	if (size > 1) {
		merge_sort(array, size);

		for (int i = 0; i < size / 3; i++) {
			sum_total += array[i];
		}
		sum_total *= discnt;

		for (int i = size / 3; i < size; i++) {
			sum_total += array[i];
		}
	}
	else {
		sum_total = array[0];
	}

	file = fopen("discnt.out", "w");
	fprintf(file, "%.2f", sum_total);
	fclose(file);

	return 0;
}

