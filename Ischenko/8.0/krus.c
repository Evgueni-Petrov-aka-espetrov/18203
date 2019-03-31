#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>


typedef enum { false, true } bool;
typedef enum { bad_number_of_vertices, bad_number_of_edges, bad_vertex, bad_number_of_lines, no_spanning_tree, bad_weigth, not_bad }bads;

void check_bads(bads is_there_anything_bad, FILE *fout) {
	if (is_there_anything_bad == bad_number_of_vertices) {
		fprintf(fout, "bad number of vertices");
		exit(0);
	}
	if (is_there_anything_bad == bad_number_of_edges) {
		fprintf(fout, "bad number of edges");
		exit(0);
	}
	if (is_there_anything_bad == bad_vertex) {
		fprintf(fout, "bad vertex");
		exit(0);
	}
	if (is_there_anything_bad == bad_number_of_lines) {
		fprintf(fout, "bad number of lines");
		exit(0);
	}
	if (is_there_anything_bad == no_spanning_tree) {
		fprintf(fout, "no spanning tree");
		exit(0);
	}
	if (is_there_anything_bad == bad_weigth) {
		fprintf(fout, "bad length");
		exit(0);
	}
}

void get_number_of_vertices(int *number_of_vertices, bads *is_there_anything_bad, FILE *fin) {
	int EOF_catcher = 0;
	int number = 0;
	EOF_catcher = fscanf(fin, "%d", &number);
	if (EOF_catcher == EOF) {
		*is_there_anything_bad = bad_number_of_lines;
		return;
	}
	*number_of_vertices = number;
	if (*number_of_vertices > 5000 || *number_of_vertices < 0) {
		*is_there_anything_bad = bad_number_of_vertices;
		return;
	}
	if (*number_of_vertices == 0) {
		*is_there_anything_bad = no_spanning_tree;
		return;
	}
}
void get_number_of_edges(int *number_of_edges, int number_of_vertices, bads *is_there_anything_bad, FILE *fin) {
	int EOF_catcher = 0;
	int number = 0;
	EOF_catcher = fscanf(fin, "%d", &number);

	if (EOF_catcher == EOF) {
		*is_there_anything_bad = bad_number_of_lines;
		return;
	}
	*number_of_edges = number;
	if (*number_of_edges > number_of_vertices * (number_of_vertices + 1) / 2 || number_of_edges < 0) {
		*is_there_anything_bad = bad_number_of_edges;
	}
}

void get_edges(const int number_of_edges, const int number_of_vertices, FILE *fin, bads *is_there_anything_bad, bool *is_spanned, int *edges) {
	int EOF_catcher = 0;
	int first_vertex = 0;
	int second_vertex = 0;
	int edge_weigth = 0;
	for (int i = 1; i < number_of_edges + 1; i++) {
		EOF_catcher = fscanf(fin, "%d", &first_vertex);
		if (EOF_catcher == EOF) {
			*is_there_anything_bad = bad_number_of_lines;
			return;
		}
		EOF_catcher = fscanf(fin, "%d", &second_vertex);
		if (EOF_catcher == EOF) {
			*is_there_anything_bad = bad_number_of_lines;
			return;
		}
		EOF_catcher = fscanf(fin, "%d", &edge_weigth);
		if (EOF_catcher == EOF) {
			*is_there_anything_bad = bad_number_of_lines;
			return;
		}
		if (first_vertex<1 || first_vertex>number_of_vertices || second_vertex<1 || second_vertex>number_of_vertices) {
			*is_there_anything_bad = bad_vertex;
			return;
		}
		if (edge_weigth < 0) {
			*is_there_anything_bad = bad_weigth;
			return;
		}
		is_spanned[first_vertex] = true;
		is_spanned[second_vertex] = true;
		edges[i] = edge_weigth;
		edges[i + number_of_edges+1] = first_vertex;
		edges[i + 2 * (number_of_edges+1)] = second_vertex;
	}

}

void check_spanning(const int number_of_vertices, bool *is_spanned, FILE *fout) {
	bads is_there_anything_bad = not_bad;
	for (int i = 1; i < number_of_vertices + 1; i++) {
		if (!is_spanned[i]) {
			is_there_anything_bad = no_spanning_tree;
			break;
		}
	}
	check_bads(is_there_anything_bad, fout);
}

void rebalance(int* array_for_sort, int distance, int root_pos, int array_lenth) {
	if (root_pos * 2 + 1 < distance) {
		if (root_pos * 2 + 2 < distance) {
			if (array_for_sort[root_pos * 2 + 1] < array_for_sort[root_pos * 2 + 2]) {
				if (array_for_sort[root_pos] < array_for_sort[root_pos * 2 + 2]) {
					int buffweight = array_for_sort[root_pos];
					int buffstart = array_for_sort[root_pos+ array_lenth];
					int buffend = array_for_sort[root_pos+ 2* array_lenth];

					array_for_sort[root_pos]= array_for_sort[root_pos * 2 + 2];
					array_for_sort[root_pos+ array_lenth] = array_for_sort[root_pos * 2 + 2 + (array_lenth)];
					array_for_sort[root_pos +2* array_lenth] = array_for_sort[root_pos * 2 + 2 +2* (array_lenth)];

					array_for_sort[root_pos * 2 + 2] =buffweight;
					array_for_sort[root_pos * 2 + 2 + (array_lenth)] = buffstart;
					array_for_sort[root_pos * 2 + 2+ 2 * (array_lenth)] = buffend;

					rebalance(array_for_sort, distance, root_pos * 2 + 2, array_lenth);
				} 
			}
			else {
				if (array_for_sort[root_pos] < array_for_sort[root_pos * 2 + 1]) {
					int buffweight = array_for_sort[root_pos];
					int buffstart = array_for_sort[root_pos + array_lenth];
					int buffend = array_for_sort[root_pos + 2 * array_lenth];

					array_for_sort[root_pos] = array_for_sort[root_pos * 2 + 1];
					array_for_sort[root_pos + array_lenth] = array_for_sort[root_pos * 2 + 1 + (array_lenth)];
					array_for_sort[root_pos + 2 * array_lenth] = array_for_sort[root_pos * 2 + 1 + (2 * (array_lenth))];

					array_for_sort[root_pos * 2 + 1] = buffweight;
					array_for_sort[root_pos * 2 + 1+ (array_lenth)] = buffstart;
					array_for_sort[root_pos * 2 + 1+ (2 * (array_lenth))] = buffend;
					rebalance(array_for_sort, distance, root_pos * 2 + 1, array_lenth);
				}
			}
		}
		else {
			if (array_for_sort[root_pos] < array_for_sort[root_pos * 2 + 1]) {
				int buffweight = array_for_sort[root_pos];
					int buffstart = array_for_sort[root_pos + array_lenth];
					int buffend = array_for_sort[root_pos + 2 * array_lenth];

					array_for_sort[root_pos] = array_for_sort[root_pos * 2 + 1];
					array_for_sort[root_pos + array_lenth] = array_for_sort[root_pos * 2 + 1 + (array_lenth)];
					array_for_sort[root_pos + 2 * array_lenth] = array_for_sort[root_pos * 2 + 1 + (2 * (array_lenth))];

					array_for_sort[root_pos * 2 + 1] = buffweight;
					array_for_sort[root_pos * 2 + 1+ (array_lenth)] = buffstart;
					array_for_sort[root_pos * 2 + 1+ (2 *(array_lenth))] = buffend;
			}
		}
	}
}

void create_sorting_tree(int* array_for_sort, int distance, int root_pos) {
	if (root_pos * 2 + 1 < distance) {
		create_sorting_tree(array_for_sort, distance, root_pos * 2 + 1);
	}
	if (root_pos * 2 + 2 < distance) {
		create_sorting_tree(array_for_sort, distance, root_pos * 2 + 2);
	}
	rebalance(array_for_sort, distance, root_pos,distance);
}

void heap_sort(int* array_for_sort, int distance) {
	for (int i = distance; i > 1; --i) {
		rebalance(array_for_sort, i, 0,distance);

		int buffweight = array_for_sort[0];
		int buffstart = array_for_sort[0+distance];
		int buffend = array_for_sort[0 + 2*(distance)];

		array_for_sort[0] = array_for_sort[i - 1];
		array_for_sort[0+distance] = array_for_sort[i - 1+ distance];
		array_for_sort[0+2*(distance)] = array_for_sort[i - 1+ 2*(distance)];

		array_for_sort[i - 1] = buffweight;
		array_for_sort[i - 1+ distance] = buffstart;
		array_for_sort[i - 1+2*(distance)] = buffend;
	}
}

void get_main_element_of_group(const int *vertices, const int vertex, int *main_element_of_group) {
	*main_element_of_group = vertex;
	while ((*main_element_of_group) != vertices[*main_element_of_group]) {
		*main_element_of_group = vertices[*main_element_of_group];
	}
}

void create_edge_frame(const int *edges, const int number_of_edges, const int number_of_vertices, int *vertices, FILE *fout) {
	int first_vertex = 0, second_vertex = 0, edge_weight = 0;
	int main_element_of_first_group = 0, main_element_of_second_group = 0;
	for (int i = 1; i < number_of_edges + 1; i++) {
		edge_weight = edges[i];
		first_vertex = edges[i + number_of_edges+1];
		second_vertex = edges[i + 2 * (number_of_edges+1)];
		get_main_element_of_group(vertices, first_vertex, &main_element_of_first_group);
		get_main_element_of_group(vertices, second_vertex, &main_element_of_second_group);
		if (main_element_of_first_group != main_element_of_second_group) {
			fprintf(fout, "%d %d\n", first_vertex, second_vertex);
			if (vertices[main_element_of_first_group + 1 + number_of_vertices] < vertices[main_element_of_second_group + 1 + number_of_vertices]) {
				vertices[main_element_of_first_group] = main_element_of_second_group;
			}
			else {
				vertices[main_element_of_second_group] = main_element_of_first_group;
				if (vertices[main_element_of_first_group + 1 + number_of_vertices] == vertices[main_element_of_second_group + 1 + number_of_vertices]) {
					vertices[main_element_of_first_group + 1 + number_of_vertices]++;
				}
			}
		}
	}
}

int main() {
	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "w");
	int number_of_vertices = -1;
	int number_of_edges = -1;
	bads is_there_anything_bad = not_bad;

	get_number_of_vertices(&number_of_vertices, &is_there_anything_bad, fin);
	check_bads(is_there_anything_bad, fout);

	bool *is_spanned = (bool*)malloc((number_of_vertices + 1) * sizeof(bool));
	for (int i = 1; i < number_of_vertices + 1; i++) {
		is_spanned[i] = false;
	}

	get_number_of_edges(&number_of_edges, number_of_vertices, &is_there_anything_bad, fin);
	check_bads(is_there_anything_bad, fout);

	int *edges = (int*)malloc((number_of_edges + 1) * 3 * sizeof(int));
	get_edges(number_of_edges, number_of_vertices, fin, &is_there_anything_bad, is_spanned, edges);
	check_bads(is_there_anything_bad, fout);

	edges[0] = -1;
	edges[number_of_edges+1] = -1;
	edges[2*(number_of_edges + 1)] = -1;

	
	if (number_of_edges == 0 && number_of_vertices == 1) {
		return 0;
	}
	check_spanning(number_of_vertices, is_spanned, fout);
	free(is_spanned);

	create_sorting_tree(edges, number_of_edges+1,0);
	heap_sort(edges, number_of_edges + 1);

	int *vertices = (int*)malloc((number_of_vertices + 1)*2 * sizeof(int));
	for (int i = 0; i < number_of_vertices + 1; i++) {
		vertices[i] = i;
		vertices[i + 1 + number_of_vertices] = 1;
	}
	create_edge_frame(edges, number_of_edges, number_of_vertices, vertices, fout);
	free(edges);
	free(vertices);
	fclose(fin);
	fclose(fout);
	return 0;
}