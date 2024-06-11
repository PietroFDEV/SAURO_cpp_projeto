#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 100
#define MAX_LINHA 256

typedef struct Aluno {
    int id;
    char nome[MAX_NOME];
    char senha[MAX_NOME];
    struct Aluno* next;
} Aluno;

typedef struct Curso {
    int id;
    char nome[MAX_NOME];
    float precoPorMes;
    int numParcelas;
    struct Curso* next;
} Curso;

typedef struct Matricula {
    int id;
    int idAluno;
    int idCurso;
    int meses; // Data de expiração para a matrícula
    struct Matricula* next;
} Matricula;

// Protótipos de Função
void cadastrar_aluno(Aluno** alunos);
void cadastrar_curso(Curso** cursos);
void cadastrar_matricula(Matricula** matriculas, int idAluno, Curso* cursos);
Aluno* carregar_alunos();
Curso* carregar_cursos();
void editar_aluno(Aluno* alunos);
void editar_curso(Curso* cursos);
void editar_matricula(Matricula* matriculas, int idMatricula, Curso* cursos, Aluno* alunos);
void excluir_aluno(Aluno** alunos, Matricula** matriculas);
void excluir_curso(Curso** cursos, Matricula** matriculas);
void excluir_matricula(Matricula** matriculas, int idAluno);
void exibir_alunos(Aluno* alunos);
void exibir_cursos(Curso* cursos);
void exibir_matriculas(int idAluno, Curso* cursos, Aluno* alunos, Matricula* matriculas);
void limpar_console();
void menu_admin(Aluno** alunos, Curso** cursos, Matricula** matriculas);
void menu_aluno(int idAluno, Aluno* alunos, Curso* cursos, Matricula* matriculas);
void menu_login();
int login(Aluno* alunos);
int encontrar_proximo_id(Aluno* alunos);
int encontrar_proximo_id_curso(Curso* cursos);
int encontrar_proximo_id_matricula(Matricula* matriculas);
time_t parse_date(const char* date_str);
Matricula* carregar_matriculas();
char* encontrar_nome_aluno(int idAluno, Aluno* alunos);
char* encontrar_nome_curso(int idCurso, Curso* cursos);
void salvar_alunos(Aluno* alunos);
void salvar_cursos(Curso* cursos);
void salvar_matriculas(Matricula* matriculas);
time_t calcular_data_expiracao();


void limpar_console() {
  #ifdef _WIN32
  system("cls");
  #else
  system("clear");
  #endif
}

int login(Aluno* alunos) {
  char nome[MAX_NOME];
  char senha[MAX_NOME];
  printf("Digite o nome de usuário: ");
  fgets(nome, MAX_NOME, stdin);
  nome[strcspn(nome, "\n")] = 0; // Remove o caractere de nova linha
  printf("Digite a senha: ");
  fgets(senha, MAX_NOME, stdin);
  senha[strcspn(senha, "\n")] = 0; // Remove o caractere de nova linha

  if (strcmp("admin", nome) == 0 && strcmp("admin", senha) == 0) {
    return -1; // Usuário administrador
  }

  Aluno* current = alunos;
  while (current != NULL) {
    if (strcmp(current->nome, nome) == 0 && strcmp(current->senha, senha) == 0) {
      return current->id; // Usuário aluno
    }
    current = current->next;
  }

  return 0; // Falha no login
}

void menu_login() {
  printf("\n======= Menu de Login =======\n");
  printf("1. Entrar\n");
  printf("2. Sair\n");
  printf("Escolha uma opção: ");
}

void menu_aluno(int idAluno, Aluno* alunos, Curso* cursos, Matricula* matriculas) {
  int opcao;
  do {
    printf("\n======= Menu do Aluno =======\n");
    printf("1. Ver cursos disponíveis\n");
    printf("2. Ver minhas matrículas\n");
    printf("3. Adicionar nova matrícula\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", & opcao);
    getchar(); // Limpa o buffer de entrada
    limpar_console();

    switch (opcao) {
    case 1:
      exibir_cursos(cursos);
      break;
    case 2:
      exibir_matriculas(idAluno, cursos, alunos, matriculas);
      break;
    case 3:
      cadastrar_matricula(&matriculas, idAluno, cursos);
      break;
    case 4:
      printf("Saindo...\n");
      break;
    default:
      printf("Opção inválida. Por favor, escolha uma opção válida.\n");
    }
  } while (opcao != 4);
}

void menu_admin(Aluno** alunos, Curso** cursos, Matricula** matriculas) {
  int opcao_admin_menu;
  int opcao_admin;
  do {
    printf("\n======= Menu de Admin =======\n");
    printf("1. Gerenciar alunos\n");
    printf("2. Gerenciar cursos\n");
    printf("3. Gerenciar matrículas\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao_admin_menu);
    getchar(); // Limpa o buffer de entrada
    limpar_console();

    switch (opcao_admin_menu) {
    case 1:
      // Gerenciamento de Alunos
      printf("\n======= Gerenciamento de Alunos =======\n");
      printf("1. Adicionar aluno\n");
      printf("2. Editar aluno\n");
      printf("3. Excluir aluno\n");
      printf("4. Ver alunos\n");
      printf("Escolha uma opção: ");
      scanf("%d", &opcao_admin);
      getchar(); // Limpa o buffer de entrada
      switch (opcao_admin) {
      case 1:
        cadastrar_aluno(alunos);
        break;
      case 2:
        editar_aluno(*alunos);
        break;
      case 3:
        excluir_aluno(alunos, matriculas);
        break;
      case 4:
        exibir_alunos(*alunos);
        break;
      default:
        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
      }
      break;
    case 2:
      // Gerenciamento de Cursos
      printf("\n======= Gerenciamento de Cursos =======\n");
      printf("1. Adicionar curso\n");
      printf("2. Editar curso\n");
      printf("3. Excluir curso\n");
      printf("4. Ver cursos\n");
      printf("Escolha uma opção: ");
      scanf("%d", &opcao_admin);
      getchar(); // Limpa o buffer de entrada
      switch (opcao_admin) {
      case 1:
        cadastrar_curso(cursos);
        break;
      case 2:
        editar_curso(*cursos);
        break;
      case 3:
        excluir_curso(cursos, matriculas);
        break;
      case 4:
        exibir_cursos(*cursos);
        break;
      default:
        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
      }
      break;
    case 3:
      // Gerenciamento de Matrículas
      printf("\n======= Gerenciamento de Matrículas =======\n");
      printf("1. Ver matrículas\n");
      printf("2. Remover matrícula\n");
      printf("3. Editar matrícula\n");
      printf("Escolha uma opção: ");
      scanf("%d", &opcao_admin);
      getchar(); // Limpa o buffer de entrada
      switch (opcao_admin) {
      case 1:
        exibir_matriculas(-1, *cursos, *alunos, *matriculas); // Exibir todas as matrículas
        break;
      case 2:
        printf("Digite o ID do aluno para remover a matrícula: ");
        int id;
        scanf("%d", &id);
        getchar(); // Limpa o buffer de entrada
        excluir_matricula(matriculas, id);
        break;
      case 3:
        printf("Digite o ID da matrícula para editar: ");
        int idMatricula;
        scanf("%d", &idMatricula);
        getchar(); // Limpa o buffer de entrada
        editar_matricula(*matriculas, idMatricula, *cursos, *alunos);
        break;
      default:
        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
      }
      break;
    case 4:
      printf("Saindo...\n");
      break;
    default:
      printf("Opção inválida. Por favor, escolha uma opção válida.\n");
    }
  } while (opcao_admin_menu != 4);
}

void exibir_cursos(Curso* cursos) {
  limpar_console();
  printf("\n======= Cursos Disponíveis =======\n");
  Curso* current = cursos;
  while (current != NULL) {
    printf("ID: %d, Nome: %s, Preço por mês: %.2f, Parcelas: %d\n", current->id, current->nome, current->precoPorMes, current->numParcelas);
    current = current->next;
  }
}

void exibir_alunos(Aluno* alunos) {
  limpar_console();
  printf("\n======= Alunos Cadastrados =======\n");
  Aluno* current = alunos;
  while (current != NULL) {
    printf("ID: %d, Nome: %s\n", current->id, current->nome);
    current = current->next;
  }
}

void exibir_matriculas(int idAluno, Curso* cursos, Aluno* alunos, Matricula* matriculas) {
  limpar_console();
  printf("\n======= Matrículas =======\n");
  Matricula* current = matriculas;
  while (current != NULL) {
    if (idAluno == -1 || current->idAluno == idAluno) {
      char* nomeAluno = encontrar_nome_aluno(current->idAluno, alunos);
      char* nomeCurso = encontrar_nome_curso(current->idCurso, cursos);
      struct tm* dataExp = localtime(&current->dataExpiracao);
      char dataExpStr[100];
      strftime(dataExpStr, sizeof(dataExpStr), "%d/%m/%Y", dataExp);
      printf("ID: %d, Aluno: %s, Curso: %s, Data de Expiração: %s\n", current->id, nomeAluno, nomeCurso, dataExpStr);
    }
    current = current->next;
  }
}

void cadastrar_aluno(Aluno** alunos) {
  Aluno* novo_aluno = (Aluno*)malloc(sizeof(Aluno));
  novo_aluno->id = encontrar_proximo_id(*alunos);
  printf("Digite o nome do aluno: ");
  fgets(novo_aluno->nome, MAX_NOME, stdin);
  novo_aluno->nome[strcspn(novo_aluno->nome, "\n")] = 0; // Remove o caractere de nova linha
  printf("Digite a senha do aluno: ");
  fgets(novo_aluno->senha, MAX_NOME, stdin);
  novo_aluno->senha[strcspn(novo_aluno->senha, "\n")] = 0; // Remove o caractere de nova linha
  novo_aluno->next = *alunos;
  *alunos = novo_aluno;
  salvar_alunos(*alunos);
  printf("Aluno cadastrado com sucesso.\n");
}

void cadastrar_curso(Curso** cursos) {
  Curso* novo_curso = (Curso*)malloc(sizeof(Curso));
  novo_curso->id = encontrar_proximo_id_curso(*cursos);
  printf("Digite o nome do curso: ");
  fgets(novo_curso->nome, MAX_NOME, stdin);
  novo_curso->nome[strcspn(novo_curso->nome, "\n")] = 0; // Remove o caractere de nova linha
  printf("Digite o preço por mês do curso: ");
  scanf("%f", &novo_curso->precoPorMes);
  printf("Digite o número de parcelas do curso: ");
  scanf("%d", &novo_curso->numParcelas);
  getchar(); // Limpa o buffer de entrada
  novo_curso->next = *cursos;
  *cursos = novo_curso;
  salvar_cursos(*cursos);
  printf("Curso cadastrado com sucesso.\n");
}

void cadastrar_matricula(Matricula** matriculas, int idAluno, Curso* cursos) {
  Matricula* nova_matricula = (Matricula*)malloc(sizeof(Matricula));
  nova_matricula->id = encontrar_proximo_id_matricula(*matriculas);
  nova_matricula->idAluno = idAluno;
  exibir_cursos(cursos);
  printf("Digite o ID do curso: ");
  scanf("%d", &nova_matricula->idCurso);
  getchar(); // Limpa o buffer de entrada
  nova_matricula->dataExpiracao = calcular_data_expiracao();
  nova_matricula->next = *matriculas;
  *matriculas = nova_matricula;
  salvar_matriculas(*matriculas);
  printf("Matrícula cadastrada com sucesso.\n");
}

void editar_aluno(Aluno* alunos) {
  int id;
  printf("Digite o ID do aluno a ser editado: ");
  scanf("%d", &id);
  getchar(); // Limpa o buffer de entrada
  Aluno* current = alunos;
  while (current != NULL) {
    if (current->id == id) {
      printf("Digite o novo nome do aluno: ");
      fgets(current->nome, MAX_NOME, stdin);
      current->nome[strcspn(current->nome, "\n")] = 0; // Remove o caractere de nova linha
      printf("Digite a nova senha do aluno: ");
      fgets(current->senha, MAX_NOME, stdin);
      current->senha[strcspn(current->senha, "\n")] = 0; // Remove o caractere de nova linha
      salvar_alunos(alunos);
      printf("Aluno editado com sucesso.\n");
      return;
    }
    current = current->next;
  }
  printf("Aluno não encontrado.\n");
}

void editar_curso(Curso* cursos) {
  int id;
  printf("Digite o ID do curso a ser editado: ");
  scanf("%d", &id);
  getchar(); // Limpa o buffer de entrada
  Curso* current = cursos;
  while (current != NULL) {
    if (current->id == id) {
      printf("Digite o novo nome do curso: ");
      fgets(current->nome, MAX_NOME, stdin);
      current->nome[strcspn(current->nome, "\n")] = 0; // Remove o caractere de nova linha
      printf("Digite o novo preço por mês do curso: ");
      scanf("%f", &current->precoPorMes);
      printf("Digite o novo número de parcelas do curso: ");
      scanf("%d", &current->numParcelas);
      getchar(); // Limpa o buffer de entrada
      salvar_cursos(cursos);
      printf("Curso editado com sucesso.\n");
      return;
    }
    current = current->next;
  }
  printf("Curso não encontrado.\n");
}

void editar_matricula(Matricula* matriculas, int idMatricula, Curso* cursos, Aluno* alunos) {
  Matricula* current = matriculas;
  while (current != NULL) {
    if (current->id == idMatricula) {
      printf("Digite o novo ID do aluno: ");
      scanf("%d", &current->idAluno);
      printf("Digite o novo ID do curso: ");
      scanf("%d", &current->idCurso);
      getchar(); // Limpa o buffer de entrada
      current->dataExpiracao = calcular_data_expiracao();
      salvar_matriculas(matriculas);
      printf("Matrícula editada com sucesso.\n");
      return;
    }
    current = current->next;
  }
  printf("Matrícula não encontrado.\n");
}

void excluir_aluno(Aluno** alunos, Matricula** matriculas) {
  int id;
  printf("Digite o ID do aluno a ser excluído: ");
  scanf("%d", &id);
  getchar(); // Limpa o buffer de entrada
  Aluno* current = *alunos;
  Aluno* previous = NULL;
  while (current != NULL) {
    if (current->id == id) {
      if (previous == NULL) {
        *alunos = current->next;
      } else {
        previous->next = current->next;
      }
      free(current);
      salvar_alunos(*alunos);
      excluir_matricula(matriculas, id); // Excluir matrículas do aluno
      printf("Aluno excluído com sucesso.\n");
      return;
    }
    previous = current;
    current = current->next;
  }
  printf("Aluno não encontrado.\n");
}

void excluir_curso(Curso** cursos, Matricula** matriculas) {
  int id;
  printf("Digite o ID do curso a ser excluído: ");
  scanf("%d", &id);
  getchar(); // Limpa o buffer de entrada
  Curso* current = *cursos;
  Curso* previous = NULL;
  while (current != NULL) {
    if (current->id == id) {
      if (previous == NULL) {
        *cursos = current->next;
      } else {
        previous->next = current->next;
      }
      free(current);
      salvar_cursos(*cursos);
      printf("Curso excluído com sucesso.\n");
      return;
    }
    previous = current;
    current = current->next;
  }
  printf("Curso não encontrado.\n");
}

void excluir_matricula(Matricula** matriculas, int idAluno) {
  Matricula* current = *matriculas;
  Matricula* previous = NULL;
  while (current != NULL) {
    if (current->idAluno == idAluno) {
      if (previous == NULL) {
        *matriculas = current->next;
      } else {
        previous->next = current->next;
      }
      free(current);
      salvar_matriculas(*matriculas);
      printf("Matrícula excluída com sucesso.\n");
      return;
    }
    previous = current;
    current = current->next;
  }
  printf("Matrícula não encontrada.\n");
}

int encontrar_proximo_id(Aluno* alunos) {
  int max_id = 0;
  Aluno* current = alunos;
  while (current != NULL) {
    if (current->id > max_id) {
      max_id = current->id;
    }
    current = current->next;
  }
  return max_id + 1;
}

int encontrar_proximo_id_curso(Curso* cursos) {
  int max_id = 0;
  Curso* current = cursos;
  while (current != NULL) {
    if (current->id > max_id) {
      max_id = current->id;
    }
    current = current->next;
  }
  return max_id + 1;
}

int encontrar_proximo_id_matricula(Matricula* matriculas) {
  int max_id = 0;
  Matricula* current = matriculas;
  while (current != NULL) {
    if (current->id > max_id) {
      max_id = current->id;
    }
    current = current->next;
  }
  return max_id + 1;
}

Aluno* carregar_alunos() {
    FILE* file = fopen("alunos.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de alunos");
        return NULL;
    }

    Aluno* head = NULL;
    Aluno* current = NULL;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), file)) {
        Aluno* novo_aluno = (Aluno*)malloc(sizeof(Aluno));
        sscanf(linha, "%d,%99[^,],%99[^\n]", &novo_aluno->id, novo_aluno->nome, novo_aluno->senha);
        novo_aluno->next = NULL;

        if (!head) {
            head = novo_aluno;
        } else {
            current->next = novo_aluno;
        }
        current = novo_aluno;
    }

    fclose(file);
    return head;
}

Curso* carregar_cursos() {
    FILE* file = fopen("cursos.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de cursos");
        return NULL;
    }

    Curso* head = NULL;
    Curso* current = NULL;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), file)) {
        Curso* novo_curso = (Curso*)malloc(sizeof(Curso));
        sscanf(linha, "%d,%99[^,],%f,%d", &novo_curso->id, novo_curso->nome, &novo_curso->precoPorMes, &novo_curso->numParcelas);
        novo_curso->next = NULL;

        if (!head) {
            head = novo_curso;
        } else {
            current->next = novo_curso;
        }
        current = novo_curso;
    }

    fclose(file);
    return head;
}

time_t parse_date(const char* date_str) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    if (sscanf(date_str, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3) {
        return (time_t)-1;
    }
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    return mktime(&tm);
}

Matricula* carregar_matriculas() {
    FILE* file = fopen("matriculas.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de matrículas");
        return NULL;
    }

    Matricula* head = NULL;
    Matricula* current = NULL;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), file)) {
        Matricula* nova_matricula = (Matricula*)malloc(sizeof(Matricula));
        char data_str[MAX_NOME];

        sscanf(linha, "%d %d %d %ld", &nova_matricula->id, &nova_matricula->idAluno, &nova_matricula->idCurso, &nova_matricula->dataExpiracao);
        nova_matricula->next = NULL;

        if (!head) {
            head = nova_matricula;
        } else {
            current->next = nova_matricula;
        }
        current = nova_matricula;
    }

    fclose(file);
    return head;
}

char* encontrar_nome_aluno(int idAluno, Aluno* alunos) {
  while (alunos != NULL) {
    if (alunos->id == idAluno) {
      return alunos->nome;
    }
    alunos = alunos->next;
  }
  char* error_aluno_nao_encontrado = "Aluno não encontrado";
  return error_aluno_nao_encontrado;
}

char* encontrar_nome_curso(int idCurso, Curso* cursos) {
  while (cursos != NULL) {
    if (cursos->id == idCurso) {
      return cursos->nome;
    }
    cursos = cursos->next;
  }
  char* error_curso_nao_encontrado = "Curso não encontrado";
  return error_curso_nao_encontrado;
}

void salvar_alunos(Aluno* alunos) {
  FILE * arquivo = fopen("alunos.txt", "w");
  if (arquivo != NULL) {
    while (alunos != NULL) {
      fprintf(arquivo, "%d %s %s\n", alunos->id, alunos->nome, alunos->senha);
      alunos = alunos->next;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados dos alunos.\n");
  }
}

void salvar_cursos(Curso* cursos) {
  FILE * arquivo = fopen("cursos.txt", "w");
  if (arquivo != NULL) {
    while (cursos != NULL) {
      fprintf(arquivo, "%d %s %.2f %d\n", cursos->id, cursos->nome, cursos->precoPorMes, cursos->numParcelas);
      cursos = cursos->next;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados dos cursos.\n");
  }
}

void salvar_matriculas(Matricula* matriculas) {
  FILE *arquivo = fopen("matriculas.txt", "w");
  if (arquivo != NULL) {
    while (matriculas != NULL) {
      fprintf(arquivo, "%d %d %d %ld\n", matriculas->id, matriculas->idAluno, matriculas->idCurso, matriculas->dataExpiracao);
      matriculas = matriculas->next;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados das matrículas.\n");
  }
}

time_t calcular_data_expiracao() {
  time_t agora = time(NULL);
  struct tm * tempo_atual = localtime( & agora);
  tempo_atual -> tm_mon++; // Incrementa o mês já que janeiro é representado por 0
  tempo_atual -> tm_year += tempo_atual -> tm_mon / 12; // Ajusta o ano se o mês exceder 12
  tempo_atual -> tm_mon %= 12; // Garante que o mês esteja entre 0 e 11
  return mktime(tempo_atual);
}

int main() {
    Aluno* alunos = NULL;
    Curso* cursos = NULL;
    Matricula* matriculas = NULL;

    int opcao;
    do {
        alunos = carregar_alunos();
        cursos = carregar_cursos();
        matriculas = carregar_matriculas();
        limpar_console();
        menu_login();
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada
        limpar_console();

        switch (opcao) {
            case 1: {
                int id = login(alunos);
                limpar_console();
                if (id == -1) {
                    menu_admin(&alunos, &cursos, &matriculas);
                } else if (id > 0) {
                    menu_aluno(id, alunos, cursos, matriculas);
                } else {
                    printf("Nome de usuário ou senha inválidos. Por favor, tente novamente.\n");
                }
                break;
            }
            case 2:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    } while (opcao != 2);

    return 0;
}
