#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef enum { bad_number_of_vertices, bad_number_of_edges, bad_vertex, bad_length, bad_number_of_lines, not_bad} bads;
typedef enum { false, true } bool;

void check_bads(const bads is_anything_bad, FILE *fout) {
	if (is_anything_bad == bad_number_of_vertices) {
		fprintf(fout, "bad number of vertices");
		exit(0);
	}
	if (is_anything_bad == bad_number_of_edges) {
		fprintf(fout, "bad number of edges");
		exit(0);
	}
	if (is_anything_bad == bad_vertex) {
		fprintf(fout, "bad vertex");
		exit(0);
	}
	if (is_anything_bad == bad_length) {
		fprintf(fout, "bad length");
		exit(0);
	}
	if (is_anything_bad == bad_number_of_lines) {
		fprintf(fout, "bad number of lines");
		exit(0);
	}
}

struct edge {
	int edge_end;
	int lenth;
	struct edge* next_edge;
};
struct edge_head {
	int vertex;
	struct edge_head* previous_vertex;
	struct edge* next_edge;
	int path_lenth;
	bool are_there_many_long_pathes;
};
struct edge_head_list{
	struct edge_head_list* next;
	struct edge_head* head;
};
struct allocated_edge_memory{
	struct edge* memory;
	int used_memory_cells;
};
struct allocated_edge_head_memory{
	struct edge_head* memory;
	int used_memory_cells;
};
struct allocated_edge_head_list_memory{
	struct edge_head_list* memory;
	int used_memory_cells;
};

struct edge* my_edge_malloc(struct allocated_edge_memory* my_edge_memory){
	struct edge* my_edge= &(my_edge_memory->memory[my_edge_memory->used_memory_cells]);
	my_edge_memory->used_memory_cells++;
	return my_edge;
}
struct edge_head* create_edge_head(int vertex){
	struct edge_head* head = malloc(sizeof(struct edge_head));
	head->vertex = vertex;
	head->previous_vertex = NULL;
	head->next_edge = NULL;
	head->path_lenth = -1;
	head->are_there_many_long_pathes = false;
	return head;
}

struct edge* create_edge(int start, int end, int lenth, struct allocated_edge_memory* my_edge_memory) {
	struct edge* new_edge = my_edge_malloc(my_edge_memory);
	new_edge->edge_end = end;
	new_edge->lenth = lenth;
	new_edge->next_edge = NULL;
	return new_edge;
}
struct edge_head_list* insert_edge_head_in_list(struct edge_head_list* list_of_edge_head,struct edge_head* inserted_edge_head) {
	struct edge_head_list* buff_edge_head_list = list_of_edge_head;
	struct edge_head_list* new_edge_head_list = (struct edge_head_list*)malloc(sizeof(struct edge_head_list));
	new_edge_head_list->next = NULL;
	new_edge_head_list->head = inserted_edge_head;
	if (list_of_edge_head == NULL) {
		return new_edge_head_list;
	}
	if (inserted_edge_head->path_lenth == -2) {
		while (buff_edge_head_list->next!=NULL){
			buff_edge_head_list = buff_edge_head_list->next;
		}
		buff_edge_head_list->next = new_edge_head_list;
		return list_of_edge_head;
	}
	if (list_of_edge_head->head->path_lenth > inserted_edge_head->path_lenth || list_of_edge_head->head->path_lenth == -2) {
		list_of_edge_head = new_edge_head_list;
		new_edge_head_list->next = buff_edge_head_list;
		return list_of_edge_head;
	}
	while (!(buff_edge_head_list->next == NULL || buff_edge_head_list->next->head->path_lenth > inserted_edge_head->path_lenth || buff_edge_head_list->next->head->path_lenth==-2)) {
		buff_edge_head_list = buff_edge_head_list->next;
	}
	struct edge_head_list* residue_of_list = buff_edge_head_list->next;
	buff_edge_head_list->next = new_edge_head_list;
	new_edge_head_list->next = residue_of_list;
	return list_of_edge_head;
}
struct edge_head* pop_edge_head_from_list(struct edge_head_list** list_of_edge_head) {
	if (*list_of_edge_head == NULL) {
		return NULL;
	}
	struct edge_head* popped_edge_head = (*list_of_edge_head)->head;
	*list_of_edge_head = (*list_of_edge_head)->next;
	return popped_edge_head;
}
struct edge* pop_edge_from_edge_head(struct edge_head* head_of_edges) {
	struct edge* popped_edge = head_of_edges->next_edge;
	if(popped_edge!=NULL){
		head_of_edges->next_edge = head_of_edges->next_edge->next_edge;
	}
	return popped_edge;
}

int get_number_of_vertices(const FILE *fin, bads *is_anything_bad) {
	int EOF_checker = 0,number_of_vertices=0;
	EOF_checker = fscanf(fin, "%d", &number_of_vertices);
	if (EOF_checker == EOF) {
		*is_anything_bad = bad_number_of_lines;
		return 0;
	}
	if(number_of_vertices < 0 || number_of_vertices>5000) {
		*is_anything_bad = bad_number_of_vertices;
		return 0;
	}
	return number_of_vertices;
}
void get_start_and_end(const FILE *fin,const int number_of_vertices, bads *is_anything_bad, int *start, int *end) {
	int EOF_checker = 0;
	EOF_checker = fscanf(fin, "%d", start);
	if (EOF_checker == EOF) {
		*is_anything_bad = bad_number_of_lines;
		return 0;
	}
	EOF_checker = fscanf(fin, "%d", end);
	if (EOF_checker == EOF) {
		*is_anything_bad = bad_number_of_lines;
		return 0;
	}
	if (*start < 1 || *start > number_of_vertices || *end < 1 || *end > number_of_vertices) {
		*is_anything_bad = bad_vertex;
		return 0;
	}
}
int get_number_of_edges(const FILE *fin,const int number_of_vertices, bads *is_anything_bad) {
	int EOF_checker = 0, number_of_edges = 0;
	EOF_checker = fscanf(fin, "%d", &number_of_edges);
	if (EOF_checker == EOF) {
		*is_anything_bad = bad_number_of_lines;
		return 0;
	}
	if (number_of_edges<0 || number_of_edges> number_of_vertices*(number_of_vertices+1)/2) {
		*is_anything_bad = bad_number_of_edges;
		return 0;
	}
	return number_of_edges;
}
void put_edge_in_array(struct edge_head** edges, struct edge* new_edge, int edge_start) {
		struct edge* current_edge = edges[edge_start]->next_edge;

		edges[edge_start]->next_edge = new_edge;
		new_edge->next_edge = current_edge;
}
bads get_edges(const FILE *fin, const int number_of_edges, const int number_of_vertices, struct edge** edges, struct allocated_edge_memory* my_edge_memory) {
	int EOF_checker = 0, first_vertex = 0,second_vertex=0,edge_lenth=0;
	for (int i = 0; i < number_of_edges; i++) {
		EOF_checker = fscanf(fin, "%d%d%d", &first_vertex,&second_vertex, &edge_lenth);
		if (EOF_checker == EOF || EOF_checker<3) {
			return bad_number_of_lines;
		}
		/*EOF_checker = fscanf(fin, "%d", &second_vertex);
		if (EOF_checker == EOF) {
			return bad_number_of_lines;
		}
		EOF_checker = fscanf(fin, "%d", &edge_lenth);
		if (EOF_checker == EOF) {
			return bad_number_of_lines;
		}*/
		if (first_vertex<1 || first_vertex>number_of_vertices || second_vertex<1 || second_vertex>number_of_vertices) {
			return bad_vertex;
		}
		if (edge_lenth < 0) {
			return bad_length;
		}
		if (first_vertex != second_vertex) {
			
			put_edge_in_array(edges, create_edge(second_vertex, first_vertex, edge_lenth, my_edge_memory), second_vertex);
			put_edge_in_array(edges, create_edge(first_vertex, second_vertex, edge_lenth, my_edge_memory), first_vertex);
		}
	}
	return not_bad;
}

/*edge_head->path_lenth :	>=0 длина пути
							-1 пути нет
							-2 путь больше INT_MAX
*/
void process_edge(struct edge_head** edges,struct edge* processing_edge, int start_vertex) {
	int edge_end = processing_edge->edge_end;
	int edge_lenth = processing_edge->lenth;
	int edge_start = start_vertex; /*= processing_edge->edge_start;*/
	struct edge_head* processing_edge_head  = edges[edge_start];
	struct edge_head* connected_edge_head = edges[edge_end];
	if (connected_edge_head->path_lenth == -1) {
		if (edge_lenth == -2 || edge_lenth + processing_edge_head->path_lenth < edge_lenth) {
			connected_edge_head->path_lenth = -2;
			connected_edge_head->previous_vertex = processing_edge_head;
			connected_edge_head->are_there_many_long_pathes = processing_edge_head->are_there_many_long_pathes;
		}
		else {
			connected_edge_head->path_lenth = edge_lenth + processing_edge_head->path_lenth;
			connected_edge_head->previous_vertex = processing_edge_head;
			connected_edge_head->are_there_many_long_pathes = processing_edge_head->are_there_many_long_pathes;
		}
	}
	else {
		if (connected_edge_head->path_lenth == -2) {
			if (edge_lenth == -2 || edge_lenth + processing_edge_head->path_lenth < edge_lenth) {
				connected_edge_head->path_lenth = -2;
				connected_edge_head->previous_vertex = processing_edge_head;
				connected_edge_head->are_there_many_long_pathes = true;
			}
			else {
				connected_edge_head->path_lenth = edge_lenth + processing_edge_head->path_lenth;
				connected_edge_head->previous_vertex = processing_edge_head;
			}
		}
		else {
			if (connected_edge_head->path_lenth > processing_edge_head->path_lenth + edge_lenth && !(edge_lenth + processing_edge_head->path_lenth < edge_lenth)) {
				connected_edge_head->path_lenth = processing_edge_head->path_lenth + edge_lenth;
				connected_edge_head->previous_vertex = processing_edge_head;
			}
		}
	}
}
void create_path(struct edge_head_list* list_of_edge_head, bool* is_vertice_checked, struct edge_head**edges){
	struct edge_head* processing_edge_head = pop_edge_head_from_list(&list_of_edge_head);
	while (processing_edge_head != NULL) {
		struct edge* processing_edge = pop_edge_from_edge_head(processing_edge_head);
		while (processing_edge!=NULL) {
			if (!is_vertice_checked[processing_edge->edge_end]) {
				
				process_edge(edges, processing_edge, processing_edge_head->vertex);
				list_of_edge_head = insert_edge_head_in_list(list_of_edge_head, edges[processing_edge->edge_end]);
			}
			processing_edge = pop_edge_from_edge_head(processing_edge_head);
		}
		is_vertice_checked[processing_edge_head->vertex]=true;
		processing_edge_head = pop_edge_head_from_list(&list_of_edge_head);
	}
}

void print_lenth_to_every_vertex(const struct edge_head** edges, const int number_of_vertices  , FILE* fout ) {
	for (int i = 1; i < number_of_vertices + 1; i++) {
		int path_lenth = edges[i]->path_lenth;
		if (path_lenth == -1) {
			fprintf(fout, "oo ");
		}
		else {
			if (path_lenth==-2) {
				fprintf(fout, "INT_MAX+ ");
			}
			else {
				fprintf(fout, "%d ", path_lenth);
			}
		}
	}
	fprintf(fout, "\n");
}
void print_path(const struct edge_head** edges,const int ending_vertex, FILE* fout ) {
	struct edge_head* current_vertex= edges[ending_vertex];
	if (current_vertex->path_lenth == -1) {
		fprintf(fout, "no path");
		return;
	}
	if (current_vertex->are_there_many_long_pathes == true && current_vertex->path_lenth == -2) {
		fprintf(fout, "overflow");
		return;
	}
	fprintf(fout, "%d ", ending_vertex);
	while (current_vertex != edges[(current_vertex->previous_vertex->vertex)]) {
		current_vertex = current_vertex->previous_vertex;
		fprintf(fout, "%d ", current_vertex->vertex);
	}
}

int main() {
	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "w");
	int number_of_vertices = 0, number_of_edges = 0, starting_vertex = 0, ending_vertex = 0;
	bads is_anything_bad = not_bad;

	number_of_vertices = get_number_of_vertices(fin, &is_anything_bad);
	check_bads(is_anything_bad, fout);
	get_start_and_end(fin, number_of_vertices, &is_anything_bad, &starting_vertex, &ending_vertex);
	check_bads(is_anything_bad, fout);
	number_of_edges = get_number_of_edges(fin, number_of_vertices, &is_anything_bad);
	check_bads(is_anything_bad, fout);

	struct edge_head**edges = malloc((number_of_vertices+1)*sizeof(struct edge_head*));
	for (int i = 1; i < number_of_vertices + 1; i++) {
		edges[i] = create_edge_head(i);
	}
	edges[starting_vertex]->path_lenth = 0;
	edges[starting_vertex]->previous_vertex = edges[starting_vertex];
	
	struct edge* edge_memory = malloc((number_of_edges+1)*2*sizeof(struct edge));
	struct allocated_edge_memory* my_edge_memory = malloc(sizeof(struct allocated_edge_memory)); 
	my_edge_memory->memory = edge_memory;
	my_edge_memory->used_memory_cells = 0;

	//struct edge_head* edge_head_memory = 

	is_anything_bad = get_edges(fin, number_of_edges, number_of_vertices, edges,my_edge_memory);
	check_bads(is_anything_bad,fout);
	bool*is_vertice_checked = (bool*)malloc((number_of_vertices+1)*sizeof(bool));
	for (int i = 1; i < number_of_vertices + 1; i++) {
		is_vertice_checked[i]=false;
	}
	struct edge_head_list*list_of_edge_head = (struct edge_head_list*) malloc (sizeof(struct edge_head_list));
	list_of_edge_head->next = NULL;
	list_of_edge_head->head = edges[starting_vertex];
	create_path( list_of_edge_head, is_vertice_checked, edges);

	print_lenth_to_every_vertex(edges, number_of_vertices, fout);
	print_path(edges, ending_vertex, fout);


	return 0;
}