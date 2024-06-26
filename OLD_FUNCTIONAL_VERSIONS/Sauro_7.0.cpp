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

typedef struct Vencimento {
    time_t data;
    float valor;
    struct Vencimento* proximo;
    int pago;
} Vencimento;


typedef struct Matricula {
    int id;
    int idAluno;
    int idCurso;
    int meses; // N�mero de meses que a matr�cula ser� ativa
    Vencimento* vencimentos; // Lista de vencimentos
    struct Matricula* next;
} Matricula;

// Prot�tipos de Fun��o
void cadastrar_aluno(Aluno** alunos);
void cadastrar_curso(Curso** cursos);
void cadastrar_matricula(Matricula** matriculas, int idAluno, Curso* cursos);
Aluno* carregar_alunos();
Curso* carregar_cursos();
void editar_aluno(Aluno* alunos);
void editar_curso(Curso* cursos);
void editar_matricula(Matricula* matricula, Curso* cursos, Aluno* alunos);
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
Matricula* carregar_matriculas(Curso* cursos);
Aluno* encontrar_aluno_por_id(int idAluno, Aluno* alunos);
Curso* encontrar_curso_por_id(int idCurso, Curso* cursos);
Vencimento* carregar_vencimentos(int idMatricula);
void salvar_alunos(Aluno* alunos);
void salvar_cursos(Curso* cursos);
void salvar_matriculas(Matricula* matriculas);
time_t calcular_data_expiracao();
int custom_strptime(const char *buf, const char *format, struct tm *tm);

// Fun��es relacionadas aos vencimentos
Vencimento* criar_vencimento(time_t data, float valor, int pago);
void adicionar_vencimento(Matricula* matricula, time_t data, float valor, int pago);
void exibir_vencimentos(Vencimento* vencimentos);
void salvar_vencimentos(Vencimento* vencimentos, int idMatricula);
void liberar_vencimentos(Vencimento* vencimentos);
void excluir_vencimento_admin(Matricula* matriculas);
void menu_admin_vencimentos(Matricula* matriculas, Curso* cursos);
void editar_vencimento_admin(Matricula* matriculas);
void criar_vencimento_admin(Matricula* matriculas, Curso* cursos);
void pagar_vencimento(int idAluno, Matricula* matriculas);
// Implementa��o das fun��es omitidas...

int custom_strptime(const char *buf, const char *format, struct tm *tm) {
    int day, month, year;
    if (sscanf(buf, "%d/%d/%d", &day, &month, &year) != 3) {
        return 0; // Parsing failed
    }
    tm->tm_mday = day;
    tm->tm_mon = month - 1; // tm_mon is 0-based
    tm->tm_year = year - 1900; // tm_year is years since 1900
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = -1; // Not set by strptime, should be set to -1
    return 1; // Parsing successful
}

Vencimento* criar_vencimento(time_t data, float valor, int pago) {
    Vencimento* novo_vencimento = (Vencimento*)malloc(sizeof(Vencimento));
    novo_vencimento->data = data;
    novo_vencimento->valor = valor;
    novo_vencimento->pago = pago;
    novo_vencimento->proximo = NULL;
    return novo_vencimento;
}

void adicionar_vencimento(Matricula* matricula, time_t data, float valor, int pago) {
    Vencimento* novo_vencimento = criar_vencimento(data, valor, pago);
    novo_vencimento->proximo = matricula->vencimentos;
    matricula->vencimentos = novo_vencimento;
}

void exibir_vencimentos(Vencimento* vencimentos) {
    printf("\n======= Vencimentos =======\n");
    while (vencimentos != NULL) {
        char data_str[100];
        struct tm* data = localtime(&vencimentos->data);
        strftime(data_str, sizeof(data_str), "%d/%m/%Y", data);
        printf("Data: %s - Valor: %.2f - Pago: %s\n", data_str, vencimentos->valor, vencimentos->pago ? "Sim" : "N�o");
        vencimentos = vencimentos->proximo;
    }
}

void salvar_vencimentos(Vencimento* vencimentos, int idMatricula) {
    FILE* file = fopen("vencimentos.txt", "a");
    if (!file) {
        perror("Erro ao abrir o arquivo de vencimentos");
        return;
    }

    Vencimento* current = vencimentos;
    while (current != NULL) {
        fprintf(file, "%d,%ld,%.2f,%d\n", idMatricula, current->data, current->valor, current->pago);
        current = current->proximo;
    }

    fclose(file);
}

void menu_admin_vencimentos(Matricula* matriculas, Curso* cursos) {
    int opcao;
    do {
        printf("\n======= Gerenciamento de Vencimentos =======\n");
        printf("1. Criar vencimento\n");
        printf("2. Editar vencimento\n");
        printf("3. Excluir vencimento\n");
        printf("4. Sair\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada

        switch (opcao) {
            case 1:
                criar_vencimento_admin(matriculas, cursos);
                break;
            case 2:
                editar_vencimento_admin(matriculas);
                break;
            case 3:
                excluir_vencimento_admin(matriculas);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
        }
    } while (opcao != 4);
}

void liberar_vencimentos(Vencimento* vencimentos) {
    while (vencimentos != NULL) {
        Vencimento* temp = vencimentos;
        vencimentos = vencimentos->proximo;
        free(temp);
    }
}

void excluir_vencimento_admin(Matricula* matriculas) {
    int idMatricula;
    printf("Informe o ID da matr�cula: ");
    scanf("%d", &idMatricula);

    Matricula* matricula = matriculas;
    while (matricula != NULL) {
        if (matricula->id == idMatricula) {
            time_t data;
            struct tm tm_data;
            char data_str[100];

            printf("Informe a data do vencimento a excluir (dd/mm/aaaa): ");
            scanf("%s", data_str);
            if (!custom_strptime(data_str, "%d/%m/%Y", &tm_data)) {
                printf("Formato de data inv�lido.\n");
                return;
            }
            data = mktime(&tm_data);

            Vencimento** vencimento_ptr = &matricula->vencimentos;
            while (*vencimento_ptr != NULL) {
                if (difftime((*vencimento_ptr)->data, data) == 0) {
                    Vencimento* temp = *vencimento_ptr;
                    *vencimento_ptr = (*vencimento_ptr)->proximo;
                    free(temp);
                    salvar_vencimentos(matricula->vencimentos, matricula->id);
                    printf("Vencimento exclu�do com sucesso.\n");
                    return;
                }
                vencimento_ptr = &(*vencimento_ptr)->proximo;
            }
            printf("Vencimento n�o encontrado.\n");
            return;
        }
        matricula = matricula->next;
    }
    printf("Matr�cula n�o encontrada.\n");
}

void editar_vencimento_admin(Matricula* matriculas) {
    int idMatricula;
    printf("Informe o ID da matr�cula: ");
    scanf("%d", &idMatricula);

    Matricula* matricula = matriculas;
    while (matricula != NULL) {
        if (matricula->id == idMatricula) {
            time_t data;
            struct tm tm_data;
            char data_str[100];

            printf("Informe a data do vencimento a editar (dd/mm/aaaa): ");
            scanf("%s", data_str);
            if (!custom_strptime(data_str, "%d/%m/%Y", &tm_data)) {
                printf("Formato de data inv�lido.\n");
                return;
            }
            data = mktime(&tm_data);

            Vencimento* vencimento = matricula->vencimentos;
            while (vencimento != NULL) {
                if (difftime(vencimento->data, data) == 0) {
                    printf("Informe o novo valor do vencimento: ");
                    scanf("%f", &vencimento->valor);

                    printf("Informe o novo status de pagamento (0 para n�o pago, 1 para pago): ");
                    scanf("%d", &vencimento->pago);

                    salvar_vencimentos(matricula->vencimentos, matricula->id);
                    printf("Vencimento editado com sucesso.\n");
                    return;
                }
                vencimento = vencimento->proximo;
            }
            printf("Vencimento n�o encontrado.\n");
            return;
        }
        matricula = matricula->next;
    }
    printf("Matr�cula n�o encontrada.\n");
}

void criar_vencimento_admin(Matricula* matriculas, Curso* cursos) {
    int idMatricula;
    printf("Informe o ID da matr�cula: ");
    scanf("%d", &idMatricula);

    Matricula* matricula = matriculas;
    while (matricula != NULL) {
        if (matricula->id == idMatricula) {
            time_t now = time(NULL);
            struct tm tm_data;
            char data_str[100];
            float valor;
            int pago;

            printf("Informe a data do vencimento (dd/mm/aaaa): ");
            scanf("%s", data_str);
            if (!custom_strptime(data_str, "%d/%m/%Y", &tm_data)) {
                printf("Formato de data inv�lido.\n");
                return;
            }
            time_t data = mktime(&tm_data);

            printf("Informe o valor do vencimento: ");
            scanf("%f", &valor);

            printf("Informe o status de pagamento (0 para n�o pago, 1 para pago): ");
            scanf("%d", &pago);

            adicionar_vencimento(matricula, data, valor, pago);
            salvar_vencimentos(matricula->vencimentos, matricula->id);
            printf("Vencimento criado com sucesso.\n");
            return;
        }
        matricula = matricula->next;
    }
    printf("Matr�cula n�o encontrada.\n");
}

void pagar_vencimento(int idAluno, Matricula* matriculas) {
    Matricula* matricula = matriculas;
    while (matricula != NULL) {
        if (matricula->idAluno == idAluno) {
            Vencimento* vencimento = matricula->vencimentos;
            Vencimento* mais_recente = NULL;
            while (vencimento != NULL) {
                if (!vencimento->pago) {
                    if (mais_recente == NULL || difftime(vencimento->data, mais_recente->data) > 0) {
                        mais_recente = vencimento;
                    }
                }
                vencimento = vencimento->proximo;
            }
            if (mais_recente != NULL) {
                mais_recente->pago = 1;
                salvar_vencimentos(matricula->vencimentos, matricula->id);
                printf("Vencimento pago com sucesso.\n");
                return;
            }
        }
        matricula = matricula->next;
    }
    printf("N�o h� vencimentos pendentes para pagar.\n");
}

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
  printf("Digite o nome de usu�rio: ");
  fgets(nome, MAX_NOME, stdin);
  nome[strcspn(nome, "\n")] = 0; // Remove o caractere de nova linha
  printf("Digite a senha: ");
  fgets(senha, MAX_NOME, stdin);
  senha[strcspn(senha, "\n")] = 0; // Remove o caractere de nova linha

  if (strcmp("admin", nome) == 0 && strcmp("admin", senha) == 0) {
    return -1; // Usu�rio administrador
  }

  Aluno* current = alunos;
  while (current != NULL) {
    if (strcmp(current->nome, nome) == 0 && strcmp(current->senha, senha) == 0) {
      return current->id; // Usu�rio aluno
    }
    current = current->next;
  }

  return 0; // Falha no login
}

void menu_login() {
  printf("\n======= Menu de Login =======\n");
  printf("1. Entrar\n");
  printf("2. Sair\n");
  printf("Escolha uma op��o: ");
}

void menu_aluno(int idAluno, Aluno* alunos, Curso* cursos, Matricula* matriculas) {
  int opcao;
  do {
    printf("\n======= Menu do Aluno =======\n");
    printf("1. Ver cursos dispon�veis\n");
    printf("2. Ver minhas matr�culas\n");
    printf("3. Adicionar nova matr�cula\n");
    printf("4. Pagar vencimento\n");
    printf("5. Sair\n");
    printf("Escolha uma op��o: ");
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
      pagar_vencimento(idAluno, matriculas);
      break;
    case 5:
      printf("Saindo...\n");
      break;
    default:
      printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
    }
  } while (opcao != 4);
}

void menu_admin(Aluno** alunos, Curso** cursos, Matricula** matriculas) {
    int opcao_admin_menu;
    do {
        printf("\n======= Menu Administrativo =======\n");
        printf("1. Gerenciamento de Alunos\n");
        printf("2. Gerenciamento de Cursos\n");
        printf("3. Gerenciamento de Matr�culas\n");
        printf("4. Gerenciamento de Vencimentos\n");
        printf("5. Sair\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao_admin_menu);
        getchar(); // Limpa o buffer de entrada
        limpar_console();
        int opcao_admin;

        switch (opcao_admin_menu) {
        case 1:
            // Gerenciamento de Alunos
            printf("\n======= Gerenciamento de Alunos =======\n");
            printf("1. Adicionar aluno\n");
            printf("2. Editar aluno\n");
            printf("3. Excluir aluno\n");
            printf("4. Ver alunos\n");
            printf("Escolha uma op��o: ");
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
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
            }
            break;
        case 2:
            // Gerenciamento de Cursos
            printf("\n======= Gerenciamento de Cursos =======\n");
            printf("1. Adicionar curso\n");
            printf("2. Editar curso\n");
            printf("3. Excluir curso\n");
            printf("4. Ver cursos\n");
            printf("Escolha uma op��o: ");
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
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
            }
            break;
        case 3:
            // Gerenciamento de Matr�culas
            printf("\n======= Gerenciamento de Matr�culas =======\n");
            printf("1. Ver matr�culas\n");
            printf("2. Remover matr�cula\n");
            printf("3. Editar matr�cula\n");
            printf("Escolha uma op��o: ");
            scanf("%d", &opcao_admin);
            getchar(); // Limpa o buffer de entrada
            switch (opcao_admin) {
            case 1:
                exibir_matriculas(-1, *cursos, *alunos, *matriculas); // Exibir todas as matr�culas
                break;
            case 2:
                {
                    printf("Digite o ID do aluno para remover a matr�cula: ");
                    int id;
                    scanf("%d", &id);
                    getchar(); // Limpa o buffer de entrada
                    excluir_matricula(matriculas, id);
                }
                break;
            case 3:
                {
                    printf("Digite o ID da matr�cula para editar: ");
                    int idMatricula;
                    scanf("%d", &idMatricula);
                    getchar(); // Limpa o buffer de entrada
                    editar_matricula(*matriculas, *cursos, *alunos);
                }
                break;
            default:
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
            }
            break;
        case 4:
            menu_admin_vencimentos(*matriculas, *cursos);
            break;
        case 5:
                printf("Saindo...\n");
                break;
        default:
            printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
        }
    } while (opcao_admin_menu != 4);
}

void exibir_cursos(Curso* cursos) {
  limpar_console();
  printf("\n======= Cursos Dispon�veis =======\n");
  Curso* current = cursos;
  while (current != NULL) {
    printf("ID: %d, Nome: %s, Pre�o por m�s: %.2f, Parcelas: %d\n", current->id, current->nome, current->precoPorMes, current->numParcelas);
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
    if (idAluno != -1) {
        printf("\n======= Matr�culas =======\n");
        Matricula* current = matriculas;
        while (current != NULL) {
            if (idAluno == -1 || current->idAluno == idAluno) {
                Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
                Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
                printf("ID da Matr�cula: %d, ID do Aluno: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
                       current->id, current->idAluno, 
                       aluno != NULL ? aluno->nome : "Aluno n�o encontrado", 
                       curso != NULL ? curso->nome : "Curso n�o encontrado", 
                       current->meses);
            }
            current = current->next;
        }
    } else {
    	
        printf("\n======= Matr�culas =======\n");
        printf("1. Exibir todas as matr�culas\n");
        printf("2. Filtrar por ID do Aluno\n");
        printf("3. Filtrar por ID do Curso\n");
        printf("4. Filtrar por N�mero de Meses\n");
        printf("5. Voltar ao menu anterior\n");
        printf("Escolha uma op��o: ");

        int opcao;
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada
        limpar_console();
        printf("\n======= Matr�culas =======\n");
		
        Matricula* current;
        switch (opcao) {
            case 1:
                current = matriculas;
                while (current != NULL) {
                    Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
                    Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
                    printf("ID da Matr�cula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
                           current->id, 
                           aluno != NULL ? aluno->nome : "Aluno n�o encontrado", 
                           curso != NULL ? curso->nome : "Curso n�o encontrado", 
                           current->meses);
                           
                    current = current->next;
                }
                
                break;
            case 2:
                {
                    int idAlunoFiltro;
                    printf("Digite o ID do Aluno: ");
                    scanf("%d", &idAlunoFiltro);
                    getchar(); // Limpa o buffer de entrada
                    limpar_console();
                    current = matriculas;
                    while (current != NULL) {
                        if (current->idAluno == idAlunoFiltro) {
                            Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
                            Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
                            printf("ID da Matr�cula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
                                   current->id, 
                                   aluno != NULL ? aluno->nome : "Aluno n�o encontrado", 
                                   curso != NULL ? curso->nome : "Curso n�o encontrado", 
                                   current->meses);
                        }
                        current = current->next;
                    }
                }
                break;
            case 3:
                {
                    int idCursoFiltro;
                    printf("Digite o ID do Curso: ");
                    scanf("%d", &idCursoFiltro);
                    getchar(); // Limpa o buffer de entrada
                    limpar_console();
                    current = matriculas;
                    while (current != NULL) {
                        if (current->idCurso == idCursoFiltro) {
                            Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
                            Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
                            printf("ID da Matr�cula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
                                   current->id, 
                                   aluno != NULL ? aluno->nome : "Aluno n�o encontrado", 
                                   curso != NULL ? curso->nome : "Curso n�o encontrado", 
                                   current->meses);
                        }
                        current = current->next;
                    }
                }
                break;
            case 4:
                {
                    int mesesFiltro;
                    printf("Digite o n�mero de meses: ");
                    scanf("%d", &mesesFiltro);
                    getchar(); // Limpa o buffer de entrada
                    limpar_console();
                    current = matriculas;
                    while (current != NULL) {
                        if (current->meses == mesesFiltro) {
                            Aluno* aluno = encontrar_aluno_por_id(current->idAluno, alunos);
                            Curso* curso = encontrar_curso_por_id(current->idCurso, cursos);
                            printf("ID da Matr�cula: %d, Nome do Aluno: %s, Nome do Curso: %s, Meses: %d\n",
                                   current->id, 
                                   aluno != NULL ? aluno->nome : "Aluno n�o encontrado", 
                                   curso != NULL ? curso->nome : "Curso n�o encontrado", 
                                   current->meses);
                        }
                        current = current->next;
                    }
                }
                break;
            case 5:
                printf("Retornando ao menu anterior...\n");
                break;
            default:
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
        }
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
  printf("Digite o pre�o por m�s do curso: ");
  scanf("%f", &novo_curso->precoPorMes);
  printf("Digite o n�mero de parcelas do curso: ");
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

    // Ask for number of months for the subscription
    int numMeses;
    printf("Digite a quantidade de meses para a matr�cula: ");
    scanf("%d", &numMeses);
    getchar(); // Clear input buffer

    // Calculate due dates for the subscription
    time_t hoje = time(NULL);
    for (int i = 0; i < numMeses; i++) {
        Vencimento* novo_vencimento = (Vencimento*)malloc(sizeof(Vencimento));
        novo_vencimento->data = hoje + (i * 30 * 24 * 60 * 60); // Adding i months worth of seconds
        novo_vencimento->valor = cursos->precoPorMes;
        novo_vencimento->proximo = NULL;

        if (i == 0) {
            nova_matricula->vencimentos = novo_vencimento;
        } else {
            Vencimento* ultimo = nova_matricula->vencimentos;
            while (ultimo->proximo != NULL) {
                ultimo = ultimo->proximo;
            }
            ultimo->proximo = novo_vencimento;
        }
    }

    nova_matricula->next = *matriculas;
    *matriculas = nova_matricula;
    salvar_matriculas(*matriculas);
    printf("Matr�cula cadastrada com sucesso.\n");
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
  printf("Aluno n�o encontrado.\n");
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
      printf("Digite o novo pre�o por m�s do curso: ");
      scanf("%f", &current->precoPorMes);
      printf("Digite o novo n�mero de parcelas do curso: ");
      scanf("%d", &current->numParcelas);
      getchar(); // Limpa o buffer de entrada
      salvar_cursos(cursos);
      printf("Curso editado com sucesso.\n");
      return;
    }
    current = current->next;
  }
  printf("Curso n�o encontrado.\n");
}

void editar_matricula(Matricula* matricula, Curso* cursos, Aluno* alunos) {
    // Prompt user for new course ID and subscription months
    printf("Digite o novo ID do curso: ");
    scanf("%d", &matricula->idCurso);
    getchar(); // Limpa o buffer de entrada
    printf("Digite a nova quantidade de meses para a matr�cula: ");
    int numMeses;
    scanf("%d", &numMeses);
    getchar(); // Clear input buffer

    // Free existing due dates
    liberar_vencimentos(matricula->vencimentos);
    matricula->vencimentos = NULL;

    // Recalculate and add new due dates
    time_t hoje = time(NULL);
    for (int i = 0; i < numMeses; i++) {
        Vencimento* novo_vencimento = (Vencimento*)malloc(sizeof(Vencimento));
        novo_vencimento->data = hoje + (i * 30 * 24 * 60 * 60); // Adding i months worth of seconds
        novo_vencimento->valor = cursos->precoPorMes;
        novo_vencimento->proximo = NULL;

        if (i == 0) {
            matricula->vencimentos = novo_vencimento;
        } else {
            Vencimento* ultimo = matricula->vencimentos;
            while (ultimo->proximo != NULL) {
                ultimo = ultimo->proximo;
            }
            ultimo->proximo = novo_vencimento;
        }
    }

    salvar_matriculas(matricula);
    printf("Matr�cula editada com sucesso.\n");
}

void excluir_aluno(Aluno** alunos, Matricula** matriculas) {
  int id;
  printf("Digite o ID do aluno a ser exclu�do: ");
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
      excluir_matricula(matriculas, id); // Excluir matr�culas do aluno
      printf("Aluno exclu�do com sucesso.\n");
      return;
    }
    previous = current;
    current = current->next;
  }
  printf("Aluno n�o encontrado.\n");
}

void excluir_curso(Curso** cursos, Matricula** matriculas) {
  int id;
  printf("Digite o ID do curso a ser exclu�do: ");
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
      printf("Curso exclu�do com sucesso.\n");
      return;
    }
    previous = current;
    current = current->next;
  }
  printf("Curso n�o encontrado.\n");
}

void excluir_matricula(Matricula** matriculas, int idMatricula) {
    Matricula* current = *matriculas;
    Matricula* previous = NULL;
    while (current != NULL) {
        if (current->id == idMatricula) {
            if (previous == NULL) {
                *matriculas = current->next;
            } else {
                previous->next = current->next;
            }
            // Free memory associated with due dates
            liberar_vencimentos(current->vencimentos);
            free(current);
            salvar_matriculas(*matriculas);
            printf("Matr�cula exclu�da com sucesso.\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Matr�cula n�o encontrada.\n");
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

Matricula* carregar_matriculas(Curso* cursos) {
    FILE* file = fopen("matriculas.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de matr�culas");
        return NULL;
    }

    Matricula* head = NULL;
    Matricula* current = NULL;
    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), file)) {
        Matricula* nova_matricula = (Matricula*)malloc(sizeof(Matricula));

        sscanf(linha, "%d,%d,%d,%d", &nova_matricula->id, &nova_matricula->idAluno, &nova_matricula->idCurso, &nova_matricula->meses);
        nova_matricula->next = NULL;
        nova_matricula->vencimentos = carregar_vencimentos(nova_matricula->id);

        if (!nova_matricula->vencimentos) {
            Curso* curso = encontrar_curso_por_id(nova_matricula->idCurso, cursos);
            if (curso) {
                time_t now = time(NULL);
                for (int i = 0; i < nova_matricula->meses; ++i) {
                    time_t vencimento_data = now + (i * 30 * 24 * 3600); // Assuming 30 days per month
                    adicionar_vencimento(nova_matricula, vencimento_data, curso->precoPorMes, 0);
                }
            }
        }

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

Aluno* encontrar_aluno_por_id(int idAluno, Aluno* alunos) {
    while (alunos != NULL) {
        if (alunos->id == idAluno) {
            return alunos;
        }
        alunos = alunos->next;
    }
    return NULL; // Return NULL if not found
}

Curso* encontrar_curso_por_id(int idCurso, Curso* cursos) {
    while (cursos != NULL) {
        if (cursos->id == idCurso) {
            return cursos;
        }
        cursos = cursos->next;
    }
    return NULL; // Return NULL if not found
}

Vencimento* carregar_vencimentos(int idMatricula) {
    FILE* file = fopen("vencimentos.txt", "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de vencimentos");
        return NULL;
    }

    Vencimento* head = NULL;
    Vencimento* current = NULL;
    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), file)) {
        int matriculaId;
        time_t data;
        float valor;
        int pago;

        sscanf(linha, "%d,%ld,%f,%d", &matriculaId, &data, &valor, &pago);
        if (matriculaId == idMatricula) {
            Vencimento* novo_vencimento = criar_vencimento(data, valor, pago);
            if (!head) {
                head = novo_vencimento;
            } else {
                current->proximo = novo_vencimento;
            }
            current = novo_vencimento;
        }
    }

    fclose(file);
    return head;
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
      fprintf(arquivo, "%d %d %d %d\n", matriculas->id, matriculas->idAluno, matriculas->idCurso, matriculas->meses);
      matriculas = matriculas->next;
    }
    fclose(arquivo);
  } else {
    printf("Erro ao salvar os dados das matr�culas.\n");
  }
}

time_t calcular_data_expiracao() {
  time_t agora = time(NULL);
  struct tm * tempo_atual = localtime( & agora);
  tempo_atual -> tm_mon++; // Incrementa o m�s j� que janeiro � representado por 0
  tempo_atual -> tm_year += tempo_atual -> tm_mon / 12; // Ajusta o ano se o m�s exceder 12
  tempo_atual -> tm_mon %= 12; // Garante que o m�s esteja entre 0 e 11
  return mktime(tempo_atual);
}

int main() {
    Aluno* alunos = NULL;
    Curso* cursos = NULL;
    Matricula* matriculas = NULL;

    int opcao;
    do {
        Aluno* alunos = carregar_alunos();
    	Curso* cursos = carregar_cursos();
    	Matricula* matriculas = carregar_matriculas(cursos);
        //limpar_console();
        menu_login();
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada
        //limpar_console();

        switch (opcao) {
            case 1: {
                int id = login(alunos);
                //limpar_console();
                if (id == -1) {
                    menu_admin(&alunos, &cursos, &matriculas);
                } else if (id > 0) {
                    menu_aluno(id, alunos, cursos, matriculas);
                } else {
                    printf("Nome de usu�rio ou senha inv�lidos. Por favor, tente novamente.\n");
                }
                break;
            }
            case 2:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Op��o inv�lida. Por favor, escolha uma op��o v�lida.\n");
        }
    } while (opcao != 2);

    return 0;
}
