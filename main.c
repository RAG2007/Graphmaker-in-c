#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main() {
	float min = FLT_MAX;
	float max = 0;
	int range_x;
	int range_y = 10;
	scanf("%d", &range_x);
	float index[range_y];

	int graph[range_x * 2][range_y];
	memset(graph, 0, range_x * 2 * range_y * sizeof(int));

	float data[range_x * 2];
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
	for (int i = 1; i < range_y; i++)
		index[i] = index[i - 1] + (float)((max - min) / (range_y - 1));
	
	int last;
	for (int a = 0; a < range_x; a++) {
		int best = -1;
		float best_value = FLT_MAX;
		for (int i = 0; i < range_y; i++) {
			if(fabsf(index[i] - data[a]) < best_value) {
				best = i;
				best_value = fabsf(index[i] - data[a]);
			}
		}
		graph[a*2][best] = 1;
		for(int i = 0; i < range_y; i++) {
			if(last == i && i == best)
				graph[a*2][i] = 1;
			if(last == i && i < best)
				graph[a*2][i] = 2;
			if(last == i && i > best)
				graph[a*2][i] = 3;
			if(((last < i && best > i) || (last > i && best < i)))
				graph[a*2][i] = 4;
			if(last > i && best == i)
				graph[a*2][i] = 5;
			if(last < i && best == i)
				graph[a*2][i] = 6;
		}
		last = best;
		graph[a*2+1][last] = 1;
	}
	for(int i = range_y - 1; i >= 0; i--) {
		printf("%6.2f ┼", index[i]);
		char str[1024];
		int length = 0;
		for(int a = 0; a < 2 * range_x; a++) {
			if(graph[a][i] == 0) {
				strcpy(&str[length], " ");
				length += sizeof(" ") - 1;
			}
			if(graph[a][i] == 1) {
				strcpy(&str[length], "─");
				length += sizeof("─") - 1;
			}
			if(graph[a][i] == 2) {
				strcpy(&str[length], "╯");
				length += sizeof("╯") - 1;
			}
			if(graph[a][i] == 3) {
				strcpy(&str[length], "╮");
				length += sizeof("╮") - 1;
			}
			if(graph[a][i] == 4) {
				strcpy(&str[length], "│");
				length += sizeof("│") - 1;
			}
			if(graph[a][i] == 5) {
				strcpy(&str[length], "╰");
				length += sizeof("╰") - 1;
			}
			if(graph[a][i] == 6) {
				strcpy(&str[length], "╭");
				length += sizeof("╭") - 1;
			}
		}
		printf("%s\n", str);
	}
}
