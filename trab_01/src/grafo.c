#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo.h"

#define MAXSTRINGLENGTH 1024 // Max vertice string length
#define MAXLINELENGTH 1024*2 // Max number of vertices in a line

// Struct to represent a list of vertices neighbors
typedef struct neighbor {
	vertice neighborVertice;
	struct neighbor *nextNeighbor;
} *neighbor;

// Struct to represent a list of vertices
typedef struct vertice {
	char *verticeName;
	struct vertice *nextVertice;
	int numNeighbors;
	neighbor firstNeighbor;
	bool processed;
} *vertice;

// Struct to represent a graph
typedef struct grafo {
	int numVertices;
	vertice firstVertice;
} *grafo;

// Function to initialize a neighbor struct
neighbor create_neighbor(vertice v) {
	neighbor n = (neighbor)malloc(sizeof(struct neighbor));
	n->neighborVertice = v;
	n->nextNeighbor = NULL;
	
	return n;
}

// Function to initialize a vertice struct
vertice create_vertice(char *str) {
	vertice v = (vertice)malloc(sizeof(struct vertice));
	v->verticeName = str;
	v->nextVertice = NULL;
	v->numNeighbors = 0;
	v->firstNeighbor = NULL;
	v->processed = false;
	
	return v;
}

// Function to initialize a graph struct
grafo create_graph() {
	grafo g = (grafo)malloc(sizeof(struct grafo));
	g->numVertices = 0;
	g->firstVertice = NULL;
	
	return g;
}

// Function to check if a vertex already exists, otherwise insert it in the last position
void enqueue_vertice_in_graph(grafo g, vertice v) {
	if (g->firstVertice == NULL) {
		g->firstVertice = v;
		g->numVertices++;
	} else {
		vertice current = g->firstVertice;
		while (current->nextVertice != NULL && strcmp(current->verticeName, v->verticeName) != 0) {
			current = current->nextVertice;
		}
		if (strcmp(current->verticeName, v->verticeName) != 0) {
			current->nextVertice = v;
			g->numVertices++;
		}
	}
}

// Function to print all vertices of a graph
void print_graph_vertices(grafo g) {
	if (g->firstVertice == NULL) {
		printf("The graph is empty.\n");
	} else {
		int i = 1;
		vertice current = g->firstVertice;
		printf("---Printing all the vertices of the graph---\n");
		while (current->nextVertice != NULL) {
			printf("v%d: %s\n", i++, current->verticeName);
			current = current->nextVertice;
		}
		printf("v%d: %s\n", i++, current->verticeName);
	}
}

// Function to print all neighbors of a vertice
void print_vertice_neighbors(vertice v) {
	if (v->firstNeighbor == NULL) {
		printf("The vertice \"%s\" has no neighbors.\n", v->verticeName);
	} else {
		int i = 1;
		neighbor current = v->firstNeighbor;
		printf("---Printing all neighbors of the vertice \"%s\"---\n", v->verticeName);
		while (current->nextNeighbor != NULL) {
			printf("v%i: %s\n", i++, current->neighborVertice->verticeName);
			current = current->nextNeighbor;
		}
		printf("v%i: %s\n", i++, current->neighborVertice->verticeName);
	}
}

// Function to print the entire graph
void print_entire_graph(grafo g) {
	if (g->firstVertice == NULL) {
		printf("The graph is empty.\n");
	} else {
		vertice current = g->firstVertice;
		while (current->nextVertice != NULL) {
			print_vertice_neighbors(current);
			current = current->nextVertice;
		}
		print_vertice_neighbors(current);
	}
}

// Function for enqueue neighbors of a vertice
void enqueue_vertice_in_neighbor(vertice v1, vertice v2) {
	if (v1->firstNeighbor == NULL) {
		neighbor n = create_neighbor(v2);
		v1->firstNeighbor = n;
		v1->numNeighbors++;
	} else {
		neighbor current = v1->firstNeighbor;
		while (current->nextNeighbor != NULL && strcmp(current->neighborVertice->verticeName, v2->verticeName) != 0) {
			current = current->nextNeighbor;
		}
		if (strcmp(current->neighborVertice->verticeName, v2->verticeName) != 0) {
			neighbor n = create_neighbor(v2);
			current->nextNeighbor = n;
			v1->numNeighbors++;
		}
	}
}

// Function to make the bidirectional link between two vertices
void link_vertices_bidirectionally(vertice v1, vertice v2) {
	enqueue_vertice_in_neighbor(v1, v2);
	enqueue_vertice_in_neighbor(v2, v1);
}

// Function to destroy (free) the entire graph
int destroi_grafo(grafo g) {
	if (g == NULL) {
		return 0;
	} else if (g->firstVertice == NULL) {
		free(g);
		
		return 1;
	} else {
		vertice temp, current = g->firstVertice;
		while (current->nextVertice != NULL) {
			temp = current;
			current = current->nextVertice;
			free(temp);
		}
		free(g);
		
		return 1;
	}
}

// The function will return a new vertice if it does not exist in the graph, otherwise it will return the existing vertice
vertice vertice_already_exists_in_graph (grafo g, char *str) {
	if (g->firstVertice == NULL) {
		vertice v = create_vertice(str);
		
		return v;
	} else {
		vertice current = g->firstVertice;
		while (current->nextVertice != NULL && strcmp(current->verticeName, str) != 0) {
			current = current->nextVertice;
		}
		if (strcmp(current->verticeName, str) != 0) {
			vertice v = create_vertice(str);
			
			return v;
		} else {
			return current;
		}
	}
}

// Function to set the "processed" variable to false in entire graph
void set_vertice_processed_flag(grafo g) {
	if (g->firstVertice == NULL) {
		printf("The graph is empty.\n");
	} else {
		vertice current = g->firstVertice;
		for (int i = 0; i < g->numVertices; i++) {
			current->processed = false;
			current = current->nextVertice;
		}
	}
}

// Recursive function to go through for all the vertices and neighbors and put these in a file
void visit_graph_vertices(FILE *output, vertice v) {
	v->processed = true;
	neighbor currNeighbor = v->firstNeighbor;
	for (int i = 0; i < v->numNeighbors; i++) {
		if (currNeighbor->neighborVertice->processed == false) {
			fprintf(output, "%s %s\n\n", v->verticeName, currNeighbor->neighborVertice->verticeName);
		}
		currNeighbor = currNeighbor->nextNeighbor;
	}

	currNeighbor = v->firstNeighbor;
	for (int i = 0; i < v->numNeighbors; i++) {
		if (currNeighbor->neighborVertice->processed == false) {
			visit_graph_vertices(output, currNeighbor->neighborVertice);
		}
		currNeighbor = currNeighbor->nextNeighbor;
	}
}

// Function to put in a file and return the graph
grafo escreve_grafo(FILE *output, grafo g) {
	visit_graph_vertices(output, g->firstVertice);
	set_vertice_processed_flag(g);
	
	return g;
}

// Function to read a file and store the data in a graph struct
grafo le_grafo(FILE *input) {
	char line[MAXSTRINGLENGTH];
	grafo g = create_graph();

	if (input) {
		while ((fgets(line, MAXLINELENGTH, input)) != NULL) {
			if (line[0] != '\n') { // If the line is not empty
				int i = 0;
				char *token, *newline, *array[2] = {NULL}; // The "token" store the return of splitted string
				const char delim[2] = " "; // Variable to define the delimiter between vertices

				// Handle the "\ n" of the line to not store the vertex name with it
				newline = strchr(line, '\n');
				if (newline) {
					*newline = 0;
				}

				// Do the first split
				token = strtok(line, delim);

				// Split into vertices and store these to an array
				while (token != NULL) {
					array[i++] = token;
					token = strtok(NULL, delim);
				}

				if (array[0] != NULL && array[1] != NULL) { // Two vertices
					char *vStr1 = (char *)malloc(strlen(array[0]) * sizeof(char));
					char *vStr2 = (char *)malloc(strlen(array[0]) * sizeof(char));
					
					strcpy(vStr1, array[0]);
					strcpy(vStr2, array[1]);
					
					vertice v1 = vertice_already_exists_in_graph(g, vStr1);
					vertice v2 = vertice_already_exists_in_graph(g, vStr2);

					link_vertices_bidirectionally(v1, v2);

					enqueue_vertice_in_graph(g, v1);
					enqueue_vertice_in_graph(g, v2);
				} else { // One vertice
					char *vStr = (char *)malloc(strlen(array[0]) * sizeof(char));
					
					strcpy(vStr, array[0]);
					
					vertice v = create_vertice(vStr);
					
					enqueue_vertice_in_graph(g, v);
				}
			}
		}
	} else {
		perror("Error to read the input file.\n");
		exit(1);
	}

	return g;
}

// Check if a vertice is in the neighbors of another
bool is_in(vertice v1, vertice v2) {
	neighbor currV2Neighbor = v2->firstNeighbor;
	for (int i = 0; i < v2->numNeighbors; i++) {
		if (v1 == currV2Neighbor->neighborVertice) {
			return true;
		}
		currV2Neighbor = currV2Neighbor->nextNeighbor;
	}
	
	return false;
}

// Function to calculate the Clustering Coefficient of a graph
double coeficiente_agrupamento_grafo(grafo g) {
	int i, j, k, cTriad = 0, oTriad = 0, tTriad = 0;
	double result;
	vertice currVertice = g->firstVertice;

	for (i = 0; i < g->numVertices; i++) {
		vertice localNeighborArray[currVertice->numNeighbors];
		neighbor currNeighbor = currVertice->firstNeighbor;

		for (j = 0; j < currVertice->numNeighbors; j++) { // Store neighbors to a local array
			localNeighborArray[j] = currNeighbor->neighborVertice;
			currNeighbor = currNeighbor->nextNeighbor;
		}

		for (j = 0; j < currVertice->numNeighbors; j++) {
			for (k = j+1; k < currVertice->numNeighbors; k++) {
				if (is_in(localNeighborArray[j], localNeighborArray[k])) {
					cTriad++;
				} else {
					oTriad++;
				}
			}
		}	

		currVertice = currVertice->nextVertice;
	}
	result = (cTriad*1.0)/(cTriad+oTriad); // The "*1.0" is to convert the result to float

	return result;
}

// The main function
int main(void) {
	FILE *output = fopen("output.txt", "w");
	grafo g = le_grafo(stdin);

	if (!g){
		return 1;
	} else {
		escreve_grafo(output, g);
		printf("O coeficiente de agrupamento do grafo é %f\n", coeficiente_agrupamento_grafo(g));

		return !destroi_grafo(g);	
	}
}