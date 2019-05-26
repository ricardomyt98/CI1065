Trabalho de Implementação 2
CI1065 - Algoritmos e Teoria dos Grafos

Autor: Ricardo Norio Miyata
Matrícula: GRR20166368
Curso: Ciência da Computação (BCC)
Usuário (Dinf): rnm16

---Especificação---
void destroy_vertice(vertice v);
    Função para destruir (liberar memória) um vértice passado como parâmetro.

vertice create_vertice(char *name);
    Função para criar um vértice (alocar memória).

grafo create_graph(void);
    Função para criar um grafo (alocação de memoria da estrutura).

vertice search_or_create_vertice(grafo g, char *name);
    Retorna um vértice caso este já exista no grafo ou cria um novo, insere este no grafo e o retorna.

bool is_neighbor(vertice target, vertice neighbor);
    Função para verificar em um dado vértice se outro já é seu vizinho.

void add_neighbor(vertice target, vertice neighbor);
    Função para adicionar um vértice aos vizinhos de outro.

void link_vertices(grafo g, char *a, char *b);
    Função para criar uma aresta entre dois vértices, isto é, adicionar um ao outro em seus vértices vizinhos.

bool have_letter(char *line);
    Função para verificar se dado uma linha (string), se existe ou não ao menos um caracter válido para criar um vértice.

void add_odd_degree_vertices_in_the_graph(grafo g);
    Função para adicionar os vértices de grau ímpar em um vertor específico.

void visit_graph_vertices(FILE *output, vertice v);
    Função para visitar os vértices de um grafo por meio da buscar recursiva em largura.

void set_vertice_processed_as_false(grafo g);
    Função para setar uma flag de processado de todos os vértices do grafo como false.

void remove_neighbor(vertice target, vertice neighbor);
    função para remover um vértice dos vizinho de outro.

void unlink_vertices(grafo g, vertice a, vertice b);
    função para retirar uma aresta entre dois vértices.

bool found_cycle(vertice *cycle, vertice v, vertice caller, vertice origin, int *cycleSize);
    Função para dizer se encontrou ou não um ciclo dado um vértice.

vertice *real_cycle(vertice *cycle, int *cycleSize);
    Função para retornar o ciclo final efetivo.

vertice *find_cycle_depth_first_search(vertice v, grafo g, int *cycleSize);
    FUnção para encontrar um ciclo por meio do método de busca em profundidade.

void merge_cycle_to_trail(vertice *trail, int *trailSize, vertice *cycle, int *cycleSize, int trailIndex);
    Função para mesclar ciclo encontrado e uma trilha.

vertice *closed_eulerian_trail(grafo g, vertice r, int *trailSize);
    Função para encontrar uma trilha euliriana fechada.

int split_closed_eulerian_trail(vertice *trailResult, int trailSize, vertice origin, vertice ***cobertura, int coberturaSize);
    Função separar uma trilha e retornar o número de trilhas encontradas.
