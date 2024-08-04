#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define TAM 10

#include "EstruturaVetores.h"

int* vetorPrincipal[TAM];
int acompanharVetor[TAM][2]; //A primeira coluna guarda o tamanho do vetor da estrutura auxiliar, e a segunda guarda a quantidade de índices que já foram preenchidos
int ehPosicaoValida();
int conferirEstruturaVazia();
int conferirExistenciaEstrutura();
void ordenarAposExclusao();

/*
Objetivo: criar estrutura auxiliar na posição 'posicao'.
com tamanho 'tamanho'

Rertono (int)
    SUCESSO - criado com sucesso
    JA_TEM_ESTRUTURA_AUXILIAR - já tem estrutura na posição
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
    SEM_ESPACO_DE_MEMORIA - Sem espaço de memória
    TAMANHO_INVALIDO - o tamanho deve ser maior ou igual a 1
*/
int criarEstruturaAuxiliar(int posicao, int tamanho) {
  int posicao_real = posicao - 1;
  int retorno = 0;

  //Validar tamanho. Não pode ser menor do que 1  
  if(tamanho < 1){
    retorno = TAMANHO_INVALIDO;
    return retorno;
  }
  
  //Validar posição. Precisa existir
  if(ehPosicaoValida(posicao) == POSICAO_INVALIDA){
    retorno = POSICAO_INVALIDA;
    return retorno;
  };

  //Checar se a posição no vetor principal é NULL. Se for, significa que nenhuma estrutura auxiliar foi criada ainda
  if(conferirExistenciaEstrutura(posicao_real) == SEM_ESTRUTURA_AUXILIAR){
    vetorPrincipal[posicao_real] = malloc(tamanho * sizeof(int));//Criar estrutura auxiliar
    if(vetorPrincipal[posicao_real] == NULL){//Checar se houve erro no malloc
      retorno = SEM_ESPACO_DE_MEMORIA;
      return retorno;
    }
    acompanharVetor[posicao_real][0] = tamanho; //A posição da matriz de acompanhamento salva o tamanho da estrutura auxiliar criada
    acompanharVetor[posicao_real][1] = 0; //Salvando a quantidade de casas preenchidas na estrutura auxiliar na outra linha da matriz. Como acabou de ser criada, tem 0 casas preenchidas
    retorno = SUCESSO;
  }else{
    retorno = JA_TEM_ESTRUTURA_AUXILIAR;
  }
 
  // a posicao pode já existir estrutura auxiliar
  //retorno = JA_TEM_ESTRUTURA_AUXILIAR;
  
  // se posição é um valor válido {entre 1 e 10}
  // retorno = POSICAO_INVALIDA;
  
  // o tamanho ser muito grande
  // retorno = SEM_ESPACO_DE_MEMORIA;
  
  // o tamanho nao pode ser menor que 1
  // retorno = TAMANHO_INVALIDO;
  
  // deu tudo certo, crie
  // retorno = SUCESSO;

  return retorno;
}

/*
Objetivo: inserir número 'valor' em estrutura auxiliar da posição 'posicao'
Rertono (int)
    SUCESSO - inserido com sucesso
    SEM_ESPACO - não tem espaço
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
CONSTANTES
*/
int inserirNumeroEmEstrutura(int posicao, int valor) {
  int retorno = 0;
  int existeEstruturaAuxiliar = 0;
  int temEspaco = 0;
  int tamanho;
  int posicao_real = posicao - 1;
  // int posicao_invalida = 0;

  //Validar posição
  if (ehPosicaoValida(posicao) == POSICAO_INVALIDA){
    retorno = POSICAO_INVALIDA;
    return retorno;
  }else{
    // testar se existe a estrutura auxiliar
    if (vetorPrincipal[posicao_real] != NULL) {
      if (acompanharVetor[posicao_real][0] > acompanharVetor[posicao_real][1]) {//Ver se a estrutura auxiliar não foi completamente preenchida
        // Insere, acessando o índice selecionado do vetor principal, que aponta para um vetor auxiliar. No vetor auxiliar, acessa-se o índice logo após o último índice preenchido. Esse valor se encontra na segunda coluna da matriz acompanharVetor, com posicao_real determinando a linha
        vetorPrincipal[posicao_real][acompanharVetor[posicao_real][1]] = valor;
        acompanharVetor[posicao_real][1]++;
        retorno = SUCESSO;
      } else {
        retorno = SEM_ESPACO;
      }
    } else {
      retorno = SEM_ESTRUTURA_AUXILIAR;
      criarEstruturaAuxiliar(posicao, tamanho);
    }
  }

  return retorno;
}

/*
Objetivo: excluir o numero 'valor' da estrutura auxiliar no final da estrutura.
ex: suponha os valores [3, 8, 7, 9,  ,  ]. Após excluir, a estrutura deve ficar
da seguinte forma [3, 8, 7,  ,  ,  ]. Obs. Esta é uma exclusão lógica

Rertono (int)
    SUCESSO - excluido com sucesso
    ESTRUTURA_AUXILIAR_VAZIA - estrutura vazia
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
*/
int excluirNumeroDoFinaldaEstrutura(int posicao) {
  int retorno = SUCESSO;
  int posicao_real = posicao - 1;
  
  if(ehPosicaoValida(posicao) == POSICAO_INVALIDA){
    retorno = POSICAO_INVALIDA;
    return retorno;
  }

  if(conferirExistenciaEstrutura(posicao_real) == SEM_ESTRUTURA_AUXILIAR){
    retorno = SEM_ESTRUTURA_AUXILIAR;
    return retorno;
  }

  if(conferirEstruturaVazia(posicao_real) == ESTRUTURA_AUXILIAR_VAZIA){
    retorno = ESTRUTURA_AUXILIAR_VAZIA;
    return retorno;
  }

  //Excluir
  //Para fazer isso sem diminuir a estrutura auxiliar, se usa INT_MIN para demarcar aquele índice como excluído
  vetorPrincipal[posicao_real][acompanharVetor[posicao_real][1]] = INT_MIN;
  acompanharVetor[posicao_real][1]--; //O demarcador de casas preenchidas é subtraído por 1
  
  return retorno;
}

/*
Objetivo: excluir o numero 'valor' da estrutura auxiliar da posição 'posicao'.
Caso seja excluido, os números posteriores devem ser movidos para as posições
anteriores ex: suponha os valores [3, 8, 7, 9,  ,  ] onde deve ser excluido o
valor 8. A estrutura deve ficar da seguinte forma [3, 7, 9,  ,  ,  ] Obs. Esta é
uma exclusão lógica Rertono (int) SUCESSO - excluido com sucesso 'valor' da
estrutura na posição 'posicao' ESTRUTURA_AUXILIAR_VAZIA - estrutura vazia
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    NUMERO_INEXISTENTE - Número não existe
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar

*/
int excluirNumeroEspecificoDeEstrutura(int posicao, int valor) {
  int retorno = NUMERO_INEXISTENTE;
  int posicao_real = posicao - 1;

  if(ehPosicaoValida(posicao) == POSICAO_INVALIDA){
    retorno = POSICAO_INVALIDA;
    return retorno;
  }

  if(conferirExistenciaEstrutura(posicao_real) == SEM_ESTRUTURA_AUXILIAR){
    retorno = SEM_ESTRUTURA_AUXILIAR;
    return retorno;
  }

  if(conferirEstruturaVazia(posicao_real) == ESTRUTURA_AUXILIAR_VAZIA){
    retorno = ESTRUTURA_AUXILIAR_VAZIA;
    return retorno;
  }

  for(int i = 0; i < (acompanharVetor[posicao_real][0]-1); i++){
    if(vetorPrincipal[posicao_real][i] == valor){
      vetorPrincipal[posicao_real][i] = INT_MIN;
      ordenarAposExclusao(i, posicao_real);
      retorno = SUCESSO;
      break;
    }
  }
  
  return retorno;
}

void ordenarAposExclusao(int i, int posicao_real){
  int aux;
  
  for(; i < acompanharVetor[posicao_real][1]-1; i++){
    aux = vetorPrincipal[posicao_real][i];
    vetorPrincipal[posicao_real][i] = vetorPrincipal[posicao_real][i+1];
    vetorPrincipal[posicao_real][i+1] = aux;
  }

  acompanharVetor[posicao_real][1]--;
}

int conferirEstruturaVazia(int posicao_real){
  int retorno = 0;
  if(acompanharVetor[posicao_real][1] == 0){
    retorno = ESTRUTURA_AUXILIAR_VAZIA;
  }
  return retorno;
}

//Avaliar se o índice da estrutura principal possui uma estrutura auxiliar associada
int conferirExistenciaEstrutura(int posicao_real){
  int retorno = SUCESSO;

  if(vetorPrincipal[posicao_real] == NULL){
    retorno = SEM_ESTRUTURA_AUXILIAR;
  }
  
  return retorno;
}

// se posição é um valor válido {entre 1 e TAM}
int ehPosicaoValida(int posicao) {
  int retorno = 0;
  if (posicao < 1 || posicao > TAM) {
    retorno = POSICAO_INVALIDA;
  } else
    retorno = SUCESSO;

  return retorno;
}
/*
Objetivo: retorna os números da estrutura auxiliar da posição 'posicao (1..10)'.
os números devem ser armazenados em vetorAux

Retorno (int)
    SUCESSO - recuperado com sucesso os valores da estrutura na posição
'posicao' SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar POSICAO_INVALIDA -
Posição inválida para estrutura auxiliar
*/
int getDadosEstruturaAuxiliar(int posicao, int vetorAux[]) {
  int posicao_real = posicao-1;
  int retorno = SUCESSO;

  if(ehPosicaoValida(posicao) == POSICAO_INVALIDA){
    retorno = POSICAO_INVALIDA;
    return retorno;
  }

  if(conferirExistenciaEstrutura(posicao_real) == SEM_ESTRUTURA_AUXILIAR){
    retorno = SEM_ESTRUTURA_AUXILIAR;
    return retorno;
  }
  
  for(int i = 0; i < acompanharVetor[posicao_real][1]; i++){
    vetorAux[i] = vetorPrincipal[posicao_real][i];
  }

  return retorno;
}

/*
Objetivo: retorna os números ordenados da estrutura auxiliar da posição 'posicao
(1..10)'. os números devem ser armazenados em vetorAux

Rertono (int)
    SUCESSO - recuperado com sucesso os valores da estrutura na posição 'posicao
(1..10)' SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar POSICAO_INVALIDA -
Posição inválida para estrutura auxiliar
*/
int getDadosOrdenadosEstruturaAuxiliar(int posicao, int vetorAux[]) {
  int posicao_real = posicao-1;
  int retorno = SUCESSO;

  if(ehPosicaoValida(posicao) == POSICAO_INVALIDA){
    retorno = POSICAO_INVALIDA;
    return retorno;
  }

  if(conferirExistenciaEstrutura(posicao_real) == SEM_ESTRUTURA_AUXILIAR){
    retorno = SEM_ESTRUTURA_AUXILIAR;
    return retorno;
  }

  for(int k = 0; k < acompanharVetor[posicao_real][1]; k++){
    vetorAux[k] = vetorPrincipal[posicao_real][k];
  }

  for (int i = 1; i < acompanharVetor[posicao_real][1]; i++) { 

    int j = i;

    while (j > 0 && vetorAux[j] < vetorAux[j-1]) {
      int aux = vetorAux[j];
      vetorAux[j] = vetorAux[j - 1];
      vetorAux[j - 1] = aux;
      j -= 1;
    }

  }	

  return retorno;
}

/*
Objetivo: retorna os números de todas as estruturas auxiliares.
os números devem ser armazenados em vetorAux

Rertono (int)
    SUCESSO - recuperado com sucesso os valores da estrutura na posição
'posicao' TODAS_ESTRUTURAS_AUXILIARES_VAZIAS - todas as estruturas auxiliares
estão vazias
*/
int getDadosDeTodasEstruturasAuxiliares(int vetorAux[]) {

  int retorno = 0;
  return retorno;
}

/*
Objetivo: retorna os números ordenados de todas as estruturas auxiliares.
os números devem ser armazenados em vetorAux

Rertono (int)
    SUCESSO - recuperado com sucesso os valores da estrutura na posição
'posicao' TODAS_ESTRUTURAS_AUXILIARES_VAZIAS - todas as estruturas auxiliares
estão vazias
*/
int getDadosOrdenadosDeTodasEstruturasAuxiliares(int vetorAux[]) {

  int retorno = 0;
  return retorno;
}

/*
Objetivo: modificar o tamanho da estrutura auxiliar da posição 'posicao' para o
novo tamanho 'novoTamanho' + tamanho atual Suponha o tamanho inicial = x, e novo
tamanho = n. O tamanho resultante deve ser x + n. Sendo que x + n deve ser
sempre >= 1

Rertono (int)
    SUCESSO - foi modificado corretamente o tamanho da estrutura auxiliar
    SEM_ESTRUTURA_AUXILIAR - Não tem estrutura auxiliar
    POSICAO_INVALIDA - Posição inválida para estrutura auxiliar
    NOVO_TAMANHO_INVALIDO - novo tamanho não pode ser negativo
    SEM_ESPACO_DE_MEMORIA - erro na alocação do novo valor
*/
int modificarTamanhoEstruturaAuxiliar(int posicao, int novoTamanho) {

  int retorno = 0;
  return retorno;
}

/*
Objetivo: retorna a quantidade de elementos preenchidos da estrutura auxiliar da
posição 'posicao'.

Retorno (int)
    POSICAO_INVALIDA - posição inválida
    SEM_ESTRUTURA_AUXILIAR - sem estrutura auxiliar
    ESTRUTURA_AUXILIAR_VAZIA - estrutura auxiliar vazia
    Um número int > 0 correpondente a quantidade de elementos preenchidos da
estrutura
*/
int getQuantidadeElementosEstruturaAuxiliar(int posicao) {

  int retorno = 0;

  return retorno;
}

/*
Objetivo: montar a lista encadeada com cabeçote com todos os números presentes
em todas as estruturas.

Retorno (No*)
    NULL, caso não tenha nenhum número nas listas
    No*, ponteiro para o início da lista com cabeçote
*/
No *montarListaEncadeadaComCabecote() { return NULL; }

/*
Objetivo: retorna os números da lista enceada com cabeçote armazenando em
vetorAux. Retorno void
*/
void getDadosListaEncadeadaComCabecote(No *inicio, int vetorAux[]) {}

/*
Objetivo: Destruir a lista encadeada com cabeçote a partir de início.
O ponteiro inicio deve ficar com NULL.

Retorno
    void.
*/
void destruirListaEncadeadaComCabecote(No **inicio) {}

/*
Objetivo: inicializa o programa. deve ser chamado ao inicio do programa

*/

void inicializar() {
  for(int i = 0; i < TAM; i++){
    vetorPrincipal[i] = NULL;
  }
}

/*
Objetivo: finaliza o programa. deve ser chamado ao final do programa
para poder liberar todos os espaços de memória das estruturas auxiliares.

*/

void finalizar() {}
