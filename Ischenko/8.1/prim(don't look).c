#include<stdio.h>
#include<malloc.h>

typedef enum { false, true }bool;

void relax_distance(int*edges, int*distance_to_frame, int new_frame_vertex, int number_of_vertices, bool*is_verterx_in_frame){
	int new_weight = -1;
	int current_edge = -1;
	for (int i = 0; i < number_of_vertices; i++){
		current_edge = (new_frame_vertex - 1)*number_of_vertices + i;
		new_weight = edges[current_edge];
		if (new_weight != -1 && new_weight < distance_to_frame[i] && !is_verterx_in_frame[i]){
			distance_to_frame[i] = new_weight;
			distance_to_frame[i + number_of_vertices] = new_frame_vertex;
		}
	}
}
int find_shortest_distance(int*distance_to_frame, bool*is_verterx_in_frame, int number_of_vertices){
	int vertex=0, min_weight = -1;
	for (int i = 0; i < number_of_vertices; i++){
		if (!is_verterx_in_frame[i] && min_weight>distance_to_frame[i] && distance_to_frame[i]!=-1 ){
			vertex = distance_to_frame[i + number_of_vertices];
			min_weight = distance_to_frame[i];
		}
	}
	return vertex + 1;
}

void create_smallest_frame(int*edges, int*distance_to_frame, bool*is_verterx_in_frame, int number_of_vertices, FILE* fout){
	relax_distance(edges, distance_to_frame, 1, number_of_vertices, is_verterx_in_frame);
	for (int i = 0; i < number_of_vertices; i++){
		int new_frame_vertex = find_shortest_distance(distance_to_frame, is_verterx_in_frame, number_of_vertices);
		is_verterx_in_frame[new_frame_vertex - 1] = true;
		fprintf(fout, "%d %d\n", i + 1, new_frame_vertex);
		relax_distance(edges, distance_to_frame, new_frame_vertex, number_of_vertices, is_verterx_in_frame);
	}

}

int main(){

	int number_of_vertices = 0, number_of_edges = 0;
	//получение данных
	int * edges = malloc(number_of_vertices*number_of_vertices*sizeof(int));
	//заполнение
	int *distance_to_frame = malloc(number_of_vertices*2*sizeof(int));
	distance_to_frame[0] = 0;
	distance_to_frame[number_of_vertices] = 1;
	for (int i = 1; i < distance_to_frame; i++){
		distance_to_frame[i] = -1;
		distance_to_frame[i+number_of_vertices] = -1;
	}
	bool* is_vertex_in_frame = malloc(number_of_vertices*sizeof(bool));
	is_vertex_in_frame[0] = true;
	for (int i = 1; i < number_of_vertices; i++){
		is_vertex_in_frame[i] = false;
	}


}