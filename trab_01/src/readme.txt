Trabalho de Implementação 1
CI1065 - Algoritmos e Teoria dos Grafos

Autor: Ricardo Norio Miyata
Matrícula: GRR20166368
Curso: Ciência da Computação (BCC)
Usuário (Dinf): rnm16

---Especificação---
O trabalho possui funções de especificação e de depuração. As funções de especificação são aquelas usadas como dependências para a correta execução do código. Funções de depuração são aquelas criadas para depurar o código (uso pessoal).

Funções de especificação:
	- neighbor create_neighbor(vertice v);
		Função de invocação e inicialização da estrutura vizinho. Esta estrutura possui um vértice e ponteiro para outro nodo de mesma estruta (lista).

	- vertice create_vertice(char *str);
		Função de invocação e inicialização da estrutura vértice. Esta estrutura possui uma string (nome do vértice) e ponteiro para outro nodo de mesma estruta (lista).

	- grafo create_graph();
		Função de invocação e inicialização da estrutura grafo. Esta estrutura possui um poteiro para o primeiro vértice do grafo.

	- void enqueue_vertice_in_graph(grafo g, vertice v);
		Função para inserir um vertice no grafo (lista de vértices).

	- void link_vertices_bidirectionally(vertice v1, vertice v2);
		Função para linkar bidirecionalmente dois vértices, isto é, para representar um grafo não direcionado.

	- vertice vertice_already_exists_in_graph (grafo g, char *str);
		Função para verificar se uma string (nome de um vértice) já existe no grafo.

	- void set_vertice_processed_flag(grafo g);
		Função para setar/ resetar a flag "processed" dos vértices.

	- void visit_graph_vertices(FILE *output, vertice v);
		Função para visitar (passar pelos vértices, tratando a flag "processed") os vértices do grafo.

	- grafo escreve_grafo(FILE *output, grafo g);
		Função da especificação para imprimir um grafo num arquivo de saída.

	- grafo le_grafo(FILE *input);
		Função para ler vértices de um arquivo de entrada e transformar este em uma estrutura de grafo

	- bool is_in(vertice v1, vertice v2);
		Função para verificar se o "v1" está presente nos vértices referente aos vizinhos de "v2".

	- double coeficiente_agrupamento_grafo(grafo g);
		Função para calcular o coeficiente de agrupamento de um grafo.

Funções de depuração:
	- void print_graph_vertices(grafo g);
		Função para imprimir no terminal vértices de um grafo.

	- void print_vertice_neighbors(vertice v);
		Função para imprimir no terminal vizinhos de um vértice.

	- void print_entire_graph(grafo g);
		Função para imprimir no terminal o grafo inteiro, desde vertices a seus vizinhos.