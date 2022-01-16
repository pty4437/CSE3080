#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//������ �� ����, �׽�Ʈ �� ����, Ư¡ �� ������ �����ϴ� ����
int numOfData, numOfTest, numOfFeature;

void calDistance(float*, float*, float*,int);
void chooseKNN(float*, int*, int*);
void chooseTestClass(int*, int*, int);
void swap(int*, int, int);
void sort(int*);
int arr[7];

int main() {
	//�����Ϳ� �׽�Ʈ�� feature������ �޴� �迭
	float** data;
	float** test;
	//�����Ϳ� �׽�Ʈ�� class������ �����ϴ� �迭
	int* dataClass;
	int* testClass;
	//test�� data������ �Ÿ��� �����ϴ� �迭
	float* distance;
	//data.txt�� test.txt, output.txt�� ���� �����ϴ� FILE�� ������
	FILE* dfp;
	FILE* tfp;
	FILE* ofp;
	int count = 0;

	dfp = fopen("data.txt", "r");
	tfp = fopen("test.txt", "r");
	ofp = fopen("output.txt", "w");

	fscanf(dfp, "%d %d", &numOfData,&numOfFeature);
	fscanf(tfp, "%d", &numOfTest);

	//�迭 �����Ҵ�
	data = (float**)malloc(sizeof(float*) * numOfData);
	test = (float**)malloc(sizeof(float*) * numOfTest);
	dataClass = (int*)malloc(sizeof(int) * numOfData);
	testClass = (int*)malloc(sizeof(int) * numOfTest);
	distance = (float*)malloc(sizeof(float) * numOfData);

	for (int i = 0; i < numOfData; i++) 
		data[i] = (float*)malloc(sizeof(float) * numOfFeature);

	for(int i = 0; i < numOfTest; i++)
		test[i] = (float*)malloc(sizeof(float) * numOfFeature);

	//�׽�Ʈ�� ������ �迭, ������Ŭ���� �迭�� �� �־��ֱ�
	for (int i = 0; i < numOfData; i++) {
		for (int j = 0; j < numOfFeature; j++) {
			fscanf(dfp, "%f", &data[i][j]);
		}
		fscanf(dfp, "%d", &dataClass[i]);
	}

	for (int i = 0; i < numOfTest; i++) {
		for (int j = 0; j < numOfFeature; j++)
			fscanf(tfp, "%f", &test[i][j]);
	}


	for (int j = 0; j < numOfTest; j++) {
		//distance����� �Ѵ�
		for (int i = 0; i < numOfData; i++) {
			calDistance(data[i], test[j], distance,i);
		}
		//distance�� ������ �̾Ƽ� �� Ŭ������ arr�迭�� �����Ѵ�.
		chooseKNN(distance, dataClass, arr);
		//j��° test�� Ŭ������ ���Ѵ�
		chooseTestClass(testClass, arr, j);
	}


	for (int i = 0; i < numOfTest; i++) {
		fprintf(ofp, "%d\n", testClass[i]);
	}

	//�޸� ����
	for (int i = 0; i < numOfData; i++)
		free(data[i]);

	free(data);

	for (int i = 0; i < numOfTest; i++)
		free(test[i]);

	free(test);

	free(dataClass); free(testClass); free(distance);

	fclose(dfp);
	fclose(tfp);
	fclose(ofp);
}

void calDistance(float data[], float test[], float distance[],int index) {
	float sum = 0;

	for (int i = 0; i < numOfFeature; i++) {
		sum += (data[i] - test[i]) * (data[i] - test[i]);
	}

	distance[index] = sqrt(sum);
}

void chooseKNN(float distance[], int dataClass[], int arr[]) {
	float min,max = 0;
	int index = 0;
	float min_before;
	int i, j;
	int add = 0;

	for (i = 0; i < numOfData; i++) 
		if (max < distance[i])
			max = distance[i];
	
	for (i = 0;i<7;i++) {

		min = max;

		for (j = 0; j < numOfData; j++) {
			if (min >= distance[j] && distance[j] != -1) {
				min = distance[j];
				index = j;
			}
		}
		arr[i] = dataClass[index];
		min_before = distance[index];
		distance[index] = -1;
	}
	
	while (1) {

		for (j = 0; j < numOfData; j++) {
			if (min >= distance[j] && distance[j] != -1) {
				min = distance[j];
				index = j;
			}
		}

		if (min_before == distance[index]) {
			if (dataClass[index] < arr[6]) {
				arr[6] = dataClass[index];
				distance[index] = -1;
			}

			else
				distance[index] = -1;
		}

		else
			break;

	}
	sort(arr);
}

void swap(int arr[], int i, int j) {
	int temp = arr[i];

	arr[i] = arr[j];
	arr[j] = temp;
}

void sort(int arr[]) {
	for (int i = 0; i < 7; i++) {
		for (int j = i; j < 7; j++) {
			if (arr[i] > arr[j])
				swap(arr, i, j);
		}
	}
}

void chooseTestClass(int testClass[], int arr[], int index) {
	int count = 0;
	int max = 0;
	int ans;

	for (int i = 0; i < 7; i++) {
		if (arr[i] == arr[i + 1])
			count++;
		else {
			if (count > max) {
				max = count;
				ans = arr[i];
				count = 0;
			}
			else
				count = 0;
		}
		
	}

	testClass[index] = ans;
}