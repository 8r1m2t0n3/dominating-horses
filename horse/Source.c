#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int N, horses = 0;
int **field = NULL, **num_field = NULL;
const int moveX[] = { -2, -1, 1, 2, 2, 1, -1, -2 };
const int moveY[] = { 1, 2, 2, 1, -1, -2, -2, -1 };
#define beat_fields 8

void put_horse(int x, int y) {
	field[y][x] = 2, horses++;
	for (int i = 0; i < beat_fields; i++) 
		if ((y + moveY[i]) < N && (x + moveX[i]) < N && (y + moveY[i]) >= 0 && (x + moveX[i]) >= 0) {
			if(field[y + moveY[i]][x + moveX[i]] != 2) field[y + moveY[i]][x + moveX[i]] = 0;
			num_field[y + moveY[i]][x + moveX[i]]++;
		}
}

void build_field(int n) {
	if(field) {
		for (int i = 0; i < N; i++) {
			free(field[i]);
			free(num_field[i]);
		}
		free(field);
		free(num_field);
	}
	N = n;
	field = (int**)malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		field[i] = (int*)malloc(sizeof(int) * N);
		memset(field[i], 0, sizeof(int) * N);
	}
	num_field = (int**)malloc(sizeof(int*) * N);
	for (int i = 0; i < N; i++) {
		num_field[i] = (int*)malloc(sizeof(int) * N);
		memset(num_field[i], 0, sizeof(int) * N);
	}
}

void print_field(int** dmas) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			printf("%d ", dmas[i][j]);
			if (j == N - 1) printf("\n");
		}
}

void del_horse(int x, int y) {
	field[y][x] = 0;
	horses--;
	for (int i = 0; i < beat_fields; i++)
		if ((y + moveY[i]) < N && (x + moveX[i]) < N && (y + moveY[i]) >= 0 && (x + moveX[i]) >= 0)
			num_field[y + moveY[i]][x + moveX[i]]--;
}

void fill_zero(int** dmas) {
	for (int i = 0; i < N; i++) memset(dmas[i], 0, sizeof(int) * N);
}

void put_all_horses() {
	for(int i = 1; i < N - 1; i++)
		for (int j = 1; j < N - 1; j++) {
			if (((i == 1 && j == 1) || (i == N - 2 && j == 1) || (i == 1 && j == N - 2) || (i == N - 2 && j == N - 2)) && N > 6) continue;
			put_horse(j, i);
		}
	if (N == 5) put_horse(N - 2, 0);
	if (N == 4) {
		put_horse(0, 0);
		put_horse(0, N - 1);
		put_horse(N - 1, 0);
		put_horse(N - 1, N - 1);
	}
}

void smart_del_1(int x, int y) {
	int num = 0;
	if (field[y][x] != 2) return;
	for (int i = 0; i < beat_fields; i++) 
		if ((y + moveY[i]) < N && (x + moveX[i]) < N && (y + moveY[i]) >= 0 && (x + moveX[i]) >= 0) {
			if (num_field[y + moveY[i]][x + moveX[i]] > 1) num--;
			num++;
		}
	if (num == 0 && num_field[y][x] > 1) del_horse(x, y);
}

void smart_del_2() {
	for (int i = 2; i < N - 1; i += 2) 
		for (int j = 1; j < N - 1; j++) {
			if (field[i][j] != 2) continue;
			del_horse(j, i);
		}
}

void smart_del_all() {
	for (int i = 1; i < N - 1; i++)
		for (int j = 1; j < N - 1; j++) smart_del_1(j, i);
}

void main() {
	clock_t start, stop;
	float end;
	int X = 0, Y = 0, flag = 0, n = 0, num = 0;
	while (1) {
		printf("Enter field's size: ");
		scanf("%d", &num);
		if (num <= 3) printf("ERROR\n");
		if (num > 16000) printf("Field configuration is to large\n");
		else break;
	}
	build_field(num);
	while (1) {
		printf("1.Put horse\n2.Delete horse\n3.Print field\n4.Filling field with horses\n5.Find solution\n6.Resize field\n");
		scanf("%d", &n);
		switch (n) {
		case 1:
			while (flag != 2) {
				scanf("%d %d", &X, &Y);
				if (X < N && Y < N && X >= 0 && Y >= 0) flag++;
				else {
					printf("Cell doesn't exist.\n");
					continue;
				}
				if (flag == 1 && field[Y][X] != 2) flag++;
				else {
					printf("Cell is occupied.\n");
					flag = 0;
				}
			}
			put_horse(X, Y);
			printf("Horse placed successfully.\nNumber of horses on the field: %d.\n", horses);
			flag = 0;
			break;
		case 2:
			scanf("%d %d", &X, &Y);
			if (Y < N && X < N && field[Y][X] == 2) {
				del_horse(X, Y);
				printf("Horse deleted successfully.\nNumber of horses on the field: %d.\n", horses);
			}
			else printf("There are no horses in that cell.\n");
			break;
		case 3:
			printf("\n");
			print_field(field);
			printf("\n");
			print_field(num_field);
			printf("\nNumber of horses on the field : %d.\n", horses);
			break;
		case 4:
			horses = 0;
			fill_zero(num_field);
			put_all_horses();
			printf("Horses placed successfully.\nNumber of horses on the field: %d.\n", horses);
			break;
		case 5:
			start = clock();
			smart_del_all();
			stop = clock();
			end = stop - start;
			printf("Elapsed time: %lf\n", end);
			printf("Operation was completed successfully.\nNumber of horses on the field: %d.\n", horses);
			break;
		case 6:
			horses = 0;
			while (1) {
				printf("Enter field's size: ");
				scanf("%d", &num);
				if (num <= 3) printf("ERROR\n");
				if (num > 16000) printf("Field configuration is to large\n");
				else break;
			}
			build_field(num);
			printf("Field resize successfully.\n");
			break;
		default:
			printf("ERROR\n");
			break;
		}
	}
}
