#include <float.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int main() {
	float min = FLT_MAX;
	float max = 0;
	int range_x;
	int range_y = 10;
	scanf("%d", &range_x);
	float index[range_y];

	int graph[range_x][range_y];
	for (int i = 0; i < range_x; i++) {
		for(int a = 0; a < range_y; a++) {
			graph[i][a] = 0;
		}
	}

	float data[range_x];
	for (int i = 0; i < range_x; i++) {
		scanf("%f", &data[i]);
		if(data[i] < min) {
			min = data[i];
		}
		if(data[i] > max) {
			max = data[i];
		}
	}

	index[0] = min;
	for (int i = 1; i < range_y; i++) {
		index[i] = index[i - 1] + (float)((max - min) / (range_y - 1));
	}
	int best = -1;
	float best_value = FLT_MAX;
	for (int i = 0; i < range_y; i++) {
		if(fabsf(index[i] - data[0]) < best_value) {
			best = i;
			best_value = fabsf(index[i] - data[0]);
		}
	}
	int last;
	last = best;
	graph[0][best] = 1;


	for (int a = 0; a < range_x; a++) {
		int best = -1;
		float best_value = FLT_MAX;
		for (int i = 0; i < range_y; i++) {
			if(fabsf(index[i] - data[a]) < best_value) {
				best = i;
				best_value = fabsf(index[i] - data[a]);
			}
		}
		graph[a][best] = 1;
		for(int i = 0; i < range_y; i++) {
			if(last == i && i == best) {
				graph[a][i] = 1;
			}
			if(last == i && i < best) {
				graph[a][i] = 2;
			}
			if(last == i && i > best) {
				graph[a][i] = 3;
			}
			if(((last < i && best > i) || (last > i && best < i))) {
				graph[a][i] = 4;
			}
			if(last > i && best == i) {
				graph[a][i] = 5;
			}
			if(last < i && best == i) {
				graph[a][i] = 6;
			}
		}
		last = best;
	}
	for(int i = range_y - 1; i >= 0; i--) {
		printf("%6.2f ┼", index[i]);
		char data[1024];
		int length = 0;
		for(int a = 0; a < range_x; a++) {
			if(graph[a][i] == 0) {
				strcpy(&data[length], " ");
				length += sizeof(" ") - 1;
			}
			if(graph[a][i] == 1) {
				strcpy(&data[length], "─");
				length += sizeof("─") - 1;
			}
			if(graph[a][i] == 2) {
				strcpy(&data[length], "╯");
				length += sizeof("╯") - 1;
			}
			if(graph[a][i] == 3) {
				strcpy(&data[length], "╮");
				length += sizeof("╮") - 1;
			}
			if(graph[a][i] == 4) {
				strcpy(&data[length], "│");
				length += sizeof("│") - 1;
			}
			if(graph[a][i] == 5) {
				strcpy(&data[length], "╰");
				length += sizeof("╰") - 1;
			}
			if(graph[a][i] == 6) {
				strcpy(&data[length], "╭");
				length += sizeof("╭") - 1;
			}
		}
		printf("%s\n", data);
	}
}