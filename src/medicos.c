#include "../includes/medicos.h"

struct medico{
  char nome[50];
  char especialidade[50];
  Paciente *pacientes;
  char disponibilidade[50];
  Medico *prox;
};

Medico *cadastroPaciente(Medico *lista) {
    char nomeMedico[50];
    Medico *aux;

    do {
        printf("Digite o nome do medico: ");
        scanf(" %[^\n]", nomeMedico);
        getchar(); 

        if (!isStringValida(nomeMedico)) {
            printf("Nome do medico invalido. Por favor, insira apenas letras e espacos.\n");
        } else {
            aux = buscaMedico(lista, nomeMedico);

            if (aux == NULL) {
                printf("Medico nao cadastrado, vamos cadastrar.\n");
                Medico m = preencheMedico();
                lista = insereOrdenado(lista, m);
            }

            break; 
        }
    } while (1); 

    Paciente p;

    if (aux == NULL) {
        printf("Agora insira os dados do paciente.\n");
        p = preenchePaciente();
        aux = buscaMedico(lista, nomeMedico);
        aux->pacientes = inserePacienteOrdenado(aux->pacientes, p);
    } else {
        printf("Agora insira os dados do paciente.\n");
        p = preenchePaciente();
        aux->pacientes = inserePacienteOrdenado(aux->pacientes, p);
    }

    return lista;
}

Medico *buscaMedico(Medico *lista, char *nome){
  Medico *p;
  for (p = lista; p != NULL; p = p->prox){
    if (strcmp(p->nome, nome) == 0){
      return p;
    }
  }
  return NULL;
}

Medico preencheMedico() {
    Medico m;

    do {
        printf("Digite o nome do medico: ");
        scanf(" %[^\n]", m.nome);
        if (!isStringValida(m.nome)) {
            printf("Nome invalido. Por favor, insira apenas letras e espacos.\n");
        }
    } while (!isStringValida(m.nome));

    do {
        printf("Digite a especialidade do medico: ");
        scanf(" %[^\n]", m.especialidade);
        if (!isStringValida(m.especialidade)) {
            printf("Especialidade invalida. Por favor, insira apenas letras e espacos.\n");
        }
    } while (!isStringValida(m.especialidade));

    do {
        printf("Digite a disponibilidade do medico (formato exemplo: 15h as 20h): ");
        scanf(" %[^\n]", m.disponibilidade);
        if (!isDisponibilidadeMedicoValida(m.disponibilidade)) {
            printf("Disponibilidade invalida. Por favor, insira no formato correto (ex: 15h as 20h).\n");
        }
    } while (!isDisponibilidadeMedicoValida(m.disponibilidade));

    m.pacientes = NULL;
    m.prox = NULL;
    return m;
}

Medico *insereOrdenado(Medico *lista, Medico m){
  Medico *novo = (Medico *)malloc(sizeof(Medico));
  if (novo == NULL){
    perror("Erro ao alocar memoria");
    exit(1);
  }
  strcpy(novo->nome, m.nome);
  strcpy(novo->especialidade, m.especialidade);
  strcpy(novo->disponibilidade, m.disponibilidade);
  novo->prox = NULL;

  if (lista == NULL || strcmp(lista->nome, novo->nome) > 0){
    novo->prox = lista;
    printf("Medico %s cadastrado com sucesso!\n", novo->nome);
    return novo;
  }

  Medico *ant = NULL;
  Medico *atual = lista;

  while (atual != NULL && strcmp(atual->nome, m.nome) < 0){
    ant = atual;
    atual = atual->prox;
  }

  ant->prox = novo;
  novo->prox = atual;
  printf("Medico %s cadastrado com sucesso!\n", novo->nome);
  return lista;
}

void imprimeMedicos(Medico *lista)
{
  if (lista == NULL)
  {
    printf("Lista vazia\n");
    return;
  }

  Medico *atual = lista;
  while (atual != NULL){
    printf("Medico: %s, Especialidade: %s, Disponibilidade: %s\n", atual->nome, atual->especialidade, atual->disponibilidade);
    imprimePacientes(atual->pacientes);
    atual = atual->prox;
  }
}

Medico *removePacienteDoMedico(Medico *lista) {
    if (lista == NULL) {
        printf("Lista vazia\n");
        return lista;
    }

    imprimeMedicos(lista);

    char nomeMedico[50];
    char nomePaciente[50];

    do {
        printf("Qual o medico do paciente que voce deseja remover? ");
        scanf(" %[^\n]", nomeMedico);
        getchar(); 

        if (!isStringValida(nomeMedico)) {
            printf("Nome do medico invalido. Por favor, insira apenas letras e espacos.\n");
        } else {
            break; 
        }
    } while (1); 

    do {
        printf("Qual o paciente que voce deseja remover? ");
        scanf(" %[^\n]", nomePaciente);
        getchar(); 

        if (!isStringValida(nomePaciente)) {
            printf("Nome do paciente invalido. Por favor, insira apenas letras e espacos.\n");
        } else {
            break; 
        }
    } while (1); 

    Medico *aux = buscaMedico(lista, nomeMedico);
    if (aux == NULL){
        printf("Medico nao encontrado\n");
        return lista;
    }

    aux->pacientes = removePaciente(aux->pacientes, nomePaciente);
    return lista;
}

Medico *editarPacientePorMedico(Medico *lista, char *nomeMedico, char *nomePaciente){
  Medico *aux = buscaMedico(lista, nomeMedico);
  if (aux == NULL){
    printf("Medico nao encontrado\n");
    return lista;
  }
  aux->pacientes = editarPaciente(aux->pacientes, nomePaciente);
  return lista;
}

Medico *removeMedico(Medico *lista, char *nome){
  Medico *ant = NULL;
  Medico *p = lista;
  while (p != NULL && strcmp(p->nome, nome) != 0){
    ant = p;
    p = p->prox;
  }
  if (p == NULL){
    printf("Medico nao encontrado\n");
    return lista;
  }
  if (ant == NULL){
    lista = p->prox;
  }else{
    ant->prox = p->prox;
  }
  free(p);
  printf("Medico %s removido com sucesso\n", nome);
  return lista;
}

void buscaPacientePorMedico(Medico *lista, char *nomeMedico, char *nomePaciente){
  Medico *aux = buscaMedico(lista, nomeMedico);
  if (aux == NULL){
    printf("Medico nao encontrado\n");
    return;
  }
  Paciente *p = buscaPaciente(aux->pacientes, nomePaciente);
  if (p == NULL){
    printf("Paciente nao encontrado\n");
    return;
  }
  imprimePacientes(p);
}

void escreveArquivo(Medico *lista, char *localDoArquivo){
  FILE *arquivo = fopen(localDoArquivo, "wt");
  if(arquivo == NULL){
    perror("Erro ao abrir o arquivo");
    exit(1);
  }

  Medico *aux = lista;
  while(aux != NULL){
    fprintf(arquivo, "Medico:%s\t%s\t%s\n", aux->nome, aux->especialidade, aux->disponibilidade);
    Paciente *p = aux->pacientes;
    while(p != NULL){
      fprintf(arquivo, "Paciente:%s\t%d\t%s\n", p->nome, p->idade, p->doenca);
      p = p->prox;
    }
    aux = aux->prox;
  }
  fclose(arquivo);
}

Medico *lerArquivo(char *localDoArquivo, Medico *lista){
  FILE *arquivo = fopen(localDoArquivo, "r");
  if (arquivo == NULL){
    perror("Erro ao abrir o arquivo");
    exit(1);
  }
  char linha[200];
  Medico m;
  Paciente p;
  while (fgets(linha, 200, arquivo) != NULL){
    Medico *aux;
    if (strstr(linha, "Medico:") != NULL){
      sscanf(linha, "Medico:%[^\t]\t%[^\t]\t%[^\n]", m.nome, m.especialidade, m.disponibilidade);
      m.prox = NULL;
      m.pacientes = NULL;
      lista = insereOrdenado(lista, m);
      aux = buscaMedico(lista, m.nome);
    }
    else{
      sscanf(linha, "Paciente:%[^\t]%d\t%[^\n]", p.nome, &p.idade, p.doenca);
      aux->pacientes = inserePacienteOrdenado(aux->pacientes, p);
    }
  }
  fclose(arquivo);
  return lista;
}

void liberaLista(Medico* lista) {
  Medico* medicoAtual = lista;
  Medico* proximoMedico;

  while (medicoAtual != NULL) {
    Paciente* pacienteAtual = medicoAtual->pacientes;
    Paciente* proximoPaciente;

    while (pacienteAtual != NULL) {
      proximoPaciente = pacienteAtual->prox;
      free(pacienteAtual);
      pacienteAtual = proximoPaciente;
    }

    proximoMedico = medicoAtual->prox;
    free(medicoAtual);
    medicoAtual = proximoMedico;
  }
}