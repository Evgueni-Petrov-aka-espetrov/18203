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

void get_edges(const int number_of_edges, const int number_of_vertices, FILE *fin, bads *is_there_anything_bad, bool *is_spanned, int * edges) {
	int EOF_catcher = 0;
	int first_vertex = 0;
	int second_vertex = 0;
	int edge_weight = 0;
	for (int i = 0; i < number_of_edges; i++) {
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
		EOF_catcher = fscanf(fin, "%d", &edge_weight);
		if (EOF_catcher == EOF) {
			*is_there_anything_bad = bad_number_of_lines;
			return;
		}
		if (first_vertex<1 || first_vertex>number_of_vertices || second_vertex<1 || second_vertex>number_of_vertices) {
			*is_there_anything_bad = bad_vertex;
			return;
		}
		if (edge_weight < 0) {
			*is_there_anything_bad = bad_weigth;
			return;
		}
		is_spanned[first_vertex] = true;
		is_spanned[second_vertex] = true;
		if (edges[(first_vertex-1)*number_of_vertices+second_vertex-1]==-1) {
			edges[(first_vertex - 1)*number_of_vertices + second_vertex-1] = edge_weight;
			edges[(second_vertex - 1)*number_of_vertices +  first_vertex- 1] = edge_weight;
		}
		else {
			if (edges[(first_vertex - 1)*number_of_vertices + second_vertex-1] > edge_weight) {
				edges[(first_vertex - 1)*number_of_vertices + second_vertex - 1] = edge_weight;
				edges[(second_vertex - 1)*number_of_vertices + first_vertex - 1] = edge_weight;
			}
		}
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

void relax_distance(int*edges, int*distance_to_frame, int new_frame_vertex, int number_of_vertices, bool*is_verterx_in_frame) {
	int new_weight = -1;
	int current_edge = -1;
	for (int i = 0; i < number_of_vertices; i++) {
		current_edge = (new_frame_vertex - 1)*number_of_vertices + i;
		new_weight = edges[current_edge];
		if (new_weight != -1 && (new_weight < distance_to_frame[i] || distance_to_frame[i] == -1) && !is_verterx_in_frame[i]) {
			distance_to_frame[i] = new_weight;
			distance_to_frame[i + number_of_vertices] = new_frame_vertex;
		}
	}
}

int find_shortest_distance(int*distance_to_frame, bool*is_vertex_in_frame, int number_of_vertices) {
	int vertex = 0, min_weight = -1;
	for (int i = 0; i < number_of_vertices; i++) {
		if (!is_vertex_in_frame[i] && (min_weight > distance_to_frame[i] || min_weight==-1)&& distance_to_frame[i] != -1) {
			vertex = i ;
			min_weight = distance_to_frame[i];
		}
	}
	return vertex + 1;
}

void create_smallest_frame(int*edges, int*distance_to_frame, bool*is_vertex_in_frame, int number_of_vertices, FILE* fout) {
	relax_distance(edges, distance_to_frame, 1, number_of_vertices, is_vertex_in_frame);
	for (int i = 1; i < number_of_vertices; i++) {
		int new_frame_vertex = find_shortest_distance(distance_to_frame, is_vertex_in_frame, number_of_vertices);
		is_vertex_in_frame[new_frame_vertex - 1] = true;
		fprintf(fout, "%d %d\n",distance_to_frame[new_frame_vertex-1+number_of_vertices], new_frame_vertex);
		relax_distance(edges, distance_to_frame, new_frame_vertex, number_of_vertices, is_vertex_in_frame);
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
	
	get_number_of_edges(&number_of_edges, number_of_vertices, &is_there_anything_bad, fin);
	check_bads(is_there_anything_bad, fout);
	if (number_of_vertices == 1) {
		goto one_vertex;
	}

	bool *is_spanned = (bool*)malloc((number_of_vertices + 1) * sizeof(bool));
		for (int i = 1; i < number_of_vertices + 1; i++) {
		is_spanned[i] = false;
	}
	int *edges = malloc(number_of_vertices*number_of_vertices*sizeof(int));
	for (int i = 0; i < number_of_vertices; i++) {
		for (int j = 0; j < number_of_vertices; j++) {
			edges[i*number_of_vertices+j] = -1;
		}
	}

	get_edges(number_of_edges, number_of_vertices, fin, &is_there_anything_bad, is_spanned, edges);
	check_bads(is_there_anything_bad, fout);

	check_spanning(number_of_vertices,is_spanned,fout);

	int *distance_to_frame = malloc(number_of_vertices * 2 * sizeof(int));
	distance_to_frame[0] = 0;
	distance_to_frame[number_of_vertices] = 1;
	for (int i = 1; i < number_of_vertices; i++) {
		distance_to_frame[i] = -1;
		distance_to_frame[i + number_of_vertices] = -1;
	}
	bool* is_vertex_in_frame = malloc(number_of_vertices * sizeof(bool));
	is_vertex_in_frame[0] = true;
	for (int i = 1; i < number_of_vertices; i++) {
		is_vertex_in_frame[i] = false;
	}

	create_smallest_frame(edges, distance_to_frame, is_vertex_in_frame, number_of_vertices, fout);

	free(is_spanned);
	free(edges);
	free(distance_to_frame);
	free(is_vertex_in_frame);

one_vertex:
	fclose(fin);
	fclose(fout);
	
	return 0;
}