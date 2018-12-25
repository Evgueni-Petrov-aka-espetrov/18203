#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef enum {false,true}bool;
typedef enum {bad_number_of_vertices, bad_number_of_edges,bad_vertex,bad_number_of_lines,impossible_to_sort,not_bad}bads;
typedef enum {not_processed,in_process,processed}vertex_state;

void check_bads(bads is_there_anything_bad,FILE *fout){
	if (is_there_anything_bad == bad_number_of_vertices){
		fprintf(fout,"bad number of vertices");
		exit(0);
	}
	if (is_there_anything_bad == bad_number_of_edges){
		fprintf(fout, "bad number of edges");
		exit(0);
	}
	if (is_there_anything_bad == bad_vertex){
		fprintf(fout, "bad vertex");
		exit(0);
	}
	if (is_there_anything_bad == bad_number_of_lines){
		fprintf(fout, "bad number of lines");
		exit(0);
	}
	if (is_there_anything_bad == impossible_to_sort){
		fprintf(fout, "impossible to sort");
		exit(0);
	}
}

void get_number_of_vertices( int *number_of_vertices, bads *is_there_anything_bad, FILE *fin){
	int EOF_catcher = 0;
	int number = 0;
	EOF_catcher=fscanf(fin,"%d", &number);
	if (EOF_catcher == EOF){
		*is_there_anything_bad = bad_number_of_lines;
		return;
	}
	*number_of_vertices = number;
	if (*number_of_vertices > 1000 || *number_of_vertices < 0) {
		*is_there_anything_bad = bad_number_of_vertices;
		return;
	}
}
void get_number_of_edges(int *number_of_edges,int number_of_vertices, bads *is_there_anything_bad,  FILE *fin){
	int EOF_catcher = 0;
	int number = 0;
	EOF_catcher = fscanf(fin,"%d", &number);

	if (EOF_catcher == EOF){
		*is_there_anything_bad = bad_number_of_lines;
		return;
	}
	*number_of_edges = number;
	if (*number_of_edges > number_of_vertices * (number_of_vertices + 1) / 2 || number_of_edges<0)  {
		*is_there_anything_bad = bad_number_of_edges;
	}
}

void get_edges(bool *adjacency_matrix,int number_of_edges, bads *is_there_anything_bad, int number_of_vertices, FILE *fin) {
	int EOF_catcher = 0;
	int first_vertex = 0;
	int second_vertex = 0;
	for (int i = 0; i < number_of_edges; i++) {
		EOF_catcher = fscanf(fin,"%d", &first_vertex);
		if (EOF_catcher == EOF) {
			*is_there_anything_bad = bad_number_of_lines;
			return;
		}
		EOF_catcher = fscanf(fin,"%d", &second_vertex);
		if (EOF_catcher == EOF) {
			*is_there_anything_bad = bad_number_of_lines;
			return;
		}
		/*if (first_vertex == second_vertex) {
			*is_there_anything_bad = impossible_to_sort;
			return;
		}*/
		if (first_vertex<1 || first_vertex>number_of_vertices || second_vertex<1 || second_vertex>number_of_vertices) {
			*is_there_anything_bad = bad_vertex;
			return;
		}
		adjacency_matrix[first_vertex*(number_of_vertices+1)+second_vertex] = true;
	}

}

bads top_sort(vertex_state* state_of_vertices,int number_of_vertices,bool *adjacency_matrix, int *topological_position){
	bads possibility_of_sorting = not_bad;
	int top_level = 1;
	for (int i = 1; i <= number_of_vertices; i++){
		if (state_of_vertices[i] == not_processed){
			possibility_of_sorting = go_deeper_on_vertices(state_of_vertices, number_of_vertices, adjacency_matrix, topological_position,  &top_level,i);
			if (possibility_of_sorting != not_bad) {
				break;
			}
		}
	}
	return possibility_of_sorting;
}

bads go_deeper_on_vertices(vertex_state* state_of_vertices, int number_of_vertices, const bool  *adjacency_matrix, int *topological_position, int *top_level, int index_of_vertex){
	bads possibility_of_sorting=not_bad;
	state_of_vertices[index_of_vertex] = in_process;
	
	for (int i = 1; i <= number_of_vertices; i++){
		if (adjacency_matrix[index_of_vertex*(number_of_vertices+1) + i] == true){
			if (state_of_vertices[i] == in_process /*&& i!=index_of_vertex*/){
				possibility_of_sorting= impossible_to_sort;
				break;
			}
			if (state_of_vertices[i] == not_processed){
				possibility_of_sorting= go_deeper_on_vertices(state_of_vertices, number_of_vertices, adjacency_matrix, topological_position, top_level, i);
			}
		}
	}
	topological_position[index_of_vertex] = *top_level;
	*top_level = *top_level + 1;
	state_of_vertices[index_of_vertex] = processed;
	return possibility_of_sorting;
}

void recover_topological_position(int * topological_position, int number_of_vertices, int *right_topological_position ) {
	for (int i = 1; i < number_of_vertices+1; i++) {
		topological_position[i] = number_of_vertices - topological_position[i] + 1;
	}
	for (int i = 1; i < number_of_vertices + 1; i++) {
		right_topological_position[topological_position[i]] = i;
	}
}

int main() {
	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "w");
	int number_of_vertices = -1;
	int number_of_edges = -1;
	bads is_there_anything_bad = not_bad;
	get_number_of_vertices(&number_of_vertices,&is_there_anything_bad,fin);
	check_bads(is_there_anything_bad,fout);
	vertex_state *state_of_vertices = (vertex_state*) malloc((number_of_vertices+1) * sizeof(vertex_state));
	for (int i = 0; i < number_of_vertices + 1; i++) {
		state_of_vertices[i] = not_processed;
	}
	bool *adjacency_matrix = (bool *)malloc((number_of_vertices+1)*(number_of_vertices+1) * sizeof( bool));
	get_number_of_edges(&number_of_edges, number_of_vertices, &is_there_anything_bad,fin);
	check_bads(is_there_anything_bad,fout);
	for (int i = 0; i < number_of_vertices; i++){
		for (int u = 0; u < number_of_vertices; u++){
			adjacency_matrix[i*number_of_vertices + u] = false;
		}
	}
	
	get_edges(adjacency_matrix, number_of_edges, &is_there_anything_bad, number_of_vertices,fin);
	check_bads(is_there_anything_bad,fout);

	 int *topological_position = ( int*)malloc((number_of_vertices+1)*sizeof( int));
	 for (int i = 0; i < number_of_vertices + 1; i++) {
		 topological_position[i] = 0;
	 }
	is_there_anything_bad= top_sort(state_of_vertices, number_of_vertices, adjacency_matrix, topological_position);
	check_bads(is_there_anything_bad,fout);

	int *right_topological_position = (int*)malloc((number_of_vertices + 1) * sizeof(int));
	recover_topological_position(topological_position, number_of_vertices, right_topological_position);
	for (int i = 1; i < number_of_vertices + 1; i++) {
		fprintf(fout,"%d ", right_topological_position[i]);
	}
	free(state_of_vertices);
	free(adjacency_matrix);
	free(topological_position);
	fclose(fin);
	fclose(fout);
	return 0;
}