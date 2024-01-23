#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int numero;
  int duracao;
  int instante;
} Atualizacao;

void trocar(Atualizacao *a, Atualizacao *b) {
  Atualizacao t = *a;
  *a = *b;
  *b = t;
}

int particionar(Atualizacao arr[], int baixo, int alto) {
  int pivô = arr[alto].duracao;
  int i = (baixo - 1);

  for (int j = baixo; j <= alto - 1; j++) {
    if (arr[j].duracao <= pivô) {
      i++;
      trocar(&arr[i], &arr[j]);
    }
  }
  trocar(&arr[i + 1], &arr[alto]);
  return (i + 1);
}

void quicksort(Atualizacao arr[], int baixo, int alto) {
  if (baixo < alto) {
    int p = particionar(arr, baixo, alto);
    quicksort(arr, baixo, p - 1);
    quicksort(arr, p + 1, alto);
  }
}

void processarOperacoes(FILE *arquivo, Atualizacao *atualizacoes, int *quantidadeAtualizacoes, int numOp) {
  for (int i = 0; i < numOp; i++) {
    char operacao;
    int instante, numAtualizacao, novaDuracao;

    if (fscanf(arquivo, " %c %d %d %d", &operacao, &instante, &numAtualizacao, &novaDuracao) != 4) {
      fprintf(stderr, "Erro na leitura da operacao %d\n", i + 1);
      exit(1);
    }

    if (operacao == 'c') {
      for (int j = 0; j < *quantidadeAtualizacoes; j++) {
        if (atualizacoes[j].numero == numAtualizacao) {
          atualizacoes[j].duracao = novaDuracao;
          atualizacoes[j].instante = instante;
          break;
        }
      }
    } else if (operacao == 'i') {
      (*quantidadeAtualizacoes)++;
      atualizacoes[*quantidadeAtualizacoes - 1].numero = numAtualizacao;
      atualizacoes[*quantidadeAtualizacoes - 1].duracao = novaDuracao;
      atualizacoes[*quantidadeAtualizacoes - 1].instante = instante;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Uso: %s <duracao_periodo> <cenario> <arquivo_entrada>\n", argv[0]);
    return 1;
  }

  int duracao = atoi(argv[1]);
  int cenario = atoi(argv[2]);
  char *arquivoEntrada = argv[3];

  FILE *arquivo = fopen(arquivoEntrada, "r");
  if (arquivo == NULL) {
    perror("Erro ao abrir arquivo de entrada");
    return 1;
  }

  int numOperacoes;
  int lineCounter = 0;
  char ch;
  while ((ch = fgetc(arquivo)) != EOF) {
    if (ch == '\n') {
      lineCounter++;
    }
  }
  rewind(arquivo);
  numOperacoes = lineCounter - 1;

  int quantidadeAtualizacoes;
  fscanf(arquivo, "%d", &quantidadeAtualizacoes);

  Atualizacao *atualizacoes = (Atualizacao *)malloc(quantidadeAtualizacoes * sizeof(Atualizacao));

  for (int i = 0; i < quantidadeAtualizacoes; i++) {
    fscanf(arquivo, "%d", &atualizacoes[i].duracao);
    atualizacoes[i].numero = i + 1;
    atualizacoes[i].instante = 0;
  }

  if (cenario == 2) {
    processarOperacoes(arquivo, atualizacoes, &quantidadeAtualizacoes, numOperacoes);
  }

  fclose(arquivo);

  quicksort(atualizacoes, 0, quantidadeAtualizacoes - 1);

  int duracaoTotal = 0;
  int quantidadeAtt = 0;
  int resp[100];

  for (int i = 0; i < quantidadeAtualizacoes; i++) {
    if (atualizacoes[i].instante >= duracao) {
      break;
    }

    if (duracaoTotal + atualizacoes[i].duracao <= duracao) {
      duracaoTotal += atualizacoes[i].duracao;
      resp[quantidadeAtt++] = atualizacoes[i].numero;
    }
  }

  FILE *arquivoSaida;
  if (cenario == 1) {
    arquivoSaida = fopen("saida1.txt", "w");
  } else if (cenario == 2) {
    arquivoSaida = fopen("saida2.txt", "w");
  } else {
    fprintf(stderr, "Cenario invalido\n");
    free(atualizacoes);
    return 1;
  }

  if (arquivoSaida == NULL) {
    perror("Erro ao abrir arquivo de saida");
    free(atualizacoes);
    return 1;
  }

  fprintf(arquivoSaida, "%d ", quantidadeAtt);
  for (int i = 0; i < quantidadeAtt; i++) {
    fprintf(arquivoSaida, "%d ", resp[i]);
  }

  fclose(arquivoSaida);

  free(atualizacoes);

  return 0;
}
