#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NOME 100
#define MAX_MATRICULAS 100

typedef struct {
  int id;
  char nome[MAX_NOME];
  char senha[MAX_NOME];
}
Aluno;

typedef struct {
  int id;
  char nome[MAX_NOME];
  float precoPorMes;
  int numParcelas;
}
Curso;

typedef struct {
  int id;
  int idAluno;
  int idCurso;
  int meses; // Data de expiração para a matrícula
}
Matricula;

typedef struct {
    int idVencimento;
    int idMatricula;
    time_t dataVencimento;
    float valorParcela;
    int pago; // Boolean para indicar se a parcela foi paga
}
Vencimento;

// Protótipos de Função
void cadastrar_aluno(Aluno alunos[], int * num_alunos);
void cadastrar_curso(Curso cursos[], int * num_cursos);
void cadastrar_matricula(Matricula matriculas[], int * num_matriculas, int idAluno, Curso cursos[], int num_cursos);
void carregar_alunos(Aluno alunos[], int * num_alunos);
void carregar_cursos(Curso cursos[], int * num_cursos);
void carregar_matriculas(Matricula matriculas[], int * num_matriculas);
void editar_aluno(Aluno alunos[], int num_alunos);
void editar_curso(Curso cursos[], int num_cursos);
void editar_matricula(Matricula matriculas[], int num_matriculas, int idMatricula, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos);
void excluir_aluno(Aluno alunos[], int * num_alunos, Matricula matriculas[], int num_matriculas);
void excluir_curso(Curso cursos[], int * num_cursos, Matricula matriculas[], int num_matriculas);
void excluir_matricula(Matricula matriculas[], int * num_matriculas, int idAluno);
void exibir_alunos(Aluno alunos[], int num_alunos);
void exibir_cursos(Curso cursos[], int num_cursos);
void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos, Matricula matriculas[], int num_matriculas);
void limpar_console();
void menu_admin(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas);
void menu_aluno(int idAluno, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas);
void menu_login();
int login(Aluno alunos[], int num_alunos);
int encontrar_proximo_id(Aluno alunos[], int num_alunos);
int encontrar_proximo_id_curso(Curso cursos[], int num_cursos);
int encontrar_proximo_id_matricula(Matricula matriculas[], int num_matriculas);
char * encontrar_nome_aluno(int idAluno, Aluno alunos[], int num_alunos);
char * encontrar_nome_curso(int idCurso, Curso cursos[], int num_cursos);
void salvar_alunos(Aluno alunos[], int num_alunos);
void salvar_cursos(Curso cursos[], int num_cursos);
void salvar_matriculas(Matricula matriculas[], int num_matriculas);
time_t calcular_data_expiracao();

void limpar_console() {
  #ifdef _WIN32
  system("cls");
  #else
  system("clear");
  #endif
}

int login(Aluno alunos[], int num_alunos) {
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

  for (int i = 0; i < num_alunos; i++) {
    if (strcmp(alunos[i].nome, nome) == 0 && strcmp(alunos[i].senha, senha) == 0) {
      return alunos[i].id; // Usuário aluno
    }
  }

  return 0; // Falha no login
}

void menu_login() {
  printf("\n======= Menu de Login =======\n");
  printf("1. Entrar\n");
  printf("2. Sair\n");
  printf("Escolha uma opção: ");
}

void menu_aluno(int idAluno, Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas) {
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
      exibir_cursos(cursos, num_cursos);
      break;
    case 2:
      exibir_matriculas(idAluno, cursos, num_cursos, alunos, num_alunos, matriculas, num_matriculas);
      break;
    case 3:
      cadastrar_matricula(matriculas, & num_matriculas, idAluno, cursos, num_cursos);
      break;
    case 4:
      printf("Saindo...\n");
      break;
    default:
      printf("Opção inválida. Por favor, escolha uma opção válida.\n");
    }
  } while (opcao != 4);
}

void menu_admin(Aluno alunos[], int num_alunos, Curso cursos[], int num_cursos, Matricula matriculas[], int num_matriculas) {
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
        cadastrar_aluno(alunos, &num_alunos);
        break;
      case 2:
        editar_aluno(alunos, num_alunos);
        break;
      case 3:
        excluir_aluno(alunos, &num_alunos, matriculas, num_matriculas);
        break;
      case 4:
        exibir_alunos(alunos, num_alunos);
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
        cadastrar_curso(cursos, &num_cursos);
        break;
      case 2:
        editar_curso(cursos, num_cursos);
        break;
      case 3:
        excluir_curso(cursos, &num_cursos, matriculas, num_matriculas);
        break;
      case 4:
        exibir_cursos(cursos, num_cursos);
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
        exibir_matriculas(-1, cursos, num_cursos, alunos, num_alunos, matriculas, num_matriculas); // Exibir todas as matrículas
        break;
      case 2:
        printf("Digite o ID do aluno para remover a matrícula: ");
        int id;
        scanf("%d", &id);
        getchar(); // Limpa o buffer de entrada
        excluir_matricula(matriculas, &num_matriculas, id);
        break;
      case 3:
        printf("Digite o ID da matrícula para editar: ");
        int idMatricula;
        scanf("%d", &idMatricula);
        getchar(); // Limpa o buffer de entrada
        editar_matricula(matriculas, num_matriculas, idMatricula, cursos, num_cursos, alunos, num_alunos);
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

void exibir_cursos(Curso cursos[], int num_cursos) {
  limpar_console();
  printf("\n======= Cursos Disponíveis =======\n");
  for (int i = 0; i < num_cursos; i++) {
    printf("ID: %d, Nome: %s, Preço por mês: %.2f, Parcelas: %d, Valor da Parcela: %.2f\n",
      cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas,
      cursos[i].precoPorMes / cursos[i].numParcelas);
  }
}

void exibir_alunos(Aluno alunos[], int num_alunos) {
  limpar_console();
  printf("\n======= Alunos Cadastrados =======\n");
  for (int i = 0; i < num_alunos; i++) {
    printf("ID: %d, Nome: %s, Senha: %s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
  }
}

void exibir_matriculas(int idAluno, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos, Matricula matriculas[], int num_matriculas) {
  limpar_console();
  if (idAluno != -1) {
    printf("\n======= Matrículas =======\n");
    for (int i = 0; i < num_matriculas; i++) {
      if (idAluno == -1 || matriculas[i].idAluno == idAluno) {
        printf("ID da Matrícula: %d, ID do Aluno: %d, ID do Curso: %d, Meses de curso: %d",
               matriculas[i].id, matriculas[i].idAluno, matriculas[i].idCurso, matriculas[i].meses);
      }
    }
  } else {
    printf("\n======= Matrículas =======\n");
    printf("1. Exibir todas as matrículas\n");
    printf("2. Filtrar por ID do Aluno\n");
    printf("3. Filtrar por ID do Curso\n");
    printf("4. Filtrar por Data de Expiração\n");
    printf("5. Voltar ao menu anterior\n");
    printf("Escolha uma opção: ");

    int opcao;
    scanf("%d", &opcao);
    getchar(); // Limpa o buffer de entrada
    limpar_console();
    printf("\n======= Matrículas =======\n");
    switch (opcao) {
      case 1:
        for (int i = 0; i < num_matriculas; i++) {
          printf("ID da Matrícula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses de curso: %d",
                 matriculas[i].id, encontrar_nome_aluno(matriculas[i].idAluno, alunos, num_alunos), 
                 encontrar_nome_curso(matriculas[i].idCurso, cursos, num_cursos), 
                 matriculas[i].meses);
        }
        break;
      case 2:
        int idAlunoFiltro;
        printf("Digite o ID do Aluno: ");
        scanf("%d", &idAlunoFiltro);
        getchar(); // Limpa o buffer de entrada
        limpar_console();
        for (int i = 0; i < num_matriculas; i++) {
          if (matriculas[i].idAluno == idAlunoFiltro) {
            printf("ID da Matrícula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses de curso: %d",
                   matriculas[i].id, encontrar_nome_aluno(matriculas[i].idAluno, alunos, num_alunos), 
                   encontrar_nome_curso(matriculas[i].idCurso, cursos, num_cursos), 
                   matriculas[i].meses);
          }
        }
        break;
      case 3:
        int idCursoFiltro;
        printf("Digite o ID do Curso: ");
        scanf("%d", &idCursoFiltro);
        getchar(); // Limpa o buffer de entrada
        limpar_console();
        for (int i = 0; i < num_matriculas; i++) {
          if (matriculas[i].idCurso == idCursoFiltro) {
            printf("ID da Matrícula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses de curso: %d",
                   matriculas[i].id, encontrar_nome_aluno(matriculas[i].idAluno, alunos, num_alunos), 
                   encontrar_nome_curso(matriculas[i].idCurso, cursos, num_cursos), 
                   matriculas[i].meses);
          }
        }
        break;
      case 4:
        int mesesDeCurso;
        printf("Digite os meses de curso");
        scanf("%d", &dataExpiracaoFiltro);
        getchar(); // Limpa o buffer de entrada
        limpar_console();
        for (int i = 0; i < num_matriculas; i++) {
          if (matriculas[i].meses == dataExpiracaoFiltro) {
            printf("ID da Matrícula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses de curso: %d",
                   matriculas[i].id, encontrar_nome_aluno(matriculas[i].idAluno, alunos, num_alunos), 
                   encontrar_nome_curso(matriculas[i].idCurso, cursos, num_cursos), 
                   matriculas[i].meses);
          }
        }
        break;
      case 5:
        printf("Retornando ao menu anterior...\n");
        break;
      default:
        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
    }
  }
}

void cadastrar_aluno(Aluno alunos[], int * num_alunos) {
  limpar_console();
  printf("Digite o nome do aluno: ");
  fgets(alunos[ * num_alunos].nome, MAX_NOME, stdin);
  alunos[ * num_alunos].nome[strcspn(alunos[ * num_alunos].nome, "\n")] = 0; // Remove o caractere de nova linha
  printf("Digite a senha: ");
  fgets(alunos[ * num_alunos].senha, MAX_NOME, stdin);
  alunos[ * num_alunos].senha[strcspn(alunos[ * num_alunos].senha, "\n")] = 0; // Remove o caractere de nova linha
  alunos[ * num_alunos].id = encontrar_proximo_id(alunos, * num_alunos);
  ( * num_alunos) ++;
  salvar_alunos(alunos, * num_alunos);
  printf("Aluno adicionado com sucesso!\n");
}

void cadastrar_curso(Curso cursos[], int * num_cursos) {
  limpar_console();
  printf("Digite o nome do curso: ");
  fgets(cursos[ * num_cursos].nome, MAX_NOME, stdin);
  cursos[ * num_cursos].nome[strcspn(cursos[ * num_cursos].nome, "\n")] = 0; // Remove o caractere de nova linha
  printf("Digite o preço por mês: ");
  scanf("%f", & cursos[ * num_cursos].precoPorMes);
  getchar(); // Limpa o buffer de entrada
  printf("Digite o número de parcelas: ");
  scanf("%d", & cursos[ * num_cursos].numParcelas);
  getchar(); // Limpa o buffer de entrada
  cursos[ * num_cursos].id = encontrar_proximo_id_curso(cursos, * num_cursos);
  ( * num_cursos) ++;
  salvar_cursos(cursos, * num_cursos);
  printf("Curso adicionado com sucesso!\n");
}

void cadastrar_matricula(Matricula matriculas[], int *num_matriculas, int idAluno, Curso cursos[], int num_cursos) {
  if (*num_matriculas < MAX_MATRICULAS) { // Certifique-se de que há espaço para mais matrículas
    Matricula nova_matricula;
    nova_matricula.id = encontrar_proximo_id_matricula(matriculas, *num_matriculas);
    nova_matricula.idAluno = idAluno;

    printf("Digite o ID do curso para a matrícula: ");
    scanf("%d", &nova_matricula.idCurso);
    getchar(); // Limpa o buffer de entrada

    // Verifica se o curso existe
    int curso_existe = 0;
    for (int i = 0; i < num_cursos; i++) {
      if (cursos[i].id == nova_matricula.idCurso) {
        curso_existe = 1;
        break;
      }
    }
    if (!curso_existe) {
      printf("Curso com ID %d não encontrado.\n", nova_matricula.idCurso);
      return;
    }

    //nova_matricula.dataExpiraca = calcular_data_expiracao();

    matriculas[*num_matriculas] = nova_matricula;
    (*num_matriculas)++;
    
    printf("Matrícula cadastrada com sucesso.\n");
  } else {
    printf("Número máximo de matrículas atingido.\n");
  }
}

void editar_aluno(Aluno alunos[], int num_alunos) {
  limpar_console();
  printf("Digite o ID do aluno para editar: ");
  int id;
  scanf("%d", & id);
  getchar(); // Limpa o buffer de entrada
  for (int i = 0; i < num_alunos; i++) {
    if (alunos[i].id == id) {
      printf("Digite o novo nome: ");
      fgets(alunos[i].nome, MAX_NOME, stdin);
      alunos[i].nome[strcspn(alunos[i].nome, "\n")] = 0; // Remove o caractere de nova linha
      printf("Digite a nova senha: ");
      fgets(alunos[i].senha, MAX_NOME, stdin);
      alunos[i].senha[strcspn(alunos[i].senha, "\n")] = 0; // Remove o caractere de nova linha
      salvar_alunos(alunos, num_alunos);
      printf("Informações do aluno atualizadas com sucesso!\n");
      return;
    }
  }
  printf("Aluno com ID %d não encontrado.\n", id);
}

void editar_curso(Curso cursos[], int num_cursos) {
  limpar_console();
  printf("Digite o ID do curso para editar: ");
  int id;
  scanf("%d", & id);
  getchar(); // Limpa o buffer de entrada
  for (int i = 0; i < num_cursos; i++) {
    if (cursos[i].id == id) {
      printf("Digite o novo nome: ");
      fgets(cursos[i].nome, MAX_NOME, stdin);
      cursos[i].nome[strcspn(cursos[i].nome, "\n")] = 0; // Remove o caractere de nova linha
      printf("Digite o novo preço por mês: ");
      scanf("%f", & cursos[i].precoPorMes);
      getchar(); // Limpa o buffer de entrada
      printf("Digite o novo número de parcelas: ");
      scanf("%d", & cursos[i].numParcelas);
      getchar(); // Limpa o buffer de entrada
      salvar_cursos(cursos, num_cursos);
      printf("Informações do curso atualizadas com sucesso!\n");
      return;
    }
  }
  printf("Curso com ID %d não encontrado.\n", id);
}

void editar_matricula(Matricula matriculas[], int num_matriculas, int idMatricula, Curso cursos[], int num_cursos, Aluno alunos[], int num_alunos) {
  int encontrado = 0;
  limpar_console();
  for (int i = 0; i < num_matriculas; i++) {
    if (matriculas[i].id == idMatricula) {
      encontrado = 1;
      printf("Editar Matrícula:\n");
      printf("Aluno atual: %s\n", encontrar_nome_aluno(matriculas[i].idAluno, alunos, num_alunos));
      printf("Curso atual: %s\n", encontrar_nome_curso(matriculas[i].idCurso, cursos, num_cursos));
      printf("Data de Expiração atual: %s\n", ctime(&matriculas[i].dataExpiracao));

      printf("Digite o novo ID do aluno (ou -1 para manter o atual): ");
      int novoIdAluno;
      scanf("%d", &novoIdAluno);
      getchar(); // Limpa o buffer de entrada
      if (novoIdAluno != -1) {
        matriculas[i].idAluno = novoIdAluno;
      }

      printf("Digite o novo ID do curso (ou -1 para manter o atual): ");
      int novoIdCurso;
      scanf("%d", &novoIdCurso);
      getchar(); // Limpa o buffer de entrada
      if (novoIdCurso != -1) {
        matriculas[i].idCurso = novoIdCurso;
      }

      printf("Digite a nova data de expiração (em segundos desde o Epoch, ou -1 para manter a atual): ");
      time_t novaDataExpiracao;
      scanf("%ld", &novaDataExpiracao);
      getchar(); // Limpa o buffer de entrada
      if (novaDataExpiracao != -1) {
        matriculas[i].dataExpiracao = novaDataExpiracao;
      }

      salvar_matriculas(matriculas, num_matriculas);
      printf("Matrícula editada com sucesso!\n");
      break;
    }
  }

  if (!encontrado) {
    printf("Matrícula com ID %d não encontrada.\n", idMatricula);
  }
}

void excluir_aluno(Aluno alunos[], int * num_alunos, Matricula matriculas[], int num_matriculas) {
  limpar_console();
  printf("Digite o ID do aluno para excluir: ");
  int id;
  scanf("%d", & id);
  getchar(); // Limpa o buffer de entrada
  int encontrado = 0;
  for (int i = 0; i < * num_alunos; i++) {
    if (alunos[i].id == id) {
      for (int j = 0; j < num_matriculas; j++) {
        if (matriculas[j].idAluno == id) {
          // Remover matrículas associadas
          for (int k = j; k < num_matriculas - 1; k++) {
            matriculas[k] = matriculas[k + 1];
          }
          num_matriculas--;
        }
      }
      // Remover aluno
      for (int j = i; j < * num_alunos - 1; j++) {
        alunos[j] = alunos[j + 1];
      }
      ( * num_alunos) --;
      salvar_alunos(alunos, * num_alunos);
      salvar_matriculas(matriculas, num_matriculas);
      encontrado = 1;
      printf("Aluno excluído com sucesso!\n");
      break;
    }
  }
  if (!encontrado) {
    printf("Aluno com ID %d não encontrado.\n", id);
  }
}

void excluir_curso(Curso cursos[], int * num_cursos, Matricula matriculas[], int num_matriculas) {
  limpar_console();
  printf("Digite o ID do curso para excluir: ");
  int id;
  scanf("%d", & id);
  getchar(); // Limpa o buffer de entrada
  int encontrado = 0;
  for (int i = 0; i < * num_cursos; i++) {
    if (cursos[i].id == id) {
      for (int j = 0; j < num_matriculas; j++) {
        if (matriculas[j].idCurso == id) {
          // Remover matrículas associadas
          for (int k = j; k < num_matriculas - 1; k++) {
            matriculas[k] = matriculas[k + 1];
          }
          num_matriculas--;
        }
      }
      // Remover curso
      for (int j = i; j < * num_cursos - 1; j++) {
        cursos[j] = cursos[j + 1];
      }
      ( * num_cursos) --;
      salvar_cursos(cursos, * num_cursos);
      salvar_matriculas(matriculas, num_matriculas);
      encontrado = 1;
      printf("Curso excluído com sucesso!\n");
      break;
    }
  }
  if (!encontrado) {
    printf("Curso com ID %d não encontrado.\n", id);
  }
}

void excluir_matricula(Matricula matriculas[], int * num_matriculas, int idAluno) {
  limpar_console();
  int encontrado = 0;
  for (int i = 0; i < * num_matriculas; i++) {
    if (matriculas[i].idAluno == idAluno) {
      for (int j = i; j < * num_matriculas - 1; j++) {
        matriculas[j] = matriculas[j + 1];
      }
      ( * num_matriculas) --;
      salvar_matriculas(matriculas, * num_matriculas);
      encontrado = 1;
      printf("Matrícula removida com sucesso!\n");
      break;
    }
  }
  if (!encontrado) {
    printf("Nenhuma matrícula encontrada para o ID do aluno %d.\n", idAluno);
  }
}

int encontrar_proximo_id(Aluno alunos[], int num_alunos) {
  if (num_alunos == 0) {
    return 1; // Começa com o ID 1 se nenhum aluno estiver cadastrado
  } else {
    return alunos[num_alunos - 1].id + 1; // Incrementa o último ID
  }
}

int encontrar_proximo_id_curso(Curso cursos[], int num_cursos) {
  if (num_cursos == 0) {
    return 1; // Começa com o ID 1 se nenhum curso estiver cadastrado
  } else {
    return cursos[num_cursos - 1].id + 1; // Incrementa o último ID
  }
}

int encontrar_proximo_id_matricula(Matricula matriculas[], int num_matriculas) {
  if (num_matriculas == 0) {
    return 1; // Começa com o ID 1 se nenhuma matrícula for feita
  } else {
    return matriculas[num_matriculas - 1].id + 1; // Incrementa o último ID
  }
}

char * encontrar_nome_aluno(int idAluno, Aluno alunos[], int num_alunos) {
  for (int i = 0; i < num_alunos; i++) {
    if (alunos[i].id == idAluno) {
      return alunos[i].nome;
    }
  }
  return "Aluno não encontrado";
}

char * encontrar_nome_curso(int idCurso, Curso cursos[], int num_cursos) {
  for (int i = 0; i < num_cursos; i++) {
    if (cursos[i].id == idCurso) {
      return cursos[i].nome;
    }
  }
  return "Curso não encontrado";
}

void salvar_alunos(Aluno alunos[], int num_alunos) {
  FILE * arquivo = fopen("alunos.txt", "w");
  if (arquivo != NULL) {
    for (int i = 0; i < num_alunos; i++) {
      fprintf(arquivo, "%d %s %s\n", alunos[i].id, alunos[i].nome, alunos[i].senha);
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados dos alunos.\n");
  }
}

void salvar_cursos(Curso cursos[], int num_cursos) {
  FILE * arquivo = fopen("cursos.txt", "w");
  if (arquivo != NULL) {
    for (int i = 0; i < num_cursos; i++) {
      fprintf(arquivo, "%d %s %.2f %d\n", cursos[i].id, cursos[i].nome, cursos[i].precoPorMes, cursos[i].numParcelas);
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados dos cursos.\n");
  }
}

void salvar_matriculas(Matricula matriculas[], int num_matriculas) {
  FILE *arquivo = fopen("matriculas.txt", "w");
  if (arquivo != NULL) {
    for (int i = 0; i < num_matriculas; i++) {
      fprintf(arquivo, "%d %d %d %ld\n", matriculas[i].id, matriculas[i].idAluno, matriculas[i].idCurso, matriculas[i].dataExpiracao);
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados das matrículas.\n");
  }
}

void carregar_alunos(Aluno alunos[], int * num_alunos) {
  FILE * arquivo = fopen("alunos.txt", "r");
  if (arquivo != NULL) {
    * num_alunos = 0;
    while (fscanf(arquivo, "%d %s %s", & alunos[ * num_alunos].id, alunos[ * num_alunos].nome, alunos[ * num_alunos].senha) != EOF) {
      ( * num_alunos) ++;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao carregar os dados dos alunos.\n");
  }
}

void carregar_cursos(Curso cursos[], int * num_cursos) {
  FILE * arquivo = fopen("cursos.txt", "r");
  if (arquivo != NULL) {
    * num_cursos = 0;
    while (fscanf(arquivo, "%d %s %f %d\n", & cursos[ * num_cursos].id, cursos[ * num_cursos].nome, & cursos[ * num_cursos].precoPorMes, & cursos[ * num_cursos].numParcelas) != EOF) {
      ( * num_cursos) ++;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao carregar os dados dos cursos.\n");
  }
}

void carregar_matriculas(Matricula matriculas[], int *num_matriculas) {
  FILE *arquivo = fopen("matriculas.txt", "r");
  if (arquivo != NULL) {
    *num_matriculas = 0;
    while (fscanf(arquivo, "%d %d %d %d", &matriculas[*num_matriculas].id, &matriculas[*num_matriculas].idAluno, &matriculas[*num_matriculas].idCurso, &matriculas[*num_matriculas].dataExpiracao) != EOF) {
      (*num_matriculas)++;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao carregar os dados das matrículas.\n");
  }
}

void carregar_vencimentos(Vencimento vencimentos[], int *num_vencimentos) {
    FILE *file = fopen("vencimentos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de vencimentos.\n");
        return;
    }

    *num_vencimentos = 0;
    while (fscanf(file, "%d %d %f %d %s\n", &vencimentos[*num_vencimentos].idVencimento, &vencimentos[*num_vencimentos].idMatricula, &vencimentos[*num_vencimentos].valorParcela, &vencimentos[*num_vencimentos].pago, vencimentos[*num_vencimentos].dataVencimento) != EOF) {
        (*num_vencimentos)++;
    }

    fclose(file);
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
  Aluno alunos[100];
  Curso cursos[100];
  Matricula matriculas[100];
  Vencimento vencimentos[100];
  int num_alunos = 0, num_cursos = 0, num_matriculas = 0;

  int opcao;
  do {
    carregar_alunos(alunos, & num_alunos);
    carregar_cursos(cursos, & num_cursos);
    carregar_matriculas(matriculas, & num_matriculas);
    limpar_console();
    menu_login();
    scanf("%d", & opcao);
    getchar(); // Limpa o buffer de entrada
    limpar_console();

    switch (opcao) {
    case 1: {
      int id = login(alunos, num_alunos);
      limpar_console();
      if (id == -1) {
        menu_admin(alunos, num_alunos, cursos, num_cursos, matriculas, num_matriculas, vencimentos, num_vencimentos);
      } else if (id > 0) {
        menu_aluno(id, alunos, num_alunos, cursos, num_cursos, matriculas, num_matriculas, vencimentos, num_vencimentos);
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
