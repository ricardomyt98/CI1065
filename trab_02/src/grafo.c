#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grafo.h"

#define VERTICE_SIZE 10
#define VERTICE_NEIGHBOR_SIZE 50
#define MAX_LINE_LENGTH 2048

// Vertice structure
typedef struct _vertice
{
    char *name;
    struct _vertice **neighbors;
    bool processed;
    int numNeighbors;
    int allocatedNeighbors;
} * vertice;

// Graph structure
typedef struct _grafo
{
    vertice *vertices;
    int numVertices;
    int allocatedVertices;
} * grafo;

// Function struct _verticeto destroy the graph
int destroi_grafo(grafo g)
{
    for (int i = 0; i < g->numVertices; i++)
    {
        free(g->vertices[i]);
    }
    free(g);

    return 1;
}

// Function to initialize a vertice struct
vertice create_vertice(char *name)
{
    vertice v = (vertice)malloc(sizeof(struct _vertice));

    v->name = name;
    v->neighbors = malloc(VERTICE_NEIGHBOR_SIZE * sizeof(struct _vertice));
    v->numNeighbors = 0;
    v->allocatedNeighbors = VERTICE_NEIGHBOR_SIZE;

    return v;
}

// Function to initialize a graph struct
grafo create_graph()
{
    grafo g = (grafo)malloc(sizeof(struct _grafo));

    g->vertices = malloc(VERTICE_SIZE * sizeof(struct _vertice));
    g->numVertices = 0;
    g->allocatedVertices = VERTICE_SIZE;

    return g;
}

// Function to search for a vertice or create and insert a new one in the graph.
vertice search_or_create_vertice(grafo g, char *name)
{
    for (int i = 0; i < g->numVertices; i++)
    {
        if (strcmp(g->vertices[i]->name, name) == 0)
        {
            return g->vertices[i];
        }
    }

    vertice newVertice = create_vertice(name);

    if (g->numVertices == g->allocatedVertices)
    {
        g->allocatedVertices <<= 1;
        g->vertices = realloc(g->vertices, g->allocatedVertices * sizeof(struct _vertice));
    }

    g->vertices[g->numVertices++] = newVertice;

    return newVertice;
}

// Function to check if a given vertice is or not neighbor of another.
bool is_neighbor(vertice target, vertice neighbor)
{
    for (int i = 0; i < target->numNeighbors; i++)
    {
        if (target->neighbors[i]->name == neighbor->name)
        {
            return true;
        }
    }

    return false;
}

// Function to insert a vertice in the neighbors of another.
void add_neighbor(vertice target, vertice neighbor)
{
    if (!is_neighbor(target, neighbor))
    {
        if (target->numNeighbors == target->allocatedNeighbors)
        {
            target->allocatedNeighbors <<= 1; // Square of the number of neighbors allocated.
            target->neighbors = realloc(target->neighbors, target->allocatedNeighbors * sizeof(struct _vertice));
        }

        target->neighbors[target->numNeighbors++] = neighbor;
    }
}

// Function to make a bidirectional link between two vertices.
void link_vertices(grafo g, char *a, char *b)
{
    vertice newA, newB;

    newA = search_or_create_vertice(g, a);
    newB = search_or_create_vertice(g, b);

    add_neighbor(newA, newB);
    add_neighbor(newB, newA);
}

// Function to check if a letter exists in a given string.
bool have_letter(char *line)
{
    while (*line)
    {
        if ((65 <= *line && *line <= 90) || (97 <= *line && *line <= 122))
        {
            return true;
        }
        line++;
    }

    return false;
}

// Function to read a file and return a graph.
grafo le_grafo(FILE *input)
{
    if (input)
    {
        char line[MAX_LINE_LENGTH];
        grafo g = create_graph();

        while (fgets(line, sizeof(line), input))
        {
            if (have_letter(line))
            {
                int i = 0;
                char *token, *newline, *array[2] = {NULL}; // The "token" store the return of splitted string.
                const char delim[2] = " ";

                // Handle the "\n" of the line to not store in the vertex name.
                newline = strchr(line, '\n');
                if (newline)
                {
                    *newline = 0;
                }

                token = strtok(line, delim);

                while (token != NULL) // Split a string and store it into an array with two positions (two vertices at most)
                {
                    array[i++] = token;
                    token = strtok(NULL, delim);
                }

                if (array[0] != NULL && array[1] != NULL) // Two vertices
                {
                    char *vStr1 = (char *)malloc(strlen(array[0]) * sizeof(char));
                    char *vStr2 = (char *)malloc(strlen(array[1]) * sizeof(char));

                    strcpy(vStr1, array[0]);
                    strcpy(vStr2, array[1]);

                    link_vertices(g, vStr1, vStr2);
                }
                else // One vertice
                {
                    char *vStr1 = (char *)malloc(strlen(array[0]) * sizeof(char));

                    strcpy(vStr1, array[0]);

                    search_or_create_vertice(g, vStr1);
                }
            }
        }

        fclose(input);

        return g;
    }
    else
    {
        perror("Error to read the input file.\n");
        exit(1);
    }
}

// Recursive function to go through for all the vertices and neighbors and put these in a file
void visit_graph_vertices(FILE *output, vertice v)
{
    v->processed = true;

    for (int i = 0; i < v->numNeighbors; i++)
    {
        if (v->neighbors[i]->processed == false)
        {
            fprintf(output, "%s %s\n\n", v->name, v->neighbors[i]->name);
        }
    }

    for (int i = 0; i < v->numNeighbors; i++)
    {
        if (v->neighbors[i]->processed == false)
        {
            visit_graph_vertices(output, v->neighbors[i]);
        }
    }
}

// Function to set the "processed" variable to false in entire graph
void set_vertice_processed_flag(grafo g)
{
    if (g->numVertices == 0)
    {
        printf("The graph is empty.\n");
    }
    else
    {
        for (int i = 0; i < g->numVertices; i++)
        {
            g->vertices[i]->processed = false;
        }
    }
}

// Function to print a graph in stdout
grafo escreve_grafo(FILE *output, grafo g)
{
    visit_graph_vertices(output, g->vertices[0]);
    set_vertice_processed_flag(g);

    return g;
}

// Returns the number of vertices of a graph.
unsigned int n_vertices(grafo g)
{
    return g->numVertices;
}

// Returns the number of edges of a graph.
unsigned int n_arestas(grafo g);

// Returns a vertice searched for in a graph by a given string.
vertice vertice_nome(grafo g, char *nome)
{
    for (int i = 0; i < g->numVertices; i++)
    {
        if (strcmp(g->vertices[i]->name, nome) == 0)
        {
            return g->vertices[i];
        }
    }

    return NULL;
}

// Returns the name of a vertice.
char *nome(vertice v)
{
    return v->name;
}

unsigned int cobertura_por_trilhas(grafo g, vertice **cobertura[]);

// The main function.
int main()
{
    FILE *output = fopen("output.txt", "w");
    grafo g = le_grafo(stdin);
    escreve_grafo(output, g);
    destroi_grafo(g);
}
