#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grafo.h"

#define VERTICE_SIZE 10
#define VERTICE_NEIGHBOR_SIZE 50
#define MAX_STRING_LENGTH 1024
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
int destroi_grafo(grafo g);

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

vertice search_or_create_vertice(grafo g, char *name)
{
    for (int i = 0; i < g->numVertices; i++)
    {
        if (g->vertices[i]->name == name)
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

void link_vertices(grafo g, char *a, char *b)
{
    vertice newA, newB;

    newA = search_or_create_vertice(g, a);
    newB = search_or_create_vertice(g, b);

    add_neighbor(newA, newB);
    add_neighbor(newB, newA);
}

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

grafo le_grafo(FILE *input)
{
    char line[MAX_STRING_LENGTH];
    grafo g = create_graph();

    if (input)
    {
        while ((fgets(line, MAX_LINE_LENGTH, input)) != NULL)
        {
            if (have_letter(line))
            {
                int i = 0;
                char *token, *newline; // The "token" store the return of splitted string
                char *array[2] = {NULL, NULL};
                const char delim[2] = " ";

                // Handle the "\ n" of the line to not store the vertex name with it
                newline = strchr(line, '\n');
                if (newline)
                {
                    *newline = 0;
                }

                token = strtok(line, delim);

                while (token != NULL)
                {
                    array[i++] = token;
                    token = strtok(NULL, delim);
                }

                if (array[0] != NULL && array[1] != NULL)
                {
                    link_vertices(g, array[0], array[1]);
                }
                else
                {
                    search_or_create_vertice(g, array[0]);
                }
            }
        }
    }
    else
    {
        perror("Error to read the input file.\n");
        exit(1);
    }

    return g;
}

grafo escreve_grafo(FILE *output, grafo g);

unsigned int n_vertices(grafo g);

unsigned int n_arestas(grafo g);

vertice vertice_nome(grafo g, char *nome);

char *nome(vertice v);

unsigned int cobertura_por_trilhas(grafo g, vertice **cobertura[]);

int main()
{
    grafo g = le_grafo(stdin);
}
