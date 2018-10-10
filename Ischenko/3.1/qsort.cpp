#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <malloc.h>

using namespace std;
int mas[2000000];

void quicksort(/*int *mas,*/ int start, int end, int N) {
	int i = 0;
	double med;
	int s = start, e = end;
	if (N >= 3) {
		med = double(mas[start] + mas[end] + mas[(start + end) / 2]) / 3;
	}
	else {
		med = double(mas[start] + mas[end]) / 2;
	}

	if (start < end) {
		while (true) {
			while (mas[e] >= med) {
				e--;
			}
			while (mas[s] < med) {
				s++;
			}

			if (e <= s)
				break;
			if (mas[s] > mas[e]) {
				int d = mas[s];
				mas[s] = mas[e];
				mas[e] = d;
				e--;
				s++;
			}

		}
		if (s - 1 == end) {
			s--;
		}
		quicksort(/*mas,*/ start, s - 1, s - start);
		if (s == start) {
			s++;
		}
		quicksort(/*mas,*/ s, end, end - s);
	}

}

int main() {

	int N = 0;
	//int * mas = (int*)malloc(N * sizeof(int));
	
	ifstream fin("in.txt");
	ofstream fon("out.txt");
	fin >> N;
	if (N == 0) {
		fon << " ";
		return 0;
	}
	if (N == 1) {
		fin >> N;
		fon << N;
		return 0;
	}
	for (int i = 0; i < N; ++i) {
		fin >> mas[i];
		
	}
	quicksort(/*mas,*/ 0, N - 1, N);
	for (int i = 0; i < N; ++i) {
		fon << mas[i] << " ";
	}
	fon.close();
	return 0;
}