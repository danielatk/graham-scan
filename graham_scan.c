#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct t_vertice{
  float x;
  float y;
};

struct t_pilha{
  int topo;
  unsigned int capacidade;
  struct t_vertice *fecho_conv;
};

//essa variável irá guardar o menor vértice considerando a coordenada y
struct t_vertice p0;

struct t_pilha criaPilha(unsigned int capacidade){
  struct t_pilha pilha;
  pilha.capacidade = capacidade;

  //inicializa topo da pilha em -1
  pilha.topo = -1;
  pilha.fecho_conv = (struct t_vertice*)malloc(pilha.capacidade * sizeof(struct t_vertice));
  return pilha;
}

//pilha está cheia quando topo é igual ao último índice
int estaCheia(struct t_pilha *pilha){
  return pilha->topo == pilha->capacidade - 1;
}
 
//pilha está vazia quando o topo é igual a -1
int estaVazia(struct t_pilha *pilha){
  return pilha->topo == -1;
}

//função para adicionar um vértice à pilha. Incrementa topo em 1
void push(struct t_pilha *pilha, struct t_vertice v){
  if(estaCheia(pilha)){

    //aloca memória para mais um vértice ser empurrado na pilha
    pilha->fecho_conv = (struct t_vertice*)realloc(pilha->fecho_conv, ++pilha->capacidade * sizeof(struct t_vertice));
  }
  pilha->fecho_conv[++pilha->topo] = v;
}
 
//função para retirar um vértice da pilha. Decrementa topo em 1
void pop(struct t_pilha *pilha){
  if(!estaVazia(pilha)){
    pilha->topo--;
  }
}

//função que retorna vértice que está no topo da pilha
struct t_vertice pegaTopo(struct t_pilha *pilha){
  return pilha->fecho_conv[pilha->topo];
}

//função que retorna vértice que está na posição anterior ao topo da pilha
struct t_vertice pegaAntTopo(struct t_pilha *pilha){
  struct t_vertice v = pegaTopo(pilha);
  pop(pilha);
  struct t_vertice ant = pegaTopo(pilha);
  push(pilha, v);
  return ant;
}

//função que troca dois vértices de posição
void troca(struct t_vertice *v1, struct t_vertice *v2){
  struct t_vertice v3 = *v1;
  *v1 = *v2;
  *v2 = v3;
}

//função que lê vértices do arquivo de entrada (formato por linha: <coordenada x> <coordenada y>)
void leVertices(FILE *f_vertices, char *nome_vertices, struct t_vertice **vert, int *qtd_vertices){
  f_vertices = fopen(nome_vertices, "r");
  if (f_vertices == NULL){
    printf("Erro na abertura do arquivo %s\n", nome_vertices);
  }

  //variável auxiliar para receber as coordenadas dos vértices do arquivo f_vertices
  char aux[30];

  *qtd_vertices = 0;

  while(fgets(aux, 30, f_vertices)!=NULL){ //descobre quantos vértices tem no arquivo de entrada
    *qtd_vertices = *qtd_vertices + 1;
  }

  //aloca espaço para cada um dos vértices (2 pois temos coordenadas x e y do vértice)
  *vert = (struct t_vertice*)malloc(*qtd_vertices * sizeof(struct t_vertice*));

  //volta para o início do documento que contém as coordenadas dos vértices
  rewind(f_vertices);

  //vai ser usada para iterar o vetor de vértices
  int index = 0;

  //vai ser usada para marcar o início de cada coordenada em aux
  int inicio;

  //vai ser usada para guardar a string que contém a coordenada x de cada vértice
  char aux_x[30];

  //vai ser usada para guardar a string que contém a coordenada y de cada vértice
  char aux_y[30];

  int i, j;

  //pega coordenadas dos vértices até acabar arquivo (1 por linha)
  while(fgets(aux, 30, f_vertices)!=NULL){
    inicio = 0;
    for(i = 0; i<strlen(aux); i++){
      if(aux[i] == ' '){
        for(j = 0; j < i - inicio; j++){
          aux_x[j] = aux[inicio + j];
        }
        inicio = i + 1;
        aux_x[j] = '\0';
        (*vert + index)->x = atof(aux_x);
      }
      if(aux[i] == '\n'){
        for(j = 0; j < i - inicio; j++){
          aux_y[j] = aux[inicio + j];
        }
        aux_y[j] = '\0';
        (*vert + index)->y = atof(aux_y);
      }

    }
    index++;
  }
}

//função para escrever os vértices no terminal
void escreveVertices(struct t_vertice *vert, int qtd_vertices){
  for(int i = 0; i < qtd_vertices; i++){
    printf("Vertice %d:\n", i);
    printf("coordenada x: %f\n", vert[i].x);
    printf("coordenada y: %f\n", vert[i].y);
    printf("\n");
  }
}

//função que retorna a distância entre dois vértices ao quadrado
int distQuad(struct t_vertice v1, struct t_vertice v2){
  return (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y);
}

//função que descobre a orientação de um 3-upla de vértices
//retorna 0 se v1, v2 e v3 forem colineares, 1, se estiverem no sentido horário e 2 se estiverem no sentido anti-horário
int orientacao(struct t_vertice v1, struct t_vertice v2, struct t_vertice v3){
  int resultado = (v2.y - v1.y) * (v3.x - v2.x) - (v2.x - v1.x) * (v3.y - v2.y);
  int orient;
  if (resultado == 0){
    orient = 0;
  }else if (resultado > 0){
    orient = 1;
  }else{
    orient = 2;
  }
  return orient;
}

//função que ordena vetor de pontos em relação ao primeiro ponto
int compara(const void *p1, const void *p2){
  struct t_vertice *v1 = (struct t_vertice *)p1;
  struct t_vertice *v2 = (struct t_vertice *)p2;

  int orient = orientacao(p0, *v1, *v2);
  int res;
  if(orient == 0){
    if(distQuad(p0, *v2) >= distQuad(p0, *v1)){
      res = -1;
    }else{
      res = 1;
    }
  }else if(orient == 2){
    res = -1;
  }else{
    res = 1;
  }
  return res;
}

/*int particiona(struct t_vertice *vert, int baixo, int alto){
  float pivo = vert[alto].x;
  int i = (baixo - 1);
  for(int j = baixo; j <= alto - 1; j++){

    //ordena pela coordenada x
    if(vert[j].x < pivo){
      i++;
      troca(&vert[i].x, &vert[j].x);
      troca(&vert[i].y, &vert[j].y);
    }

    //se coordenada x igual então ordena pela coordenada y
    if(vert[j].x == pivo){
      pivo = vert[alto].y;
      if(vert[j].y <= pivo){
        i++;
        troca(&vert[i].x, &vert[j].x);
        troca(&vert[i].y, &vert[j].y);
      }
      //restaura x como coordenada para pivô
      pivo = vert[alto].x;
    }
  }
  troca(&vert[i + 1].x, &vert[alto].x);
  troca(&vert[i + 1].y, &vert[alto].y);
  return (i + 1);
}

void quickSort(struct t_vertice *vert, int baixo, int alto){
  if(baixo < alto){
    int ip = particiona(vert, baixo, alto);
    quickSort(vert, baixo, ip - 1);
    quickSort(vert, ip + 1, alto);
  } 
}*/

//função que imprime fecho convexo
void imprimeFecho(struct t_pilha pilha){
  int index = 0;
  struct t_vertice v;
  while(!estaVazia(&pilha)){
    v = pegaTopo(&pilha);
    printf("Vertice %d:\n", index);
    printf("coordenada x: %f\n", v.x);
    printf("coordenada y: %f\n\n", v.y);
    pop(&pilha);
    index++;
  }
}

struct t_pilha fechoConvexo(struct t_vertice *vertices, int n){
  float menor = vertices[0].y;
  int ind_menor = 0;

  //encontra o menor vértice considerando a coordenada y (se coordenada y for igual, o menor ponto é decidido pela coordenada x)
  for (int i = 1; i < n; i++){
    if (vertices[i].y < menor){
      menor = vertices[i].y;
      ind_menor = i;
    }else if (vertices[i].y == menor){
      if (vertices[i].x <= vertices[ind_menor].x){
        menor = vertices[i].y;
        ind_menor = i;
      }
    }
  }
  troca(&vertices[0], &vertices[ind_menor]);
  p0 = vertices[0];

  //ordena n - 1 vértices em relação ao primeiro vértice
  //vértice v1 fica antes de v2 se v2 tem ângulo polar maior que p1 (sentido anti-horário)
  qsort(&vertices[1], n - 1, sizeof(struct t_vertice), compara);

  //se dois ou mais pontos têm mesmo ângulo em relação a p0 remove todos menos o que está mais longe de p0
  //na ordenação se dois ou mais pontos têm mesmo ângulo o ponto mais longe é mantido

  //variável guardará tamanho do vetor modificado
  int m = 1;
  for (int i = 1; i < n; i++){
    //laço que pula i se vertices[i], vertices[i+1] e p0 forem colineares
    while (i < (n-1) && orientacao(p0, vertices[i], vertices[i+1]) == 0){
      i++;
    }
    vertices[m] = vertices[i];
    m++;
  }

  //não podemos ter fecho convexo com menos de 3 pontos
  if(m < 3){
    exit(0);
  }

  //cria uma pilha vazia e dá push nos primeiros 3 pontos
  struct t_pilha pilha = criaPilha(3);
  push(&pilha, vertices[0]);
  push(&pilha, vertices[1]);
  push(&pilha, vertices[2]);

  //agora processo os n-3 vértices restantes (erro aqui)
  for(int i = 3; i < m; i++){
    //remove vértice do topo enquanto o ângulo formado pelo vértice do topo, vértice anterior e vertice[i] fizer um desvio que não seja para a esquerda
    while(orientacao(pegaAntTopo(&pilha), pegaTopo(&pilha), vertices[i]) != 2){
      pop(&pilha);
    }
    push(&pilha, vertices[i]);
  }
  return pilha;
}

void escreveFecho(struct t_pilha pilha, FILE *f, char* nome_arq){
  f = fopen(nome_arq, "w");
  struct t_vertice v;
  while(!estaVazia(&pilha)){
    v = pegaTopo(&pilha);
    fprintf(f, "%f ", v.x);
    fprintf(f, "%f\n", v.y);
    pop(&pilha);
  }
  fclose(f);
}

int main(){
  char* nome_vertices = "points.txt";

  char* nome_fecho = "loop.txt";

  FILE *f_vertices;

  FILE *f_fecho;

  int qtd_vertices;

  struct t_vertice *vertices;

  leVertices(f_vertices, nome_vertices, &vertices, &qtd_vertices);

  struct t_pilha fecho = fechoConvexo(vertices, qtd_vertices); //está imprimindo os vértices errados

  imprimeFecho(fecho);

  escreveFecho(fecho, f_fecho, nome_fecho);

  free(vertices);
}
