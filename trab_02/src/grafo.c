#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grafo.h"

#define VERTICE_SIZE 10
#define VERTICE_NEIGHBOR_SIZE 50
#define MAX_LINE_LENGTH 2048

// Vertice structure.
typedef struct _vertice
{
    char *name;
    struct _vertice **neighbors;
    bool processed;
    int numNeighbors;
    int allocatedNeighbors;
} * vertice;

// Graph structure.
typedef struct _grafo
{
    vertice *vertices;
    vertice *oddDegreeVertices;
    int numVertices;
    int allocatedVertices;
    int numOddDegreeVertices;
    int allocatedOddDegreeVertices;
    int numEdges;
} * grafo;

void destroy_vertice(vertice v)
{
    free(v->neighbors);
    free(v->name);
    free(v);
}

// Function to destroy (free) the graph.
int destroi_grafo(grafo g)
{
    if (!g)
    {
        return 0;
    }
    for (int i = 0; i < g->numVertices; i++)
    {
        destroy_vertice(g->vertices[i]);
    }

    free(g->vertices);
    free(g->oddDegreeVertices);
    free(g);

    return 1;
}

// Function to initialize a vertice struct.
vertice create_vertice(char *name)
{
    vertice v = (vertice)malloc(sizeof(struct _vertice));

    v->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(v->name, name);
    v->neighbors = malloc(VERTICE_NEIGHBOR_SIZE * sizeof(vertice));
    v->numNeighbors = 0;
    v->allocatedNeighbors = VERTICE_NEIGHBOR_SIZE;

    return v;
}

// Function to initialize a graph struct.
grafo create_graph()
{
    grafo g = (grafo)malloc(sizeof(struct _grafo));

    g->vertices = malloc(VERTICE_SIZE * sizeof(struct _vertice));
    g->oddDegreeVertices = malloc(VERTICE_SIZE * sizeof(struct _vertice));
    g->numVertices = 0;
    g->allocatedVertices = VERTICE_SIZE;
    g->numOddDegreeVertices = 0;
    g->allocatedOddDegreeVertices = VERTICE_SIZE;
    g->numEdges = 0;

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
    if (target->numNeighbors == target->allocatedNeighbors)
    {
        target->allocatedNeighbors <<= 1; // Square of the number of neighbors allocated.
        target->neighbors = realloc(target->neighbors, target->allocatedNeighbors * sizeof(struct _vertice));
    }

    target->neighbors[target->numNeighbors++] = neighbor;
}

// Function to make a bidirectional link between two vertices.
void link_vertices(grafo g, char *a, char *b)
{
    vertice newA, newB;

    newA = search_or_create_vertice(g, a);
    newB = search_or_create_vertice(g, b);

    if (!is_neighbor(newA, newB) && !is_neighbor(newB, newA)) // *Redundancy
    {
        add_neighbor(newA, newB);
        add_neighbor(newB, newA);

        g->numEdges++;
    }
}

// Function to check if a letter exists in a given string.
bool have_letter(char *line)
{
    while (*line)
    {
        if ((65 <= *line && *line <= 90) || (97 <= *line && *line <= 122) || (48 <= *line && *line <= 57))
        {
            return true;
        }
        line++;
    }

    return false;
}

void add_odd_degree_vertices_in_the_graph(grafo g)
{
    for (int i = 0; i < g->numVertices; i++)
    {
        if (g->vertices[i]->numNeighbors % 2 != 0)
        {
            if (g->numOddDegreeVertices == g->allocatedOddDegreeVertices)
            {
                g->allocatedOddDegreeVertices <<= 1;
                g->oddDegreeVertices = realloc(g->oddDegreeVertices, g->allocatedOddDegreeVertices * sizeof(struct _vertice));
            }

            g->oddDegreeVertices[g->numOddDegreeVertices++] = g->vertices[i];
        }
    }
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

                while (token != NULL) // Split a string and store it into an array with two positions (two vertices at most).
                {
                    array[i++] = token;
                    token = strtok(NULL, delim);
                }

                if (array[0] != NULL && array[1] != NULL) // Two vertices.
                {
                    link_vertices(g, array[0], array[1]);
                }
                else // One vertice.
                {
                    search_or_create_vertice(g, array[0]);
                }
            }
        }

        fclose(input);

        add_odd_degree_vertices_in_the_graph(g);

        return g;
    }
    else
    {
        perror("Error to read the input file.\n");
        exit(1);
    }
}

// Recursive function to go through for all the vertices and neighbors and put these in a file.
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

// Function to set the "processed" variable to false in entire graph.
void set_vertice_processed_as_false(grafo g)
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

// Function to print a graph in stdout.
grafo escreve_grafo(FILE *output, grafo g)
{
    visit_graph_vertices(output, g->vertices[0]);
    set_vertice_processed_as_false(g);

    return g;
}

// Returns the number of vertices of a graph.
unsigned int n_vertices(grafo g)
{
    return g->numVertices;
}

// Returns the number of edges of a graph.
unsigned int n_arestas(grafo g)
{
    return g->numEdges;
}

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

// Function to remove a neighbor vertice from another.
void remove_neighbor(vertice target, vertice neighbor)
{
    for (int i = 0; i < target->numNeighbors; i++)
    {
        if (target->neighbors[i] == neighbor)
        {
            for (int j = i; j < target->numNeighbors - 1; j++)
            {
                target->neighbors[j] = target->neighbors[j + 1];
            }
            target->numNeighbors--;
            return;
        }
    }
}

// Function to bidirectionally remove a link between two vertices.
void unlink_vertices(grafo g, vertice a, vertice b)
{
    remove_neighbor(a, b);
    remove_neighbor(b, a);
    g->numEdges--;
}

// Function to check if a cycle was found.
bool found_cycle(vertice *cycle, vertice v, vertice caller, vertice origin, int *cycleSize)
{
    cycle[*cycleSize] = v;
    (*cycleSize)++;
    v->processed = true;

    for (int i = 0; i < v->numNeighbors; i++)
    {
        if (v->neighbors[i] != caller)
        {
            if (v->neighbors[i] == origin) // Found cycle.
            {
                cycle[*cycleSize] = v->neighbors[i];
                (*cycleSize)++;
                return true;
            }

            if (v->neighbors[i]->processed == false && found_cycle(cycle, v->neighbors[i], v, origin, cycleSize))
            {
                return true;
            }
        }
    }

    (*cycleSize)--;
    return false;
}

// Function to filter the final cycle.
vertice *real_cycle(vertice *cycle, int *cycleSize)
{
    int j = *cycleSize - 2, k = 0;
    vertice *cycleResult = malloc((*cycleSize) * sizeof(vertice));

    cycleResult[k++] = cycle[*cycleSize - 1]; // Put the first vertice in the final cycle.

    while (j >= 0 && cycle[j] != cycle[*cycleSize - 1]) // While not find the first vertice inserted, continue inserting vertices in the cycleResult.
    {
        cycleResult[k++] = cycle[j--];
    }

    cycleResult[k++] = cycle[j];

    *cycleSize = k;

    return cycleResult;
}

// Function to find a cycle with the DFS search method.
vertice *find_cycle_depth_first_search(vertice v, grafo g, int *cycleSize)
{
    set_vertice_processed_as_false(g);

    *cycleSize = 0;
    vertice *cycle = (vertice *)malloc((g->numEdges + 1) * sizeof(vertice));
    cycle[*cycleSize] = v;
    (*cycleSize)++;
    v->processed = true;

    for (int i = 0; i < v->numNeighbors; i++)
    {
        if (found_cycle(cycle, v->neighbors[i], v, v, cycleSize)) // If have a cycle
        {
            vertice *cycleResult = real_cycle(cycle, cycleSize); // Cut out and return to the final cycle (without "trash" vertices).

            free(cycle);

            return cycleResult;
        }
    }
    free(cycle);

    return NULL;
}

// Function to merge a cycle to the final trail.
void merge_cycle_to_trail(vertice *trail, int *trailSize, vertice *cycle, int *cycleSize, int trailIndex)
{
    for (int i = *trailSize - 1 + *cycleSize - 1, j = 0; j < *trailSize; i--, j++)
    {
        trail[i] = trail[i - (*cycleSize - 1)];
    }

    for (int i = trailIndex, j = 0; j < *cycleSize; i++, j++)
    {
        trail[i] = cycle[j];
    }

    *trailSize += *cycleSize - 1;
}

// Function to return a trail.
vertice *closed_eulerian_trail(grafo g, vertice r, int *trailSize)
{
    vertice *trail = malloc(g->numEdges * sizeof(vertice)); // A trail with the maximum number of possible positions (number of edges).

    int i = 0, cycleSize;
    trail[0] = r; // Insert the root vertice in the first position of the trail.
    *trailSize = 1;

    while (i < *trailSize && trail[i]->numNeighbors > 0) // While there is still a neighbor.
    {
        vertice *possibleCycle = find_cycle_depth_first_search(trail[i], g, &cycleSize);

        if (possibleCycle)
        {
            merge_cycle_to_trail(trail, trailSize, possibleCycle, &cycleSize, i);

            for (int j = 0; j < cycleSize - 1; j++)
            {
                unlink_vertices(g, possibleCycle[j], possibleCycle[j + 1]);
            }

            while (i < *trailSize && trail[i]->numNeighbors == 0)
            {
                i++;
            }
        }
        else
        {
            i++;
        }
    }

    return trail;
}

int split_closed_eulerian_trail(vertice *trailResult, int trailSize, vertice origin, vertice ***cobertura, int coberturaSize)
{
    int numTrails = 1, subTrailSize = 0, i = coberturaSize; // Have at least one trail (numTrails).

    for (int j = 1; j < trailSize - 1; j++) // count number of trails.
    {
        if (trailResult[j] == origin)
        {
            numTrails++;
        }
    }

    *cobertura = calloc((numTrails + 1), sizeof(vertice **)); // numTrails+1 -> sentinel (NULL).

    for (int j = 1; j < trailSize - 1; j++) // Allocate subtrails.
    {
        if (trailResult[j] != origin)
        {
            subTrailSize++;
        }
        else
        {
            (*cobertura)[i++] = calloc((subTrailSize + 1), sizeof(vertice *)); // subTrailSize + 1 -> sentinel (NULL)
            subTrailSize = 0;
        }
    }
    (*cobertura)[i++] = calloc((subTrailSize + 1), sizeof(vertice *)); // subTrailSize + 1 -> sentinel (NULL)
    subTrailSize = 0;

    i = coberturaSize; // reset

    for (int j = 1, k = 0; j < trailSize - 1; j++)
    {
        if (trailResult[j] != origin)
        {
            (*cobertura)[i][k++] = trailResult[j];
        }
        else
        {
            i++;
            k = 0;
        }
    }

    return numTrails;
}

unsigned int cobertura_por_trilhas(grafo g, vertice **cobertura[])
{
    int trailSize;
    vertice *trailResult, tempV = search_or_create_vertice(g, "tempV"); // A temporary vertice.
    unsigned int numTrails = 0;
    (*cobertura) = NULL;

    if (g->numOddDegreeVertices != 0)
    {
        for (int i = 0; i < g->numOddDegreeVertices; i++) // Link the temporary vertice to all vertices with odd degrees.
        {
            link_vertices(g, tempV->name, g->oddDegreeVertices[i]->name);
        }
        trailResult = closed_eulerian_trail(g, tempV, &trailSize);
        numTrails += (unsigned int)split_closed_eulerian_trail(trailResult, trailSize, tempV, cobertura, numTrails);
        free(trailResult);
    }

    for (int i = 0; i < g->numVertices; i++)
    {
        if (g->vertices[i]->numNeighbors > 0)
        {
            trailSize = 0;
            trailResult = closed_eulerian_trail(g, g->vertices[i], &trailSize);

            (*cobertura) = realloc((*cobertura), (numTrails + 2) * sizeof(vertice **));
            (*cobertura)[numTrails] = calloc(trailSize + 1, sizeof(vertice *));
            (*cobertura)[numTrails + 1] = NULL;

            for (int k = 0; k < trailSize; k++)
            {
                (*cobertura)[numTrails][k] = trailResult[k];
            }

            numTrails++;
        }
    }

    return numTrails;
}

// The main function.
int main()
{
    FILE *output = fopen("output.txt", "w");
    grafo g = le_grafo(stdin);
    vertice **cobertura;

    unsigned int numTrails = cobertura_por_trilhas(g, &cobertura);

    printf("%i\n", numTrails);

    for (int i = 0; cobertura[i]; i++)
    {
        for (int k = 0; cobertura[i][k]; k++)
        {
            printf("%s, ", cobertura[i][k]->name);
        }
        printf("\n");
    }

    escreve_grafo(output, g);
    destroi_grafo(g);
}
