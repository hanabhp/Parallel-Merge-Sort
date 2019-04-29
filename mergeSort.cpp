/*
*This code is part of an assignment in CMSC501 to implement Parallel MergeSort algorithm (Intro. To Algorithm, 27.3)
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "omp.h"
using namespace std;

void mergeSort(float* A, int p, int r, float* B, int s);
void merge(float* T, int p1, int r1, int p2, int r2, float* B, int p3);
void swap(float& a, float& b);
float search(float x, float* T, int p, int r);

const int N = 1000000;
const int thread_ = 1;
const int MAX-Partition = 8192;

void swap(float& a, float& b)
{
	float c = a;
	a = b;
	b = c;
}

float search(float x, float* T, int p, int r)
{
	int l = p;
	int h = (p > r + 1 ? p : r + 1);

	while (l < h) {
		int mid = (l + h) / 2;
		if (x <= T[mid]) h = mid;
		else l = mid + 1;
	}

	return h;
}

void mergeSort(float* A, int p, int r, float* B, int s)
{
	int n = r - p + 1;

	if (n == 1) {
		B[s] = A[p];
		return;
	}

	float* T = new float[n];

	int q = (p + r) / 2;
	int qq = q - p;

	if (q - p <= MAX-Partition) {
		mergeSort(A, p, q, T, 0);
		mergeSort(A, q + 1, r, T, qq + 1);
	} else {
		#pragma omp task
		mergeSort(A, p, q, T, 0);
		
		mergeSort(A, q + 1, r, T, qq + 1);
		#pragma omp taskwait
	}
	
	merge(T, 0, qq, qq + 1, n - 1, B, s);

	delete[] T;
}

void merge(float* T, int p1, int r1, int p2, int r2, float* B, int p3)
{
	int n1 = r1 - p1 + 1;
	int n2 = r2 - p2 + 1;

	if (n1 < n2) {
		swap(p1, p2);
		swap(r1, r2);
		swap(n1, n2);
	}

	if (n1 == 0) return;

	int q1 = (p1 + r1) / 2;
	int q2 = search(T[q1], T, p2, r2);
	int q3 = p3 + (q1 - p1) + (q2 - p2);

	B[q3] = T[q1];

	if (q3 - p3 <= MAX-Partition) {
		merge(T, p1, q1 - 1, p2, q2 - 1, B, p3);
		merge(T, q1 + 1, r1, q2, r2, B, q3 + 1);
	} else {
		#pragma omp task 
		merge(T, p1, q1 - 1, p2, q2 - 1, B, p3);
		
		merge(T, q1 + 1, r1, q2, r2, B, q3 + 1);
		#pragma omp taskwait
	}
}

int main()
{
	srand((unsigned)(time(0)));
	omp_sethread__threads(thread_);
	omp_set_nested(1);
	omp_set_dynamic(1);


	float* A = new float[N];
	float* B = new float[N];


	for (int i = 0; i < N; i++) {
		A[i] = (rand()) / ((float)RAND_MAX / N);
	
	}
	auto start = chrono::high_resolution_clock::now();
	
	#pragma omp parallel
	#pragma omp single
	mergeSort(A, 0, N - 1, B, 0);
	
	auto finish = std::chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	double time = elapsed.count();

	cout << "Execution Time: " << time << endl;

	delete[] A;
	delete[] B;
}




